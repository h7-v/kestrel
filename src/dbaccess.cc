#include "src/dbaccess.h"

BlockDBAccess::BlockDBAccess(const std::string &dbpath)
    : block_database_file_path_(dbpath) {
    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = true;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path_, &blockdb);

    if (!blockdbStatus.ok()) std::cerr << blockdbStatus.ToString() << std::endl;
    assert(blockdbStatus.ok());

    if (blockdbStatus.ok()) {
        blockdb_status_ = (blockdbStatus.ToString());
    }

    delete blockdb;
}

BlockDBAccess::~BlockDBAccess() {}

std::string BlockDBAccess::getBlockDBStatus() const {
    return blockdb_status_;
}

LatestInBlockDB BlockDBAccess::getLatestInBlockDBforUI() const {
    LatestInBlockDB latestData;

    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path_, &blockdb);

    assert(blockdbStatus.ok());

    leveldb::Iterator *it = blockdb->NewIterator(leveldb::ReadOptions());
    it->SeekToLast();
    latestData.latest_key = it->key().ToString();
    latestData.latest_value = it->value().ToString();
    assert(it->status().ok());
    delete it;
    delete blockdb;

    // Removes the '1' at the very beginning of the value. This '1' represents
    // if the block has been mined or not, but we do not want to see it
    // displayed in the UI.
    latestData.latest_value.erase(latestData.latest_value.begin());

    return latestData;
}

std::string BlockDBAccess::getBlockDBValueByKeyforUI(const std::string &key) const {
    std::string value;
    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path_, &blockdb);

    assert(blockdbStatus.ok());

    blockdb->Get(leveldb::ReadOptions(), key, &value);
    delete blockdb;

    // Removes the '1' at the very beginning of the value. This '1' represents
    // if the block has been mined or not, but we do not want to see it
    // displayed in the UI.
    value.erase(value.begin());

    return value;
}

std::string BlockDBAccess::getLatestBlockDBHash() const {
    std::string value_buffer = getLatestInBlockDBforUI().latest_value;
    std::string hashString = "HASH: ";
    int startingChar = 6;
    int finishingChar = 0;
    char hash[65];

    size_t found = value_buffer.find(hashString);
    if (found != std::string::npos) {
        startingChar += found;
    }

    finishingChar = startingChar + 64;

    for (int i = 0; startingChar < finishingChar; i++, startingChar++) {
        hash[i] = value_buffer[startingChar];
    }
    hash[64] = '\0';

    return hash;
}

TxRangeFromBlockDB BlockDBAccess::getTxRangeFromLatestBlock() const {
    TxRangeFromBlockDB range;
    std::string latest_block_value = getLatestInBlockDBforUI().latest_value;
    std::string tx_range_string = "Transaction Range: ";

    // Getting range start value
    int s_starting_char = 19;
    int s_finishing_char = 0;
    char start[10];  // Length of number when padded with zeros.

    size_t s_found = latest_block_value.find(tx_range_string);
    if (s_found != std::string::npos) {
        s_starting_char += s_found;
    }

    s_finishing_char = s_starting_char + 9;

    for (int i = 0; s_starting_char < s_finishing_char; i++, s_starting_char++) {
        start[i] = latest_block_value[s_starting_char];
    }
    start[9] = '\0';

    range.tx_range_start = std::stoi(start);

    // Getting range end value
    int e_starting_char = 31;
    int e_finishing_char = 0;  // Declaring a finishing char twice for readability.
    char end[10];  // Length of number when padded with zeros.

    size_t e_found = latest_block_value.find(tx_range_string);
    if (e_found != std::string::npos) {
        e_starting_char += e_found;
    }

    e_finishing_char = e_starting_char + 9;

    for (int i = 0; e_starting_char < e_finishing_char; i++, e_starting_char++) {
        end[i] = latest_block_value[e_starting_char];
    }
    end[9] = '\0';

    range.tx_range_end = std::stoi(end);

    return range;
}

void BlockDBAccess::onPreviousBlockInMinedState() {
    previousDBBlockIsMined = true;
}

void BlockDBAccess::checkIfLatestBlockDBIsMined() {
    cthread_ = new CheckerThread();
    cthread_->setFilePath(block_database_file_path_);

    connect(cthread_, &CheckerThread::previousBlockInMinedState,
            this, &BlockDBAccess::onPreviousBlockInMinedState);


    cthread_->start();
    cthread_->wait();
    delete cthread_;
}

bool BlockDBAccess::getPreviousDBBlockIsMined() const {
    return previousDBBlockIsMined;
}

