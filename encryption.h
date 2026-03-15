#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>

std::string encrypt(const std::string& key, const std::string& plaintext);
std::string decrypt(const std::string& key, const std::string& ciphertext);

#endif