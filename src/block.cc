#include <string>
#include <ios>
#include <iomanip>
#include <sstream>
#include "src/block.h"
#include "ext/sha256/sha256.h"
#include "src/kestrel.h"
#include "ui_kestrel.h"

inline std::string Block::calculateHash() const {
    std::stringstream ss;
    ss << index_ << nonce_ << data_<< prev_hash_;

    return sha256(ss.str());
}

inline std::string Block::calculateGenesis() const {
    std::stringstream ss;
    ss << index_ << data_ << nonce_ << time_;

    return sha256(ss.str());
}

Block::Block(uint32_t nIndexIn, const std::string &sDataIn)
    : index_(padBlockIndexWithZeros(nIndexIn)), data_(sDataIn) {
    nonce_ = -1;
    prev_hash_ = "";
    hash_ = "";
    time_ = time(nullptr);  // overwritten when tempblock is copied to new block
}

std::string Block::getIndex() const {
    return index_;
}

void Block::setIndex(uint32_t index) {
    index_ = padBlockIndexWithZeros(index);
}

std::string Block::padBlockIndexWithZeros(uint32_t index) const {
    std::stringstream ss;
    ss << std::internal << std::setfill('0') << std::setw(8) << index;
    return ss.str();
}

void Block::setPrevHash(std::string lastBlockHash) {
    prev_hash_ = lastBlockHash;
}

std::string Block::getHash() {
    return hash_;
}

void Block::setTime(time_t time) {
    time_ = time;
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
        nonce_++;
        hash_ = calculateHash();
    } while (hash_.substr(0, nDifficulty) != difficultyComparison);
}

void Block::mineBlock(uint32_t nDifficulty) {
    prepareDifficultyComparison(nDifficulty);
    satisfyProofOfWork(nDifficulty);

    if (hash_.substr(0, nDifficulty) == difficultyComparison) {
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
        nonce_++;
        hash_ = calculateGenesis();
    } while (hash_.substr(0, nDifficulty) != str);

    if (hash_.substr(0, nDifficulty) == str) {
        isMined = '1';
    }

    std::cout << "Block mined: " << hash_ << "\n";
}

void Block::fillBlockData(const std::vector<Transaction> &vDataIn) {
    std::stringstream ss;
    for (uint i = 0; i < vDataIn.size(); i++) {
        ss << vDataIn[i].getTransactionData();
    }
    data_ = ss.str();
}

void Block::emptyBlockContents() {
    index_ = 1;
    nonce_ = 0;
    data_ = "";
    hash_ = "";
    time_ = 0;
}

std::string Block::getBlockContents() const {
    char buf[25];
    ctime_r(&time_, buf);  // ctime_r is more multithreading safe. I think?
    std::stringstream ss;
    ss << isMined
       << "Index: " << index_ << "\n"
       << "Nonce: " <<nonce_ << "\n"
       << "Data Contents: " << data_ << "\n"
       << "Previous Block's Hash: " << prev_hash_ << "\n"
       << "HASH: " << hash_ << "\n"
       << "Time: " << buf;

    return ss.str();
}

std::string Block::getBlockData() const {
    return data_;
}
