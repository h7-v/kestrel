#include "src/transaction.h"
#include <string>
#include <sstream>
#include <iomanip>

Transaction::Transaction(uint32_t index, std::string fromHash, std::string toHash, float amount)
    : from_wallet_(fromHash), to_wallet_(toHash), amount_transferred_(amount) {
    tx_index_ = padTxIndexWithZeros(index);
    time_of_transaction_ = time(nullptr);
}

Transaction::Transaction(std::string raw_data)
    : raw_tx_data_(raw_data) {
    for (int i = 2; i < 11; i++) {
        tx_index_ += raw_data[i];
    }
}

std::string Transaction::getTxIndex() const {
    return tx_index_;
}

std::string Transaction::getTransactionData() const {
    std::stringstream ss;
    ss << "--" << tx_index_
       << ",," << from_wallet_
       << ",," << to_wallet_
       << ",," << amount_transferred_
       << ",," << time_of_transaction_ << "__";
    return ss.str();
}

std::string Transaction::padTxIndexWithZeros(uint32_t index) const {
    std::stringstream ss;
    ss << std::internal << std::setfill('0') << std::setw(9) << index;
    return ss.str();
}

std::string Transaction::getRawTxData() const {
    return raw_tx_data_;
}
