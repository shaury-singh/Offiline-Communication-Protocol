#ifndef scheduling_h
#define scheduling_h

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

vector<vector<string>> string_to_hex(const string& key);
vector<string> xorWords(vector<string> a, vector<string> b);
vector<string> rotWord(vector<string> word);
vector<string> subWord(vector<string> word, unordered_map<string, string>& sbox);
vector<vector<string>> generateRoundKeys(vector<vector<string>>& hexMatrix, unordered_map<string, string>& sbox);

#endif