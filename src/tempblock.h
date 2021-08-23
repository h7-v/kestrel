#ifndef TEMPBLOCK_H
#define TEMPBLOCK_H

#include <iostream>
#include <cstdint>
#include <string>
#include "src/block.h"

// Used as a buffer to store transactions until the last block has been mined
// and added to the chain.
class TempBlock : public Block {
 public:
  // Stripped back Block constructor that uses only the necessary fields for a
  // buffer block.
  TempBlock(uint32_t index, std::string data);

  // Returns 1 if the buffer block contains data, 0 if not.
  bool tempBlockContainsData() const;
};
#endif  // TEMPBLOCK_H
