#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include "sessionkey.h"

using namespace std;

string generateSessionKey(){
    string charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMOPQRSTUVWXYZ";
    string result = "";
    srand(time(0));
    for (int i=0; i<16; i++){
        int random = (rand() % 51) + 0;
        result += charSet[random];
    }
    return result;
}

int main(){
    cout << generateSessionKey();
    return 0;
}