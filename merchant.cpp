#include "merchant.h"
#include "packet.h"
#include "generatingFunctions.h"
#include "keyDerivation.h"
#include "encryption.h"
#include <string>
#include <iostream>

Merchant::Merchant(int id) {
    state = CLOSED;
    merchantID = id;
    globalMasterKey = "17374626";
}

int Merchant::generateSYN() {
    this->seqNum = generateSequence(10000, 99999);
    this->ackNum = 0;
    this->SYN = true;
    this->ACK = false;
    return this->seqNum;
}

bool Merchant::setACK(int seqNum, int ackNum) {
    if (this->state != SYN_SENT){
        return false;
    }
    if (ackNum == this->seqNum + 1) {
        this->seqNum = this->seqNum + 1;
        ACK = true;
        SYN = false;
    } else {
        return false;
    }
    this->ackNum = seqNum + 1;
    this->state = ACK_RECEIVED;
    return true;
}

std::vector<int> Merchant::returnSYN_ACK() {
    return {this->seqNum, this->ackNum};
}

bool Merchant::setSYN(int seqNum) {
    if (this->state != CLOSED){
        return false;
    }
    if (seqNum == 0) {
        return false;
    }
    this->seqNum = generateSequence(10000, 99999);
    this->ackNum = seqNum + 1;
    this->SYN = true;
    this->ACK = false;
    this->state = SYN_RECEIVED;
    return true;
}

int Merchant::returnACK() {
    this->ACK = true;
    return this->seqNum;
}

bool Merchant::setSYN_ACK(int seqNum, int ackNum) {
    if (this->state != ACK_SENT){
        return false;
    }
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
    Header h1{};
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
    Header h1{};
    bool flag = this->setACK(seqNum,ackNum);
    if (this->state != ACK_RECEIVED || flag == false){
        if (flag == false){
            h1.statusCode = INVALID_PACKET;
            return h1;
        }
        h1.statusCode = INVALID_STATE;
        return h1;
    }
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    this->state = ESTABLISHED;
    return h1;
}

Header Merchant::receiveSYNAndSendACK(int seqNum){
    bool flag = this -> setSYN(seqNum);
    Header h1{};
    if (this->state != SYN_RECEIVED || flag == false){
        if (flag == false){
            h1.statusCode = INVALID_PACKET;
            return h1;
        }
        h1.statusCode = INVALID_STATE;
        return h1;
    }
    this -> returnACK();
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    this->state = ACK_SENT;
    return h1;
}

Header Merchant::receiveSYN_ACK(int seqNum, int ackNum){
    bool  flag = this -> setSYN_ACK(seqNum,ackNum);
    Header h1{};
    if (flag == false){
        h1.statusCode = INVALID_STATE;
        return h1;
    }
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    this->state = ESTABLISHED;
    return h1;
}

Packet Merchant::validatePacketAndgenerateChallenge(int seqNum, std::string userID){
    Packet p1{};
    if (this->state != ESTABLISHED){
        p1.header.statusCode = INVALID_STATE;
        return p1;
    }
    if (seqNum != this->ackNum){
        p1.header.statusCode = INVALID_PACKET;
        return p1;
    }
    std::string challenge = std::to_string(generateSequence(10000000,99999999));
    std::string secretKey = deriveKey(this->globalMasterKey,userID);
    std::string challengeString = encrypt(secretKey,challenge);
    std::cout << "Encrypted String: " << challengeString << std::endl;
    std::cout << "challenge is: " << challenge << " and size is: " << challengeString.size() << std::endl;
    this->ackNum += userID.size();
    this->seqNum++;
    p1.header.seqNum = this->seqNum;
    this->seqNum += challengeString.size();
    p1.header.ackNum = this->ackNum;
    p1.header.SYN = this->SYN;
    p1.header.ACK = this->ACK;
    p1.header.statusCode = OK;
    p1.payload.stringData = challengeString;
    this->state = CHALLENGE_SENT;
    return p1;
}