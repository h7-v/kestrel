#include "src/tempblock.h"
#include <string>

TempBlock::TempBlock(uint32_t index, std::string data)
    : Block(index, data) {
    _nNonce = 0;
    _sHash = "";
}

bool TempBlock::tempBlockContainsData() const {
    if (_sData == "") {
        return false;
    } else {
        return true;
    }
}
