#include "src/tempblock.h"
#include <string>

TempBlock::TempBlock(uint32_t index, std::string data)
    : Block(index, data) {
    nonce_ = 0;
    hash_ = "";
}

bool TempBlock::tempBlockContainsData() const {
    if (data_ == "") {
        return false;
    } else {
        return true;
    }
}
