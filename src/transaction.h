#ifndef TRANSACTION_H
#define TRANSACTION_H

#pragma once

#include <iostream>

class Transaction {
  // Transaction data.
  std::string tx_index_;
  std::string from_wallet_;
  std::string to_wallet_;
  float amount_transferred_;
  time_t time_of_transaction_;

  // When a Blockchain object uses updateBChainWithLatestTx(), this string is
  // used in the merkle root computation to prevent unnecessary encoding and
  // decoding of Transaction objects.
  std::string raw_tx_data_;

 public:
  Transaction(uint32_t index, std::string fromHash, std::string toHash, float amount);

  // An alternative constructor used by updateBChainWithLatestTx() for making
  // transactions with formatted raw string data only.
  Transaction(std::string raw_data);

  std::string getTxIndex() const;

  // Returns the transaction's contents with some formatting. See
  // transaction.cc.
  std::string getTransactionData() const;

  // Pads the start of the input integer with enough zeros to make the number
  // nine digits long. Up to 999,999,999 transactions are possible.
  std::string padTxIndexWithZeros(uint32_t index) const;

  std::string getRawTxData() const;
};
#endif  // TRANSACTION_H
