#include <string>
#include <ios>
#include <iomanip>
#include <sstream>
#include "src/block.h"
#include "ext/sha256/sha256.h"
#include "src/kestrel.h"
#include "ui_kestrel.h"

inline std::string Block::_calculateHash() const {
    std::stringstream ss;
    ss << _nIndex << _nNonce << _sData << _sPrevHash;

    return sha256(ss.str());
}

inline std::string Block::_calculateGenesis() const {
    std::stringstream ss;
    ss << _nIndex << _sData << _nNonce << _tTime;

    return sha256(ss.str());
}

Block::Block(uint32_t nIndexIn, const std::string &sDataIn)
    : _nIndex(padIndexWithZeros(nIndexIn)), _sData(sDataIn) {
    _nNonce = -1;
    _sPrevHash = "";
    _sHash = "";
    _tTime = time(nullptr);  // overwritten when tempblock is copied to new block
}

std::string Block::getIndex() const {
    return _nIndex;
}

void Block::setIndex(uint32_t index) {
    _nIndex = padIndexWithZeros(index);
}

std::string Block::padIndexWithZeros(uint32_t index) const {
    std::stringstream ss;
    ss << std::internal << std::setfill('0') << std::setw(8) << index;
    return ss.str();
}

void Block::setPrevHash(std::string lastBlockHash) {
    _sPrevHash = lastBlockHash;
}

std::string Block::getHash() {
    return _sHash;
}

void Block::setTime(time_t time) {
    _tTime = time;
}

bool Block::getMinedStatus() const {
    return isMined;
}

void Block::prepareDifficultyComparison(uint32_t nDifficulty) {
    char *cstr = new char[nDifficulty + 1];
    for (uint32_t i = 0; i < nDifficulty; i++) {
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';

    std::string str(cstr);
    difficultyComparison = str;
    delete[] cstr;
}

void Block::satisfyProofOfWork(uint32_t nDifficulty) {
    do {
        _nNonce++;
        _sHash = _calculateHash();
    } while (_sHash.substr(0, nDifficulty) != difficultyComparison);
}

void Block::mineBlock(uint32_t nDifficulty) {
    prepareDifficultyComparison(nDifficulty);
    satisfyProofOfWork(nDifficulty);

    if (_sHash.substr(0, nDifficulty) == difficultyComparison) {
        isMined = '1';
    }
}

void Block::mineGenesis(u_int32_t nDifficulty) {
    char *cstr = new char[nDifficulty + 1];
    for (uint32_t i = 0; i < nDifficulty; i++) {
        cstr[i] = '0';
    }
    cstr[nDifficulty] = '\0';

    std::string str(cstr);
    delete[] cstr;

    do {
        _nNonce++;
        _sHash = _calculateGenesis();
    } while (_sHash.substr(0, nDifficulty) != str);

    if (_sHash.substr(0, nDifficulty) == str) {
        isMined = '1';
    }

    std::cout << "Block mined: " << _sHash << "\n";
}

void Block::fillBlockData(const std::vector<Transaction> &vDataIn) {
    std::stringstream ss;
    for (uint i = 0; i < vDataIn.size(); i++) {
        ss << vDataIn[i].getTransactionData().str();
    }
    _sData = ss.str();
}

void Block::emptyBlockContents() {
    _nIndex = 1;
    _nNonce = 0;
    _sData = "";
    _sHash = "";
    _tTime = 0;
}

std::string Block::getBlockContents() const {
    char buf[25];
    ctime_r(&_tTime, buf);  // ctime_r is more multithreading safe. I think?
    std::stringstream ss;
    ss << isMined
       << "Index: " << _nIndex << "\n"
       << "Nonce: " <<_nNonce << "\n"
       << "Data Contents: " << _sData << "\n"
       << "Previous Block's Hash: " << _sPrevHash << "\n"
       << "HASH: " << _sHash << "\n"
       << "Time: " << buf;

    return ss.str();
}

std::string Block::getBlockData() const {
    return _sData;
}
