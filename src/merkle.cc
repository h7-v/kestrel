#include "merkle.h"
#include "ext/sha256/sha256.h"

Merkle::Merkle() {}

void Merkle::txToHashes(const std::vector<Transaction> &tx_vector) {
    // hashes_.reserve(tx_vector.size());
    for (size_t i = 0; i < tx_vector.size(); i++) {
        hashes_.push_back(sha256(tx_vector[i].getTransactionData()));
    }
}

std::string Merkle::hashesToRoot() {
    std::vector<std::string> nodes;

    // Make the number of leaves even.
    while (hashes_.size() > 1) {
        if (hashes_.size() & 1) {
            hashes_.push_back(hashes_.back());
        }

        // Create nodes by adding leaves in groups of 2.
        size_t i = 0;
        size_t j = 0;
        for (; i < hashes_.size() / 2; i++, j += 2) {
            nodes.push_back(hashes_[j] + hashes_[j + 1]);
        }

        // Hash the nodes.
        hashes_.clear();
        for (size_t i = 0; i < nodes.size(); i++) {
            hashes_.push_back(sha256(nodes[i]));
        }

        nodes.clear();
    }

    // Checks that the vector only contains the root.
    if (hashes_.size() != 1) {
        return "merkle_calculation_error";
    }

    return hashes_[0];
}

std::string calculateMerkleRoot(const std::vector<Transaction> &tx_vector) {
    Merkle merkle = Merkle();
    merkle.txToHashes(tx_vector);
    return merkle.hashesToRoot();
}
