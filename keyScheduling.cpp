#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include "scheduling.h"

using namespace std;

vector<vector<string>> string_to_hex_(const string& key) {
    unordered_map<char, string> map = {
        {' ', "20"}, {'A', "41"}, {'B', "42"}, {'C', "43"}, {'D', "44"}, {'E', "45"},
        {'F', "46"}, {'G', "47"}, {'H', "48"}, {'I', "49"}, {'J', "4A"}, {'K', "4B"},
        {'L', "4C"}, {'M', "4D"}, {'N', "4E"}, {'O', "4F"}, {'P', "50"}, {'Q', "51"},
        {'R', "52"}, {'S', "53"}, {'T', "54"}, {'U', "55"}, {'V', "56"}, {'W', "57"},
        {'X', "58"}, {'Y', "59"}, {'Z', "5A"}, {'a', "61"}, {'b', "62"}, {'c', "63"},
        {'d', "64"}, {'e', "65"}, {'f', "66"}, {'g', "67"}, {'h', "68"}, {'i', "69"},
        {'j', "6A"}, {'k', "6B"}, {'l', "6C"}, {'m', "6D"}, {'n', "6E"}, {'o', "6F"},
        {'p', "70"}, {'q', "71"}, {'r', "72"}, {'s', "73"}, {'t', "74"}, {'u', "75"},
        {'v', "76"}, {'w', "77"}, {'x', "78"}, {'y', "79"}, {'z', "7A"}
    };

    vector<vector<string>> matrix(4, vector<string>(4));
    for (int i = 0; i < 16; i++) {
        matrix[i % 4][i / 4] = map[key[i]];
    }
    return matrix;
}

unordered_map<string, string> get_sbox() {
    unordered_map<string, string> sbox = {
        {"00", "63"}, {"01", "7C"}, {"02", "77"}, {"03", "7B"}, {"04", "F2"}, {"05", "6B"},
        {"06", "6F"}, {"07", "C5"}, {"08", "30"}, {"09", "01"}, {"0A", "67"}, {"0B", "2B"},
        {"0C", "FE"}, {"0D", "D7"}, {"0E", "AB"}, {"0F", "76"}, {"10", "CA"}, {"11", "82"},
        {"12", "C9"}, {"13", "7D"}, {"14", "FA"}, {"15", "59"}, {"16", "47"}, {"17", "F0"},
        {"18", "AD"}, {"19", "D4"}, {"1A", "A2"}, {"1B", "AF"}, {"1C", "9C"}, {"1D", "A4"},
        {"1E", "72"}, {"1F", "C0"}, {"20", "B7"}, {"21", "FD"}, {"22", "93"}, {"23", "26"},
        {"24", "36"}, {"25", "3F"}, {"26", "F7"}, {"27", "CC"}, {"28", "34"}, {"29", "A5"},
        {"2A", "E5"}, {"2B", "F1"}, {"2C", "71"}, {"2D", "D8"}, {"2E", "31"}, {"2F", "15"},
        {"30", "04"}, {"31", "C7"}, {"32", "23"}, {"33", "C3"}, {"34", "18"}, {"35", "96"},
        {"36", "05"}, {"37", "9A"}, {"38", "07"}, {"39", "12"}, {"3A", "80"}, {"3B", "E2"},
        {"3C", "EB"}, {"3D", "27"}, {"3E", "B2"}, {"3F", "75"}, {"40", "09"}, {"41", "83"},
        {"42", "2C"}, {"43", "1A"}, {"44", "1B"}, {"45", "6E"}, {"46", "5A"}, {"47", "A0"},
        {"48", "52"}, {"49", "3B"}, {"4A", "D6"}, {"4B", "B3"}, {"4C", "29"}, {"4D", "E3"},
        {"4E", "2F"}, {"4F", "84"}, {"50", "53"}, {"51", "D1"}, {"52", "00"}, {"53", "ED"},
        {"54", "20"}, {"55", "FC"}, {"56", "B1"}, {"57", "5B"}, {"58", "6A"}, {"59", "CB"},
        {"5A", "BE"}, {"5B", "39"}, {"5C", "4A"}, {"5D", "4C"}, {"5E", "58"}, {"5F", "CF"},
        {"60", "D0"}, {"61", "EF"}, {"62", "AA"}, {"63", "FB"}, {"64", "43"}, {"65", "4D"},
        {"66", "33"}, {"67", "85"}, {"68", "45"}, {"69", "F9"}, {"6A", "02"}, {"6B", "7F"},
        {"6C", "50"}, {"6D", "3C"}, {"6E", "9F"}, {"6F", "A8"}, {"70", "51"}, {"71", "A3"},
        {"72", "40"}, {"73", "8F"}, {"74", "92"}, {"75", "9D"}, {"76", "38"}, {"77", "F5"},
        {"78", "BC"}, {"79", "B6"}, {"7A", "DA"}, {"7B", "21"}, {"7C", "10"}, {"7D", "FF"},
        {"7E", "F3"}, {"7F", "D2"}, {"80", "CD"}, {"81", "0C"}, {"82", "13"}, {"83", "EC"},
        {"84", "5F"}, {"85", "97"}, {"86", "44"}, {"87", "17"}, {"88", "C4"}, {"89", "A7"},
        {"8A", "7E"}, {"8B", "3D"}, {"8C", "64"}, {"8D", "5D"}, {"8E", "19"}, {"8F", "73"},
        {"90", "60"}, {"91", "81"}, {"92", "4F"}, {"93", "DC"}, {"94", "22"}, {"95", "2A"},
        {"96", "90"}, {"97", "88"}, {"98", "46"}, {"99", "EE"}, {"9A", "B8"}, {"9B", "14"},
        {"9C", "DE"}, {"9D", "5E"}, {"9E", "0B"}, {"9F", "DB"}, {"A0", "E0"}, {"A1", "32"},
        {"A2", "3A"}, {"A3", "0A"}, {"A4", "49"}, {"A5", "06"}, {"A6", "24"}, {"A7", "5C"},
        {"A8", "C2"}, {"A9", "D3"}, {"AA", "AC"}, {"AB", "62"}, {"AC", "91"}, {"AD", "95"},
        {"AE", "E4"}, {"AF", "79"}, {"B0", "E7"}, {"B1", "C8"}, {"B2", "37"}, {"B3", "6D"},
        {"B4", "8D"}, {"B5", "D5"}, {"B6", "4E"}, {"B7", "A9"}, {"B8", "6C"}, {"B9", "56"},
        {"BA", "F4"}, {"BB", "EA"}, {"BC", "65"}, {"BD", "7A"}, {"BE", "AE"}, {"BF", "08"},
        {"C0", "BA"}, {"C1", "78"}, {"C2", "25"}, {"C3", "2E"}, {"C4", "1C"}, {"C5", "A6"},
        {"C6", "B4"}, {"C7", "C6"}, {"C8", "E8"}, {"C9", "DD"}, {"CA", "74"}, {"CB", "1F"},
        {"CC", "4B"}, {"CD", "BD"}, {"CE", "8B"}, {"CF", "8A"}, {"D0", "70"}, {"D1", "3E"},
        {"D2", "B5"}, {"D3", "66"}, {"D4", "48"}, {"D5", "03"}, {"D6", "F6"}, {"D7", "0E"},
        {"D8", "61"}, {"D9", "35"}, {"DA", "57"}, {"DB", "B9"}, {"DC", "86"}, {"DD", "C1"},
        {"DE", "1D"}, {"DF", "9E"}, {"E0", "E1"}, {"E1", "F8"}, {"E2", "98"}, {"E3", "11"},
        {"E4", "69"}, {"E5", "D9"}, {"E6", "8E"}, {"E7", "94"}, {"E8", "9B"}, {"E9", "1E"},
        {"EA", "87"}, {"EB", "E9"}, {"EC", "CE"}, {"ED", "55"}, {"EE", "28"}, {"EF", "DF"},
        {"F0", "8C"}, {"F1", "A1"}, {"F2", "89"}, {"F3", "0D"}, {"F4", "BF"}, {"F5", "E6"},
        {"F6", "42"}, {"F7", "68"}, {"F8", "41"}, {"F9", "99"}, {"FA", "2D"}, {"FB", "0F"},
        {"FC", "B0"}, {"FD", "54"}, {"FE", "BB"}, {"FF", "16"}
    };
    return sbox;
}

