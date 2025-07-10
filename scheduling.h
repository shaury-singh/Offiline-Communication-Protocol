#ifndef scheduling_h
#define scheduling_h

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

vector<vector<string>> string_to_hex_(const string& key);
unordered_map<string, string> get_sbox();
vector<string> xorWords_(vector<string> a, vector<string> b);
vector<string> rotWord_(vector<string> word);
vector<string> subWord_(vector<string> word, unordered_map<string, string>& sbox);
vector<vector<string>> generateRoundKeys_(vector<vector<string>>& hexMatrix, unordered_map<string, string>& sbox);

#endif