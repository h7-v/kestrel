#include "src/transaction.h"
#include <string>
#include <sstream>

Transaction::Transaction(std::string fromHash, std::string toHash, float amount)
    : from_wallet_(fromHash), to_wallet_(toHash), amount_transferred_(amount) {
    time_of_transaction_ = time(nullptr);
}

std::stringstream Transaction::getTransactionData() const {
    std::stringstream ss;
    ss << "--" << from_wallet_;
    ss << ",," << to_wallet_;
    ss << ",," << amount_transferred_;
    ss << ",," << time_of_transaction_ << "__";
    return ss;
}
