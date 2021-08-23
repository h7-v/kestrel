#include "src/dbaccess.h"

BlockDBAccess::BlockDBAccess(const std::string &dbpath)
    : block_database_file_path(dbpath) {
    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = true;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path, &blockdb);

    assert(blockdbStatus.ok());

    if (!blockdbStatus.ok()) {
        std::cerr << blockdbStatus.ToString() << std::endl;
    } else if (blockdbStatus.ok()) {
        blockdb_status = (blockdbStatus.ToString());
    }

    delete blockdb;
}

BlockDBAccess::~BlockDBAccess() {}

std::string BlockDBAccess::getBlockDBStatus() const {
    return blockdb_status;
}

LatestInBlockDB BlockDBAccess::getLatestInBlockDBforUI() const {
    LatestInBlockDB latestData;

    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path, &blockdb);

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
                blockdbOptions, block_database_file_path, &blockdb);

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

void BlockDBAccess::onPreviousBlockInMinedState() {
    previousDBBlockIsMined = true;
}

void BlockDBAccess::checkIfLatestBlockDBIsMined() {
    cThread = new CheckerThread();
    cThread->setFilePath(block_database_file_path);

    connect(cThread, &CheckerThread::previousBlockInMinedState,
            this, &BlockDBAccess::onPreviousBlockInMinedState);


    cThread->start();
    cThread->wait();
    delete cThread;
}

bool BlockDBAccess::getPreviousDBBlockIsMined() const {
    return previousDBBlockIsMined;
}

void BlockDBAccess::putInBlockDB(const Block &b) const {
    leveldb::DB *blockdb;
    leveldb::Options blockdbOptions;
    blockdbOptions.create_if_missing = false;

    leveldb::Status blockdbStatus = leveldb::DB::Open(
                blockdbOptions, block_database_file_path, &blockdb);

    assert(blockdbStatus.ok());

    if (!blockdbStatus.ok()) {
        std::cerr << blockdbStatus.ToString() << std::endl;
    } else if (blockdbStatus.ok()) {
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
                blockdbOptions, block_database_file_path, &blockdb);

    assert(blockdbStatus.ok());

    if (!blockdbStatus.ok()) {
        std::cerr << blockdbStatus.ToString() << std::endl;
    } else if (blockdbStatus.ok()) {
        blockdb->Delete(leveldb::WriteOptions(), key);
        assert(blockdbStatus.ok());
    }

    delete blockdb;
}

// ----------------------------------------------------------------------------

TransactionDBAccess::TransactionDBAccess(const std::string &dbpath)
    : transaction_database_file_path(dbpath) {
    leveldb::DB *transactiondb;
    leveldb::Options transactiondbOptions;
    transactiondbOptions.create_if_missing = true;

    leveldb::Status transactiondbStatus = leveldb::DB::Open(
                transactiondbOptions, transaction_database_file_path,
                &transactiondb);

    assert(transactiondbStatus.ok());

    if (!transactiondbStatus.ok()) {
        std::cerr << transactiondbStatus.ToString() << std::endl;
    } else if (transactiondbStatus.ok()) {
        transactiondb_status = (transactiondbStatus.ToString());
    }

    delete transactiondb;
}

TransactionDBAccess::~TransactionDBAccess() {}

std::string TransactionDBAccess::getTransactionDBStatus() const {
    return transactiondb_status;
}
