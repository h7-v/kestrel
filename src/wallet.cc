#include "src/wallet.h"
#include <string>

Wallet::Wallet(const std::string &address, const std::string &key)
    : _sWalletAddress(address), _sPrivateKey(key) {
    setBalance();
}

void Wallet::setBalance() {
    // calculate balance based on previous transactions in the chain
}

float Wallet::getBalance() const {
    return _fBalance;
}
