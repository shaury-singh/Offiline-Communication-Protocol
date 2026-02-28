#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/err.h>
#include <iostream>
#include <vector>
#include <cstring>

void handleErrors() {
    ERR_print_errors_fp(stderr);
    abort();
}

int encrypt(unsigned char *plaintext, int plaintext_len,unsigned char *key, unsigned char *iv,unsigned char *ciphertext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    int len;
    int ciphertext_len;
    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv)) handleErrors();
    if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) handleErrors();
    ciphertext_len = len;
    if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}

int decrypt(unsigned char *ciphertext, int ciphertext_len,unsigned char *key, unsigned char *iv,unsigned char *plaintext) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) handleErrors();
    int len;
    int plaintext_len;
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key, iv))
        handleErrors();
    if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
        handleErrors();
    plaintext_len = len;
    if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
        handleErrors();
    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);
    return plaintext_len;
}

int main() {
    unsigned char key[16] = "123456789012345";  // 16 bytes = 128-bit
    unsigned char iv[16]  = "abcdefghijklmno";  // 16 bytes IV
    unsigned char plaintext[] = "Hello AES-128 CBC!";
    unsigned char ciphertext[128];
    unsigned char decryptedtext[128];
    int ciphertext_len = encrypt(plaintext, strlen((char*)plaintext),key, iv, ciphertext);
    int decryptedtext_len = decrypt(ciphertext, ciphertext_len,key, iv, decryptedtext);
    decryptedtext[decryptedtext_len] = '\0';
    std::cout << "Original: " << plaintext << std::endl;
    std::cout << "Encrypted: " << ciphertext << std::endl;
    std::cout << "Decrypted: " << decryptedtext << std::endl;
    return 0;
}