void BlockDBAccess::putInBlockDB(const Block &b) const {
    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path_, &blockdb);

    if (!blockdbStatus.ok()) std::cerr << blockdbStatus.ToString() << std::endl;
    assert(blockdbStatus.ok());

    if (blockdbStatus.ok()) {
        blockdb->Put(leveldb::WriteOptions(), b.getIndex(),
                     b.getBlockContents());
        assert(blockdbStatus.ok());
    }

    delete blockdb;
}

void BlockDBAccess::deleteFromBlockDB(const std::string &key) const {
    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path_, &blockdb);

    if (!blockdbStatus.ok()) std::cerr << blockdbStatus.ToString() << std::endl;
    assert(blockdbStatus.ok());

    if (blockdbStatus.ok()) {
        blockdb->Delete(leveldb::WriteOptions(), key);
        assert(blockdbStatus.ok());
    }

    delete blockdb;
}

// ----------------------------------------------------------------------------

TransactionDBAccess::TransactionDBAccess(const std::string &dbpath)
    : transaction_database_file_path_(dbpath) {
    leveldb::DB *transactiondb;
    leveldb::Options transactiondbOptions;
    transactiondbOptions.create_if_missing = true;

    leveldb::Status transactiondbStatus = leveldb::DB::Open(
                transactiondbOptions, transaction_database_file_path_,
                &transactiondb);

    if (!transactiondbStatus.ok()) std::cerr << transactiondbStatus.ToString() << std::endl;
    assert(transactiondbStatus.ok());

    if (transactiondbStatus.ok()) {
        txdb_status_ = (transactiondbStatus.ToString());
    }

    delete transactiondb;
}

TransactionDBAccess::~TransactionDBAccess() {}

std::string TransactionDBAccess::getTxDBStatus() const {
    return txdb_status_;
}

void TransactionDBAccess::putInTxDB(const Transaction &t) {
    leveldb::DB *transactiondb;
    leveldb::Options transactiondbOptions;
    transactiondbOptions.create_if_missing = false;

    leveldb::Status transactiondbStatus = leveldb::DB::Open(
                transactiondbOptions, transaction_database_file_path_,
                &transactiondb);

    if (!transactiondbStatus.ok()) std::cerr << transactiondbStatus.ToString() << std::endl;
    assert(transactiondbStatus.ok());

    if (transactiondbStatus.ok()) {
        transactiondb->Put(leveldb::WriteOptions(), t.getTxIndex(),
                     t.getTransactionData());
        assert(transactiondbStatus.ok());
    }

    last_tx_id_ = t.getTxIndex();

    delete transactiondb;
}

LatestInTxDB TransactionDBAccess::getLatestInTxDB() const {
    LatestInTxDB latestData;

    leveldb::DB *transactiondb;
    leveldb::Options transactiondbOptions;
    transactiondbOptions.create_if_missing = false;

    leveldb::Status transactiondbStatus = leveldb::DB::Open(
                transactiondbOptions, transaction_database_file_path_,
                &transactiondb);

    if (!transactiondbStatus.ok()) std::cerr << transactiondbStatus.ToString() << std::endl;
    assert(transactiondbStatus.ok());

    leveldb::Iterator *it = transactiondb->NewIterator(leveldb::ReadOptions());
    it->SeekToLast();
    latestData.latest_key = it->key().ToString();
    latestData.latest_value = it->value().ToString();
    assert(it->status().ok());
    delete it;
    delete transactiondb;

    return latestData;
}

std::string TransactionDBAccess::getLastTxID() const {
    return last_tx_id_;
}

std::vector<Transaction> TransactionDBAccess::getTxVectorOfTxDBRange(
        std::string start, std::string limit) const {
    std::vector<Transaction> tx_vector;


    leveldb::DB *transactiondb;
    leveldb::Options transactiondbOptions;
    transactiondbOptions.create_if_missing = false;

    leveldb::Status transactiondbStatus = leveldb::DB::Open(
                transactiondbOptions, transaction_database_file_path_,
                &transactiondb);

    if (!transactiondbStatus.ok()) std::cerr << transactiondbStatus.ToString() << std::endl;
    assert(transactiondbStatus.ok());

    leveldb::Iterator *it = transactiondb->NewIterator(leveldb::ReadOptions());
    for (it->Seek(start); it->Valid() && it->key().ToString() < limit;
         it->Next()) {
        Transaction tx = Transaction(it->value().ToString());
        tx_vector.emplace_back(tx);
        assert(it->status().ok());
    }

    // The above iterator does not catch the last key/value stored in the source.
    it->SeekToLast();
    Transaction tx = Transaction(it->value().ToString());
    tx_vector.emplace_back(tx);
    assert(it->status().ok());

    delete it;
    delete transactiondb;

    return tx_vector;
}
