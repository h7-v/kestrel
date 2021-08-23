#ifndef TRANSACTION_H
#define TRANSACTION_H

#pragma once

#include <iostream>

class Transaction {
  std::string from_wallet_;
  std::string to_wallet_;
  float amount_transferred_;
  time_t time_of_transaction_;

 public:
  Transaction(std::string fromHash, std::string toHash, float amount);

  std::stringstream getTransactionData() const;

};
#endif  // TRANSACTION_H
