#include <openssl/evp.h>
#include <openssl/kdf.h>
#include <openssl/rand.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <cstring>
#include <string>

std::string deriveKey(const std::string& masterKey, const std::string& publicID) {
    std::vector<unsigned char> derivedKey(16);
    EVP_PKEY_CTX* pctx = EVP_PKEY_CTX_new_id(EVP_PKEY_HKDF, NULL);
    EVP_PKEY_derive_init(pctx);
    EVP_PKEY_CTX_set_hkdf_md(pctx, EVP_sha256());
    unsigned char salt[] = "protocol_salt";
    EVP_PKEY_CTX_set1_hkdf_salt(pctx, salt, sizeof(salt));
    EVP_PKEY_CTX_set1_hkdf_key(pctx, (unsigned char*)masterKey.data(), masterKey.size());
    EVP_PKEY_CTX_add1_hkdf_info(
        pctx,
        reinterpret_cast<const unsigned char*>(publicID.data()),
        publicID.size()
    );
    size_t keyLen = 16;
    EVP_PKEY_derive(pctx, derivedKey.data(), &keyLen);
    EVP_PKEY_CTX_free(pctx);
    return std::string((char*)derivedKey.data(), derivedKey.size());
}