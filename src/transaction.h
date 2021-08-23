#ifndef TRANSACTION_H
#define TRANSACTION_H

#pragma once

#include <iostream>

class Transaction {
  std::string _sFromWallet;
  std::string _sToWallet;
  float _fAmountTransferred;
  time_t _tTransactionTime;

 public:
  Transaction(std::string fromHash, std::string toHash, float amount);

  std::stringstream getTransactionData() const;

};
#endif  // TRANSACTION_H
