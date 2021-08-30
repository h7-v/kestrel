#ifndef DBACCESS_H
#define DBACCESS_H

#pragma once

// **Only works with Kestrel Block and Transaction types**
//
// API for dealing with leveldb database access to cut down on repeated code.
// Unfortunately, there is a lot of repeated code in this API, but there is
// no easy way to streamline database access without resorting to undefined
// behaviour. Each method available has to open and close the database each
// time it is called.
//
// dbaccess is used for handling block and transaction storage within
// blockchains.
//
// leveldb exclusively uses strings for keys and values.
//
// Also used for batch processing of the database? Let's try

#include <leveldb/db.h>
#include <string>
#include "src/block.h"
#include "src/checkerthread.h"
#include "src/transaction.h"

// Struct with a member variable pair for returning two values from
// the getLatestInBlockDB() method present in the BlockDBAccess class below.
struct LatestInBlockDB {
 public:
  std::string latest_key;
  std::string latest_value;
};

// This class handles leveldb database access and modification for Kestrel
// blocks. Code becomes messy in other translation units without this class
// because of the number of lines required for database access.
class BlockDBAccess : public QObject {
 private:
  // Used in place of filling in the database path on leveldb::DB::Open
  // each time.
  std::string block_database_file_path_;

  // Translation units can get access to leveldb::Status variables with this.
  // Necessary if we wish to print the status to the UI.
  std::string blockdb_status_;

  bool previousDBBlockIsMined = false;

 public:
  // Takes a filepath for where we wish to create the database. This path
  // defines block_database_file_path_. The database is created and then error
  // checked. All methods in this class will not create the database when
  // called - only the constructor.
  explicit BlockDBAccess(const std::string &dbpath);

  // No clean up is necessary as we delete the database pointer at the end of
  // each member function.
  ~BlockDBAccess();

  std::string getBlockDBStatus() const;

  // Returns an object containing two strings for the key and value of the
  // latest database entry. Does not apply to the repeated addition of the
  // Genesis Block.
  LatestInBlockDB getLatestInBlockDBforUI() const;

  // Returns the value stored at the input key.
  std::string getBlockDBValueByKeyforUI(const std::string &key) const;

  // The two functions above are used for printing to the UI, but we may need
  // equivalent functions that don't erase the isMined char off the front end
  // of the value.
  // TODO(matt): See above.

  // Finds "HASH: " within the latest database entry's value and returns the
  // 64 character SHA-256 hash that follows.
  std::string getLatestBlockDBHash() const;

  CheckerThread *cthread_{nullptr};

 public slots:
  // Slot matching the SIGNAL previousBlockInMinedState() from CheckerThread.
  // Sets previousDBBlockIsMined to true.
  void onPreviousBlockInMinedState();

 public:
  // Checks if the latest block by index is marked with a 1 at the beginning of
  // it's value.
  void checkIfLatestBlockDBIsMined();

  bool getPreviousDBBlockIsMined() const;

  // Easy access to the Put() method in leveldb for use with Kestrel's block
  // type. Used for adding blocks to the chain when block consensus is
  // successful.
  void putInBlockDB(const Block &b) const;

  // USED EXCLUSIVELY FOR DEBUGGING. We must never modify the chain other than
  // when adding trusted blocks.
  void deleteFromBlockDB(const std::string &key) const;
};

// ----------------------------------------------------------------------------

// Struct with a member variable pair for returning two values from
// the getLatestInTxDB() method present in the TransactionDBAccess class below.
struct LatestInTxDB {
 public:
  std::string latest_key;
  std::string latest_value;
};

// This class works similarly to the BlockDBAccess class above, but is for use
// with Kestrel transactions.
class TransactionDBAccess {
 private:
  // Used in place of filling in the database path on leveldb::DB::Open
  // each time.
  std::string transaction_database_file_path_;

  // Translation units can get access to leveldb::Status variables with this.
  // Necessary if we wish to print the status to the UI.
  std::string txdb_status_;

 public:
  // Works identically to the explicit BlockDBAccess constructor above.
  explicit TransactionDBAccess(const std::string &dbpath);

  // No clean up is necessary as we delete the database pointer at the end of
  // each member function.
  ~TransactionDBAccess();

  std::string getTxDBStatus() const;

  void putInTxDB(const Transaction &t) const;

  LatestInTxDB getLatestInTxDB() const;

  // TODO(matt): Fill this in when developing transactions
};
#endif  // BLOCKDB_H
