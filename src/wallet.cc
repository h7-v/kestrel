#include "src/wallet.h"
#include "openssl/rand.h"
#include "openssl/err.h"
#include "openssl/sha.h"  // Necessary over the SHA256 lib in ext/ as we are processing bytes here.
#include "openssl/ripemd.h"
#include "ext/secp256k1/include/secp256k1.h"
#include <stdint.h>
#include <unistd.h>
#include <string>
#include <iomanip>

// Credit to https://nickfarrow.com/Cryptography-in-Bitcoin-with-C/ for a
// fantastic resource.

// unsigned char is used over byte when we want to match the documentation
// of a library for readability.
typedef unsigned char byte;

Wallet::Wallet() {
    setBalance();
}

std::string Wallet::getPublicKey() const {
    return public_key_;
}

std::string Wallet::getWalletAddress() const {
    return wallet_address_;
}

void Wallet::setBalance() {
    // TODO(matt): calculate balance based on previous transactions in the chain
}

float Wallet::getBalance() const {
    return balance_;
}

int Wallet::getRandomSeed() const {
    byte buffer[4];
    int rstatus = RAND_bytes(buffer, sizeof(buffer));  // Cryptographically secure randomising of bytes in buffer.

    unsigned long err = ERR_get_error();  // Holds error code if RAND_bytes fails.
    if (rstatus != 1) {  // rstatus holds a 1 or a 0. 1 indicates RAND_bytes was successful.
        std::cout << "RAND_bytes failed. Error: " << err << std::endl;
        return 0;
    }

    // Convert char array bytes to a single int.
    unsigned int seed = int((byte)(buffer[0]) << 24 |
                   (byte)(buffer[1]) << 16 |
                   (byte)(buffer[2]) << 8 |
                   (byte)(buffer[3]));

    return seed;
}

std::string Wallet::generatePrivateKey() {
    std::string s;
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ";  // Caps only so private keys are easy to type in for Tx's.

    int rand_seed;

    s.reserve(32);

    for (int i = 0; i < 32; ++i) {
        rand_seed = getRandomSeed();
        if (rand_seed == 0) {
            std::cout << "Key generation failed. Seed returned zero." << std::endl;
            return "ERROR: Key generation failed.";
        }
        srand(rand_seed);
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return s;
}

std::string Wallet::computePublicKey(const std::string &private_key) const {
    // Creating the context for creation and serialisation.
    static secp256k1_context *ctx = NULL;
    ctx = secp256k1_context_create(
                SECP256K1_CONTEXT_SIGN | SECP256K1_CONTEXT_VERIFY);  // Init flags.

    // Copying the input string to an array for processing.
    unsigned char *seckey = (unsigned char*) malloc(private_key.length() *
                                                    sizeof(unsigned char));
    for (size_t i = 0; i < private_key.length(); i++) {
        seckey[i] = private_key[i];
    }

    // Creating the public key.
    secp256k1_pubkey pubkey;
    if (!secp256k1_ec_pubkey_create(ctx, &pubkey, seckey)) {  // Context object, output address, input address.
        throw "Public key creation error.";
    }
    // We are finished with seckey. There is potential for a memory leak here
    // if conditions on above lines are not met and seckey is not freed.
    free(seckey);

    // Memory allocation for seralising the pubkey object into a serialised
    // byte sequence.
    size_t size = 65;  // secp256k1_ec_pubkey_serialize() requires 65 bytes.
    unsigned char *bytes = (unsigned char*) malloc(65 * sizeof(unsigned char));

    secp256k1_ec_pubkey_serialize(
        ctx,
        bytes,
        &size,
        &pubkey,
        SECP256K1_EC_UNCOMPRESSED
        );
    // Context object, output address, output length, input address, flags.

    // Convert the pubkey bytes to a readable public key.
    byte s[64];
    for (int i = 0; i < 64; i++) {
        s[i] = bytes[i];
    }
    char out[64];
    base58(s, 64, out, 64);

    std::string public_key;
    for (int i = 0; i < 64; i++) {
        public_key += out[i];
    }

    return public_key;
}

// Base 58 conversion written by nickfarrow.com(?)
char* Wallet::base58(unsigned char *s, int s_size, char *out, int out_size) const {
        static const char *base_chars = "123456789"
                "ABCDEFGHJKLMNPQRSTUVWXYZ"
                "abcdefghijkmnopqrstuvwxyz";

        byte s_cp[s_size];
        memcpy(s_cp, s, s_size);

        int c, i, n;

        out[n = out_size] = 0;
        while (n--) {
                for (c = i = 0; i < s_size; i++) {
                        c = c * 256 + s_cp[i];
                        s_cp[i] = c / 58;
                        c %= 58;
                }
                out[n] = base_chars[c];
        }

        return out;
}

std::string Wallet::walletAddressFromHash(const std::string &pub_key_in) const {
    byte s[65];
    byte out_hash[5 + RIPEMD160_DIGEST_LENGTH];

    for (int i = 0; i < 65; i++) {
        s[i] = pub_key_in[i];
    }

    out_hash[0] = 0;  // Version byte.
    RIPEMD160(SHA256(s, 65, 0), SHA256_DIGEST_LENGTH, out_hash + 1);  // MD hash the SHA hash of the key bytes.

    // Creating the checksum and copying the last 4 bytes to out_hash.
    memcpy(out_hash + 21, SHA256(SHA256(out_hash, 21, 0), SHA256_DIGEST_LENGTH, 0), 4);

    // Base58 conversion and leading ones removal.
    char address[34];
    base58(out_hash, 25, address, 34);

    // Count the number of extra 1s at the beginning of the address.
    int ones;
    for (ones = 1; address[ones] == '1'; ones++);

    // Count the number of extra leading 0x00 bytes.
    int n;
    for (n = 1; out_hash[n] == 0x00; n++);

    // Remove ones-n leading 1's from the address.
    memmove(address, address + (ones-n), 34-(ones-n));
    address[34-(ones-n)] = '\0';

    // Convert to string and add the "addr" prefix to indicate an address.
    std::string result = "addr";
    for (size_t i = 0; i < sizeof(address); i++) {
        result += address[i];
    }

    // Our result can sometimes be one character too short, but never more than
    // one character. If the result is too short, we simply copy the last
    // legitimate character onto the end so that the expected length is met.
    if (result[37] == '\0') {
        result[37] = result[36];
    }

    return result;
}

int Wallet::computePkeyAndWalletAddress(const std::string &private_key) {
    public_key_ = computePublicKey(private_key);
    wallet_address_ = walletAddressFromHash(public_key_);
    if (public_key_.length() != 64 || wallet_address_.length() != 38) {
        public_key_ = "";
        wallet_address_ = "";
        return 0;
    }
    return 1;
}

std::string Wallet::getWalletAddrFromPrivateKey(const std::string &private_key) const {
    std::string pkey = computePublicKey(private_key);
    std::string addr = walletAddressFromHash(pkey);
    if (pkey.length() != 64 || addr.length() != 38) {
        return "ERROR: Key or address incorrect length after process.";
    }

    return walletAddressFromHash(computePublicKey(private_key));
}
