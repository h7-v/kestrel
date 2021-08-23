#ifndef WALLET_H
#define WALLET_H

#include <iostream>
#include <string>
#include <vector>

class Wallet {
  std::string wallet_address_;
  std::string private_key_;
  float balance_;

 public:
  Wallet(const std::string &address, const std::string &key);
  void setBalance();
  float getBalance() const;
};
#endif  // WALLET_H
