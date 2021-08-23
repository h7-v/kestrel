#ifndef WALLET_H
#define WALLET_H

#include <iostream>
#include <string>
#include <vector>

class Wallet {
  std::string _sWalletAddress;
  std::string _sPrivateKey;
  float _fBalance;

 public:
  Wallet(const std::string &address, const std::string &key);
  void setBalance();
  float getBalance() const;
};
#endif  // WALLET_H