vector<string> xorWords_(vector<string> a, vector<string> b) {
    vector<string> result(4);
    for (int i = 0; i < 4; i++) {
        int val = stoi(a[i], nullptr, 16) ^ stoi(b[i], nullptr, 16);
        stringstream ss;
        ss << hex << uppercase << setfill('0') << setw(2) << val;
        result[i] = ss.str();
    }
    return result;
}

vector<string> rotWord_(vector<string> word) {
    rotate(word.begin(), word.begin() + 1, word.end());
    return word;
}

vector<string> subWord_(vector<string> word, unordered_map<string, string>& sbox) {
    vector<string> result(4);
    for (int i = 0; i < 4; i++) {
        result[i] = sbox[word[i]];
    }
    return result;
}

vector<vector<string>> generateRoundKeys_(vector<vector<string>>& hexMatrix, unordered_map<string, string>& sbox) {
    unordered_map<int, vector<int>> rCon = {
        {1, {0x01, 0x00, 0x00, 0x00}}, {2, {0x02, 0x00, 0x00, 0x00}}, {3, {0x04, 0x00, 0x00, 0x00}},
        {4, {0x08, 0x00, 0x00, 0x00}}, {5, {0x10, 0x00, 0x00, 0x00}}, {6, {0x20, 0x00, 0x00, 0x00}},
        {7, {0x40, 0x00, 0x00, 0x00}}, {8, {0x80, 0x00, 0x00, 0x00}}, {9, {0x1B, 0x00, 0x00, 0x00}},
        {10, {0x36, 0x00, 0x00, 0x00}}
    };

    vector<vector<string>> w;

    for (int i = 0; i < 4; i++) {
        vector<string> word(4);
        for (int j = 0; j < 4; j++) {
            word[j] = hexMatrix[j][i];
        }
        w.push_back(word);
    }

    for (int i = 4; i < 44; i++) {
        vector<string> temp = w[i - 1];
        if (i % 4 == 0) {
            temp = rotWord_(temp);
            temp = subWord_(temp, sbox);
            vector<string> rconWord(4);
            for (int j = 0; j < 4; j++) {
                stringstream ss;
                ss << hex << uppercase << setfill('0') << setw(2) << rCon[i / 4][j];
                rconWord[j] = ss.str();
            }
            temp = xorWords_(temp, rconWord);
        }
        w.push_back(xorWords_(w[i - 4], temp));
    }

    return w;
}

void printRoundKeys(const vector<vector<string>>& w) {
    for (int round = 0; round < 11; round++) {
        cout << "Round " << round << " Key:\n";
        for (int row = 0; row < 4; row++) {
            for (int col = 0; col < 4; col++) {
                cout << w[round * 4 + col][row] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

// int main() {
//     string key = "Thats my Kung Fu";
//     auto hexMatrix = string_to_hex_(key);
//     auto sbox = get_sbox();
//     auto roundKeys = generateRoundKeys_(hexMatrix, sbox);
//     cout << roundKeys.size() << endl;
//     printRoundKeys(roundKeys);
//     return 0;
// }