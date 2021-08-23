#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include "src/transaction.h"

class Block {
 protected:
  // Block data used in mining.
  std::string index_;  // Was uint32_t before padding was added.
  int64_t nonce_;
  std::string data_;
  std::string prev_hash_;

  // Block hash variable set after mining is completed.
  std::string hash_;

  // Used to mark the block with a timestamp on completion of mining.
  // The Genesis block uses this variable in the mining process as well as
  // using it as the associated timestamp. This is contrary to every other
  // block on the chain.
  time_t time_;

 private:
  // Using a string stream, converts the stored data for the block to a string
  // and then returns the SHA256 hash of that data. Used by
  // satisfyProofOfWork().
  std::string calculateHash() const;

  // Does the same as calculateHash(), but adds the set time for Genesis, and
  // ignores the previous hash as the Genesis block is the first block in the
  // chain. Used by mineGenesis().
  std::string calculateGenesis() const;

  // Used as an internal means to keep track of whether or not a block has been
  // mined.
  char isMined = '0';

  // Used by satisfyProofOfWork() and mineBlock() to check if the first x
  // digits of the block hash are zeros, where x is the difficulty set by the
  // blockchain.
  std::string difficultyComparison;

 public: 
  // When a new block is created, Blockchain sets the index and passes the TX
  // data. The remaining variables are also initialised with default values.
  Block(uint32_t nIndexIn, const std::string &sDataIn);
    
  std::string getIndex() const;

  // Takes a number, pads it with zeros using padIndexWithZeros(), then sets
  // index_.
  void setIndex(uint32_t index);

  // Pads the start of the input integer with enough zeros to make the number
  // eight digits long.
  std::string padIndexWithZeros(uint32_t index) const;

  // Set by Blockchain when Blockchain::createBlock() is called.
  void setPrevHash(std::string lastBlockHash);

  std::string getHash();

  // Used by Blockchain to mark blocks with the time at which they finished
  // being mined.
  void setTime(time_t time);

  bool getMinedStatus() const;

  // Converts an input integer into a string of zeros, the length of which
  // is the input int, and then adds a null termination character.
  // E.g. 6 is converted into "000000\0".
  void prepareDifficultyComparison(uint32_t nDifficulty);

  // Increases the block nonce by 1 and runs calculateHash() repeatedly until
  // the result hash is padded with enough zeros to match the Blockchain
  // difficulty.
  void satisfyProofOfWork(uint32_t nDifficulty);

  // Uses prepareDifficultyComparison() and satisfyProofOfWork() to mark when
  // the block has been mined.
  void mineBlock(uint32_t nDifficulty);

  // Combines the block mining functions into a one off function for the
  // Genesis block as the difficulty is lower for Genesis. This speeds up
  // creation of the Blockchain object when Kestrel runs.
  void mineGenesis(u_int32_t nDifficulty);

  // Takes a vector of transactions, converts them into processable
  // string packets, and then streams the packets to data_.
  void fillBlockData(const std::vector<Transaction> &sDataIn);

  // Resets the block data to 0 and empty values.
  // Useful for emptying the bufferBlock.
  void emptyBlockContents();

  // Gets the ENTIRE block contents and returns as a string.
  std::string getBlockContents() const;

  // Gets ONLY data_.
  std::string getBlockData() const;
};
