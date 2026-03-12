#include "merchant.h"
#include "packet.h"
#include "generatingFunctions.h"

Merchant::Merchant(int id) {
    state = CLOSED;
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
        this->seqNum = this->seqNum + 1;
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

bool Merchant::setSYN_ACK(int seqNum, int ackNum) {
    if (seqNum == this->ackNum) {
        this->ackNum = this->ackNum + 1;
        this->ACK = true;
        this->SYN = false;
        return true;
    } else {
        return false;
    }
}

std::vector<int> Merchant::getNum(){
    return {this -> seqNum, this -> ackNum};
}

std::vector<bool> Merchant::getFlags(){
    return {this -> SYN, this -> ACK};
}

Header Merchant::sendSYN(){
    Header h1;
    if (this->state != CLOSED){
        h1.statusCode = INVALID_STATE;
        return h1;
    }
    this->generateSYN();
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    this->state = SYN_SENT;
    h1.statusCode = OK;
    return h1;
}

Header Merchant::receiveACKAndSendSYN_ACK(int seqNum, int ackNum){
    this->setACK(seqNum,ackNum);
    Header h1;
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    return h1;
}

Header Merchant::receiveSYNAndSendACK(int seqNum){
    this -> setSYN(seqNum);
    this -> returnACK();
    Header h1;
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    return h1;
}

Header Merchant::receiveSYN_ACK(int seqNum, int ackNum){
    this -> setSYN_ACK(seqNum,ackNum);
    Header h1;
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    return h1;
}