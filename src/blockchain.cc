#include "src/blockchain.h"
#include <unistd.h>
#include <assert.h>
#include <string>
#include "leveldb/db.h"

// Returns the latest Block object in chain_vector_.
// TODO(matt): REMOVE WHEN 100% SURE THAT BLOCKDB WORKS!
Block Blockchain::getLastBlock() const {
    return chain_vector_.back();
}

Blockchain::Blockchain(const std::string &blockdbFilepath,
                       const std::string &transactiondbFilepath)
    : blockdb_file_path_(blockdbFilepath),
      transactiondb_file_path_(transactiondbFilepath), buffer_block_(1, "") {

    // Init databases.
    BlockDBAccess *bdba = new BlockDBAccess(blockdb_file_path_);
    block_db_access_ = bdba;
    TransactionDBAccess *txdba = new TransactionDBAccess(transactiondb_file_path_);
    transaction_db_access_ = txdba;
    setChainBlockDBStatusFromAccess();
    setChainTransactionDBStatusFromAccess();

    // Create Genesis Block at Index 0.
    Block genesis = Block(0, "11337cc528618b86a8e918edadcd5f1c955790469704a1a3a8075799e610bc72");  // creator's name sha256
    difficulty_ = 5;
    genesis.setTime(1628806560);  // Fixed time for the Genesis block.
    genesis.mineGenesis(4);  // Easier mining conditions for Genesis so that Kestrel starts quickly.
    chain_vector_.emplace_back(genesis);

    block_db_access_->putInBlockDB(genesis);

    // Set tx count for this Blockchain object to match the latest index in the tx db.
    // There needs to be at least one transaction in the database already.
    Transaction t = Transaction(0, "dev", "chain", 1);
    transaction_db_access_->putInTxDB(t);
    tx_count_ = std::stoi(transaction_db_access_->getLatestInTxDB().latest_key);

    bchain_object_exists_ = true;
}

Blockchain::~Blockchain() {
    delete block_db_access_;
    delete transaction_db_access_;
}

bool Blockchain::getBChainObjectExists() const {
    return bchain_object_exists_;
}

void Blockchain::setChainBlockDBStatusFromAccess() {
    chain_block_db_status_ = block_db_access_->getBlockDBStatus();
}

void Blockchain::setChainTransactionDBStatusFromAccess() {
    chain_transaction_db_status_ = transaction_db_access_->getTxDBStatus();
}

std::string Blockchain::getChainBlockDBStatus() const {
    return chain_block_db_status_;
}

std::string Blockchain::getChainTransactionDBStatus() const {
    return chain_transaction_db_status_;
}

void Blockchain::runBlockchainMining() {
    is_mining_ = true;
}

void Blockchain::stopBlockchainMining() {
    is_mining_ = false;
}

bool Blockchain::getIsMining() const {
    if (is_mining_) {
        return true;
    } else {
        return false;
    }
}

void Blockchain::createBlock() {
    usleep(500000);  // Wait for 0.5s so that the database has time to close from the previous block.
    block_db_access_->checkIfLatestBlockDBIsMined();

    while (!block_db_access_->getPreviousDBBlockIsMined()) {
        // usleep(100000);  // Wait for 0.1s.
    }

    if (block_db_access_->getPreviousDBBlockIsMined()) {
        Block bNew = buffer_block_;
        buffer_block_.emptyBlockContents();
        transactions_vector_.clear();

//        bNew.setIndex(chain_vector_.back().getIndex() + 1);
        bNew.setIndex(std::stoi(block_db_access_->
                                getLatestInBlockDBforUI().latest_key) + 1);
        bNew.setPrevHash(block_db_access_->getLatestBlockDBHash());

        bNew.mineBlock(difficulty_);
        bNew.setTime(time(nullptr));  // Time at completion of mining.
        chain_vector_.emplace_back(bNew);

        block_db_access_->putInBlockDB(bNew);
    }
}

Transaction Blockchain::getLastTransaction() const {
    return transactions_vector_.back();
}

void Blockchain::executeTransaction(const std::string &from,
                                    const std::string &to,
                                    const float &amount) {
    tx_count_++;
    Transaction t = Transaction(tx_count_, from, to, amount);
    transactions_vector_.push_back(t);
    transaction_db_access_->putInTxDB(t);
}

void Blockchain::transactionsToBlockBuffer() {
    buffer_block_.fillBlockData(transactions_vector_);
}

bool Blockchain::bufferBlockContainsData() const {
    return buffer_block_.tempBlockContainsData();
}

std::string Blockchain::getBlockDBContents() const {
    return block_db_access_->getLatestInBlockDBforUI().latest_value;
}

std::string Blockchain::getBlockDBContents(const std::string &key) const {
    return block_db_access_->getBlockDBValueByKeyforUI(key);
}

std::string Blockchain::getTempBlockContents() const {  // used for debugging
    return buffer_block_.getBlockContents();
}

std::string Blockchain::getBlockDataOnly() const {  // used for debugging
    return buffer_block_.getBlockData();
}

void Blockchain::getLatestTXInVectorAndDB() const {  // used for debugging
    std::cout << "VectorTX:" << transactions_vector_.end()->getTransactionData() << std::endl;
    std::cout << "TX DB: " << transaction_db_access_->getLatestInTxDB().latest_value << std::endl;
}

void Blockchain::getTxCount() const {  // used for debugging
    std::cout << tx_count_ << std::endl;
}
