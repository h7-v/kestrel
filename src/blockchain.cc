#include "src/blockchain.h"
#include <unistd.h>
#include <assert.h>
#include <string>
#include "leveldb/db.h"

// Returns the latest Block object in _vChain.
// TODO(matt): REMOVE WHEN 100% SURE THAT BLOCKDB WORKS!
Block Blockchain::_getLastBlock() const {
    return _vChain.back();
}

Blockchain::Blockchain(const std::string &blockdbFilepath,
                       const std::string &transactiondbFilepath)
    : blockdb_file_path(blockdbFilepath),
      transactiondb_file_path(transactiondbFilepath), _bBufferBlock(1, "") {

    BlockDBAccess *bdba = new BlockDBAccess(blockdb_file_path);
    block_db_access = bdba;
    TransactionDBAccess *txdba = new TransactionDBAccess(transactiondb_file_path);
    transaction_db_access = txdba;
    setChainBlockDBStatusFromAccess();
    setChainTransactionDBStatusFromAccess();

    Block genesis = Block(0, "11337cc528618b86a8e918edadcd5f1c955790469704a1a3a8075799e610bc72");  // creator's name sha256
    _nDifficulty = 5;
    genesis.setTime(1628806560);  // Fixed time for the Genesis block.
    genesis.mineGenesis(4);  // Easier mining conditions for Genesis so that Kestrel starts quickly.
    _vChain.emplace_back(genesis);

    block_db_access->putInBlockDB(genesis);

    bChainObjectExists = true;
}

Blockchain::~Blockchain() {
    delete block_db_access;
    delete transaction_db_access;
}

bool Blockchain::getbChainObjectExists() const {
    return bChainObjectExists;
}

void Blockchain::setChainBlockDBStatusFromAccess() {
    chainBlockDBStatus = block_db_access->getBlockDBStatus();
}

void Blockchain::setChainTransactionDBStatusFromAccess() {
    chainTransactionDBStatus = transaction_db_access->getTransactionDBStatus();
}

std::string Blockchain::getChainBlockDBStatus() const {
    return chainBlockDBStatus;
}

std::string Blockchain::getChainTransactionDBStatus() const {
    return chainTransactionDBStatus;
}

void Blockchain::runBlockchainMining() {
    isMining = true;
}

void Blockchain::stopBlockchainMining() {
    isMining = false;
}

bool Blockchain::getIsMining() const {
    if (isMining) {
        return true;
    } else {
        return false;
    }
}

void Blockchain::createBlock() {
    usleep(500000);  // Wait for 0.5s so that the database has time to close from the previous block.
    block_db_access->checkIfLatestBlockDBIsMined();

    while (!block_db_access->getPreviousDBBlockIsMined()) {
        // usleep(100000);  // Wait for 0.1s.
    }

    if (block_db_access->getPreviousDBBlockIsMined()) {
        // previousBlockIsMined = false;
        Block bNew = _bBufferBlock;
        _bBufferBlock.emptyBlockContents();
        _vTransactions.clear();

//        bNew.setIndex(_vChain.back().getIndex() + 1);
        bNew.setIndex(std::stoi(block_db_access->getLatestInBlockDBforUI().latest_key) + 1);
        bNew.setPrevHash(block_db_access->getLatestBlockDBHash());

        bNew.mineBlock(_nDifficulty);
        bNew.setTime(time(nullptr));  // Time at completion of mining.
        _vChain.emplace_back(bNew);

        block_db_access->putInBlockDB(bNew);
    }
}

Transaction Blockchain::getLastTransaction() const {
    return _vTransactions.back();
}

void Blockchain::executeTransaction(const std::string &from,
                                    const std::string &to,
                                    const float &amount) {
    Transaction t = Transaction(from, to, amount);
    _vTransactions.push_back(t);
}

void Blockchain::transactionsToBlockBuffer() {
    _bBufferBlock.fillBlockData(_vTransactions);
}

bool Blockchain::bufferBlockContainsData() const {
    return _bBufferBlock.tempBlockContainsData();
}

std::string Blockchain::getBlockDBContents() const {
    return block_db_access->getLatestInBlockDBforUI().latest_value;
}

std::string Blockchain::getBlockDBContents(const std::string &key) const {
    return block_db_access->getBlockDBValueByKeyforUI(key);
}

std::string Blockchain::getTempBlockContents() const {  // used for debugging
    return _bBufferBlock.getBlockContents();
}

std::string Blockchain::getBlockDataOnly() const {  // used for debugging
    return _bBufferBlock.getBlockData();
}
