#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

std::string encrypt(const std::string& key, const std::string& plaintext) {
    if (key.size() != 16) {
        std::cerr << "Key must be 16 bytes for AES-128" << std::endl;
        return "";
    }
    unsigned char iv[16];
    if (RAND_bytes(iv, 16) != 1) {
        handleErrors();
    }
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    int len;
    int ciphertext_len;
    std::vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH);
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char*)key.data(), iv)) handleErrors();
    if (1 != EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (unsigned char*)plaintext.data(), plaintext.size())) handleErrors();
    ciphertext_len = len;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len)) handleErrors();
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    std::string result;
    result.append((char*)iv, 16);
    result.append((char*)ciphertext.data(), ciphertext_len);
    return result;
}

std::string decrypt(const std::string& key, const std::string& ciphertext) {
    if (key.size() != 16) {
        std::cerr << "Key must be 16 bytes for AES-128" << std::endl;
        return "";
    }
    if (ciphertext.size() < 16) {
        std::cerr << "Ciphertext too short" << std::endl;
        return "";
    }
    unsigned char iv[16];
    memcpy(iv, ciphertext.data(), 16);
    std::string actual_ciphertext = ciphertext.substr(16);
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    int len;
    int plaintext_len;
    std::vector<unsigned char> plaintext(actual_ciphertext.size());
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (unsigned char*)key.data(), iv)) handleErrors();
    if (1 != EVP_DecryptUpdate(ctx, plaintext.data(), &len, (unsigned char*)actual_ciphertext.data(), actual_ciphertext.size())) handleErrors();
    plaintext_len = len;
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len)) handleErrors();
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return std::string((char*)plaintext.data(), plaintext_len);
}