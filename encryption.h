#ifndef encryption.h
#define encryption.h

using namespace std;

#include <vector>
#include <string>
#include <unordered_map>

vector<vector<string>> string_to_hex(string s);
vector<vector<int>> hex_to_int(vector<vector<string>>& matrixMapping);
vector<vector<int>> xorCyprt(vector<vector<int>>& keyIntHex, vector<vector<int>>& TextIntHex);
vector<vector<string>> int_to_hex(vector<vector<int>>& intVector);
vector<vector<string>> substitutionBytes(vector<vector<string>> hexVector);
void shifRows(vector<string>& row, int offset);
vector<vector<string>> shiftRowMatrix(vector<vector<string>>& substitutionMatrix);
vector<vector<string>> mixColoums(vector<vector<int>>& fixedMatrix, vector<vector<string>>& substitutionMatrix);
vector<vector<string>> encryptMessage_(vector<vector<int>>& fixedMatrix ,vector<vector<string>>& roundKeys, vector<vector<string>>& text, int index);

#endif