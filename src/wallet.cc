#include "src/wallet.h"
#include <string>

Wallet::Wallet(const std::string &address, const std::string &key)
    : wallet_address_(address), private_key_(key) {
    setBalance();
}

void Wallet::setBalance() {
    // calculate balance based on previous transactions in the chain
}

float Wallet::getBalance() const {
    return balance_;
}
