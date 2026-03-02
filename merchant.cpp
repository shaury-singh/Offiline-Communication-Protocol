#include "Merchant.h"
#include "generatingFunctions.h"

Merchant::Merchant(int id) {
    merchantID = id;
    globalMasterKey = 17374626;
}

int Merchant::generateSYN() {
    this->seqNum = generateSequence(10000, 99999);
    this->ackNum = 0;
    this->SYN = true;
    this->ACK = false;
    return this->seqNum;
}

bool Merchant::setACK(int seqNum, int ackNum) {
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

std::vector<int> Merchant::returnSYN_ACK() {
    return {this->seqNum, this->ackNum};
}

bool Merchant::setSYN(int seqNum) {
    if (seqNum == 0) {
        return false;
    }
    this->seqNum = generateSequence(10000, 99999);
    this->ackNum = seqNum + 1;
    this->SYN = true;
    this->ACK = false;
    return true;
}

int Merchant::returnACK() {
    this->ACK = true;
    return this->seqNum;
}

bool Merchant::setSYN_ACK(int seqNum, int ackNum, int payloadSize) {
    if (seqNum == this->ackNum) {
        this->ackNum = this->ackNum + payloadSize;
        this->ACK = true;
        this->SYN = false;
        return true;
    } else {
        return false;
    }
}