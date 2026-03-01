#include <iostream>
#include <random>
#include <ctime>

using namespace std;

int generateSequence(int start, int end){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distrib(start, end);
    int random_num = distrib(gen);
    return random_num;
}