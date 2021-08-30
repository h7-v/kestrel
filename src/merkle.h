#ifndef MERKLE_H
#define MERKLE_H

#include <iostream>
#include <string>
#include <vector>
#include "src/transaction.h"

// Used for calculating the Merkle Root of a set of transactions for a block.
// The block range is the list of transactions used in the tree.
// 1. Call calculateMerkleRoot().
// 2. Transactions are hashed with txToHashes().
// 3. A tree is formed and the root is returned with hashesToRoot().
class Merkle
{
 private:
  // Used in calculateMerkleRoot() to hold hashes.
  std::vector<std::string> hashes_;

 public:
  Merkle();

  // Passes each transaction through SHA256.
  void txToHashes(const std::vector<Transaction> &tx_vector);

  // Creates the Merkle tree and returns the root.
  std::string hashesToRoot();
};

// Public access function for calculating the Merkle root of a set of
// transactions using the Merkle class.
std::string calculateMerkleRoot(const std::vector<Transaction> &tx_vector);

#endif // MERKLE_H
