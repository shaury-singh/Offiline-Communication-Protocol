#ifndef KEYDERIVATION_H
#define KEYDERIVATION_H

#include <string>

std::string deriveKey(const std::string& masterKey, const std::string& publicID);

#endif