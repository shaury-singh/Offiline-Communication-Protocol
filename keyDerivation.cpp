#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/rand.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstring>

std::vector<unsigned char> deriveKey(const std::vector<unsigned char>& masterKey,const std::string& publicID) {
    std::vector<unsigned char> derivedKey(16); // 128-bit AES key
    EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
    EVP_PKEY_derive_init(pctx);
    EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256());
    // Salt (can also be random or protocol constant)
    unsigned char salt[] = "protocol_salt";
    EVP_PKEY_CTX_set1_hkdf_salt(pctx, salt, sizeof(salt));
    EVP_PKEY_CTX_set1_hkdf_key(pctx, masterKey.data(), masterKey.size());
    EVP_PKEY_CTX_add1_hkdf_info(
        pctx,
        reinterpret_cast<const unsigned char*>(publicID.data()),
        publicID.size()
    );
    size_t keyLen = 16;
    EVP_PKEY_derive(pctx, derivedKey.data(), &keyLen);
    EVP_PKEY_CTX_free(pctx);
    return derivedKey;
}

int main() {
    std::vector<unsigned char> masterKey(32);
    RAND_bytes(masterKey.data(), masterKey.size());
    std::string publicID = "user_123";
    auto derivedKey = deriveKey(masterKey, publicID);
    std::cout << "Derived AES-128 Key:\n";
    for (unsigned char byte : derivedKey) {
        std::cout << std::hex << std::setw(2) << std::setfill('0')<< (int)byte;
    }
    std::cout << std::endl;
    return 0;
}