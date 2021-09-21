#ifndef WALLET_H
#define WALLET_H

#include <iostream>
#include <string>
#include <vector>

// This class handles everything to do with wallets and will eventually play
// a big part in how the transaction class operates.

// Wallet Address Processing
// To calculate a Kestrel wallet address from a Kestrel private key, the
// following happens:
// - A string input containing the private key is converted to an unsigned char
// array and we are left with a pointer to that array.
//
// - secp256k1 library is then used for the pubkey create function which uses
// elliptic curve crypto in its key generation. It returns 1 if successful,
// and 0 if unsuccessful. The output is at the memory address of pubkey.
//
// - The pubkey serialize function from this library is then used to convert
// the public key stored at pubkey back to a char array at bytes.
//
// - These bytes are hashed by SHA256, then this hash is hashed by MD160. This
// is our new pubkey hash.
//
// - We then create a checksum by hashing twice with SHA256 again and copying
// the last 4 bytes over our pubkey hash.
//
// - This hash is encoded in Base58 so that simiar looking characters are not
// present when the user types in an address.
//
// - Leading 0s are removed, and all 1s but one are removed. The address starts
// with a 1.
class Wallet {
 private:
  std::string public_key_;
  std::string wallet_address_;
  float balance_;

  // Returns a random number using the RAND_bytes function from the
  // openssl crypto library.
  int getRandomSeed() const;

  // Elliptic curve secp256k1 used to encrypt the private key.
  std::string computePublicKey(const std::string &private_Key) const;

  // Converts input byte hash to Base 58.
  char* base58(unsigned char *s, int s_size, char *out, int out_size) const;

  // Uses SHA256 and RIPEMD160 and generates a Base58 address.
  std::string walletAddressFromHash(const std::string &pub_key_in) const;

 public:
  Wallet();
  std::string getPublicKey() const;
  std::string getWalletAddress() const;
  void setBalance();
  float getBalance() const;

  // Uses openssl to generate cryptographically secure random numbers for use
  // in a Kestrel private key.
  std::string generatePrivateKey();

  // Takes a Kestrel private key and sets public_key_ and wallet_address_
  // accordingly. Returns 1 if successful and 0 if not.
  int computePkeyAndWalletAddress(const std::string &private_key);

  std::string getWalletAddrFromPrivateKey(const std::string &private_key) const;
};
#endif  // WALLET_H
