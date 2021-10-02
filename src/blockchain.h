#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <QObject>
#include "src/wallet.h"
#include "src/transaction.h"
#include "src/tempblock.h"
#include "src/dbaccess.h"
#include "src/checkerthread.h"

// This class creates, controls and maintains the Blockchain object used
// locally by Kestrel to interact with the Kestrel Blockchain.
class Blockchain {
  // Strings for storing the chosen database file paths in case we need to use
  // them at all in the blockchain translation unit.
  std::string blockdb_file_path_;
  std::string transactiondb_file_path_;

  // Pointers copied to by the Blockchain constructor when we create our
  // database access objects. We can use these across scopes in the Blockchain
  // translation unit for access and modification to the local Kestrel
  // databases.
  BlockDBAccess *block_db_access_{nullptr};
  TransactionDBAccess *transaction_db_access_{nullptr};

  // Used with getChainBlockDBStatus() and getChainTransactionDBStatus() so
  // that we can return the status to the UI.
  std::string chain_block_db_status_;
  std::string chain_transaction_db_status_;

  // Number of zeros necessary at the start of Block hashes to satisfy Proof of
  // Work on Kestrel.
  uint32_t difficulty_;

  // Temporary block to be filled with data while Kestrel is mining the
  // previous block. This temporary block is then copied to a new block that
  // will be mined and put forward for consensus. The temporary block is
  // cleared immediately after this copy so that it can begin being filled
  // again.
  TempBlock buffer_block_;

  // OLD VECTOR USED FOR STORING BLOCKS. REMOVE WHEN 100% SURE THAT BLOCKDB WORKS!
  std::vector<Block> chain_vector_;  // TODO(matt): Blockdb is working. This should be cleared when 100% sure everything works.

  // REMOVE WHEN 100% SURE THAT TRANSACTIONDB WORKS!
  std::vector<Transaction> transactions_vector_;  // TODO(matt): Migrate from this vector to the leveldb transaction db.

  // Used to track the number of transactions that have happened on the
  // blockchain. This number allows us to properly index transactions in the
  // database.
  int tx_count_ = 0;

  // Vector used for storing basic wallet data locally. Wallet data is updated
  // before any transactions take place, and private keys are NEVER stored.
  // TODO(matt): Make sure that private keys exist in the program for the LEAST
  // AMOUNT OF TIME POSSIBLE. STACK STORAGE FOR AS FEW CPU CYCLES POSSIBLE.
  std::vector<Wallet> wallets_vector_;

  // Returns the latest Block object in chain_vector_.
  // TODO(matt): REMOVE WHEN 100% SURE THAT BLOCKDB WORKS!
  Block getLastBlock() const;

  // Used with the getBChainObjectExists() method to let the UI know that
  // the Blockchain object has been created. This happens at the end of the
  // Blockchain constructor, so we can assume that if no assert() methods
  // force a crash in the creation/opening of leveldb databases, the Blockchain
  // object and associated databases have been opened and are functioning as
  // normal.
  bool bchain_object_exists_ = false;

  // Keeps track of whether or not the Blockchain is currently mining locally.
  // Used by the MinerThread object (multithreading to prevent
  // UI lockup) to determine whether the mining process should be started or
  // stopped.
  bool is_mining_ = false;

 public:
  // Sets database file paths.
  // Creates the _bufferBlock.
  // Creates the block and transaction databases.
  // Creates the Genesis Block at key 0 to be compared with the other earliest
  // block in the chain. The other earliest block's last hash should match the
  // Genesis hash.
  // Sets bchain_object_exists_ to true.
  // TODO(matt): If the earliest block doesn't match the Genesis block, the
  // local chain is corrupt so the databases should be erased and downloaded
  // from the start.
  Blockchain(const std::string &blockdbFilepath,
             const std::string &transactiondbFilepath);

  // Clears the memory for the database access objects allocated on the heap.
  ~Blockchain();

  bool getBChainObjectExists() const;

  // Sets the relevent Blockchain object database status member variable by
  // using the status getter from the dbaccess API.
  void setChainBlockDBStatusFromAccess();
  void setChainTransactionDBStatusFromAccess();

  std::string getChainBlockDBStatus() const;

  std::string getChainTransactionDBStatus() const;

  // Sets is_mining_ to true.
  void runBlockchainMining();

  // Sets is_mining_ to false.
  void stopBlockchainMining();

  // Used by MinerThread class. See is_mining_ member variable above.
  bool getIsMining() const;

  // Used to check for any transactions in the tx database that have not yet
  // been added to a block locally, adds these transactions and then mines
  // the block. TODO(matt): This process needs to be changed when networking
  // is added. Returns 1 if an update was necessary, 0 if not.
  int updateBChainWithLatestTx();

 public:
  // Waits for the previous block to finish mining, copies the contents of the
  // buffer block to a new block, clears the buffer block and transactions
  // buffer, and then mines the new block and adds it to the chain. This method
  // is used by MinerThread.
  void createBlock();

  // Returns the last transaction object in the Transactions vector.
  // TODO(matt): Remove this when transaction dev is finished.
  Transaction getLastTransaction() const;

  // Creates a new transaction object and fills it with parameters. Then adds
  // this transaction object to the Transactions vector.
  // TODO(matt): Remove this when transaction dev is finished.
  void executeTransaction(const std::string &from, const std::string &to,
                          const float &amount);

  // Copies the Transactions vector into the buffer block. Buffer block then
  // calls fillBlockData() in the Block class to convert the vector contents
  // into a string.
  // TODO(matt): Remove this when transaction dev is finished.
  void transactionsToBlockBuffer();

  // Used by MinerThread to check if there are transactions waiting to
  // be mined.
  bool bufferBlockContainsData() const;

  // gets latest block contents by default. Optionally, a key can be passed
  // to return the value stored by that key.
  std::string getBlockDBContents() const;
  std::string getBlockDBContents(const std::string &key) const;

  // used for debugging to the console/text window
  std::string getTempBlockContents() const;
  std::string getBlockDataOnly() const;
  void getLatestTXInVectorAndDB() const;
  void getTxCount() const;
};
