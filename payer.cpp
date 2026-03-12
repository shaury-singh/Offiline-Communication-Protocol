#include "payer.h"
#include "generatingFunctions.h"
#include <vector>

Payer :: Payer(int id, int key){
    state = CLOSED;
    this->SenderID = id;
    this->secretKey = key;
}

int Payer :: generateSYN(){
    this->seqNum = generateSequence(10000, 99999);
    this->ackNum = 0;
    this->SYN = true;
    this->ACK = false;
    return this->seqNum;
}

bool Payer :: setACK(int seqNum, int ackNum) {
    if (ackNum == this->seqNum + 1) {
        this->seqNum = ackNum;
        ACK = true;
        SYN = false;
    } else {
        return false;
    }
    this->ackNum = seqNum + 1;
    return true;
}

std::vector<int> Payer :: returnSYN_ACK() {
    return {this->seqNum, this->ackNum};
}

bool Payer::setSYN(int seqNum) {
    if (seqNum == 0) {
        return false;
    }
    this->seqNum = generateSequence(10000, 99999);
    this->ackNum = seqNum + 1;
    this->SYN = true;
    this->ACK = false;
    return true;
}

int Payer ::returnACK() {
    this->ACK = true;
    return this->seqNum;
}

bool Payer::setSYN_ACK(int seqNum, int ackNum) {
    if (seqNum == this->ackNum) {
        this->ackNum = this->ackNum + 1;
        this->ACK = true;
        this->SYN = false;
        return true;
    } else {
        return false;
    }
}

std::vector<int> Payer::getNum(){
    return {this -> seqNum, this -> ackNum};
}

std::vector<bool> Payer::getFlags(){
    return {this -> SYN, this -> ACK};
}

Header Payer::sendSYN(){
    Header h1;
    int ISN = this->generateSYN();
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    return h1;
}

Header Payer::receiveACKAndSendSYN_ACK(int seqNum, int ackNum){
    this->setACK(seqNum,ackNum);
    Header h1;
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    return h1;
}

Header Payer::receiveSYNAndSendACK(int seqNum){
    this -> setSYN(seqNum);
    this -> returnACK();
    Header h1;
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    return h1;
}