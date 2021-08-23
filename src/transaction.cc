#include "src/transaction.h"
#include <string>
#include <sstream>

Transaction::Transaction(std::string fromHash, std::string toHash, float amount)
    : _sFromWallet(fromHash), _sToWallet(toHash), _fAmountTransferred(amount) {
    _tTransactionTime = time(nullptr);
}

std::stringstream Transaction::getTransactionData() const {
    std::stringstream ss;
    ss << "--" << _sFromWallet;
    ss << ",," << _sToWallet;
    ss << ",," << _fAmountTransferred;
    ss << ",," << _tTransactionTime << "__";
    return ss;
}
