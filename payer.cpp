#include "payer.h"
#include "keyDerivation.h"
#include "generatingFunctions.h"
#include "encryption.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

Payer :: Payer(std::string id){
    state = CLOSED;
    this->SenderID = id;
    this->secretKey = deriveKey("17374626",id);
    cout << "key: " << this->secretKey;
}

int Payer :: generateSYN(){
    this->seqNum = generateSequence(10000, 99999);
    this->ackNum = 0;
    this->SYN = true;
    this->ACK = false;
    return this->seqNum;
}

bool Payer :: setACK(int seqNum, int ackNum) {
    if (this->state != SYN_SENT){
        return false;
    }
    if (ackNum == this->seqNum + 1) {
        this->seqNum = ackNum;
        ACK = true;
        SYN = false;
    } else {
        return false;
    }
    this->ackNum = seqNum + 1;
    this->state = ACK_RECEIVED;
    return true;
}

std::vector<int> Payer :: returnSYN_ACK() {
    return {this->seqNum, this->ackNum};
}

bool Payer::setSYN(int seqNum) {
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

int Payer ::returnACK() {
    this->ACK = true;
    return this->seqNum;
}

bool Payer::setSYN_ACK(int seqNum, int ackNum) {
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

std::vector<int> Payer::getNum(){
    return {this -> seqNum, this -> ackNum};
}

std::vector<bool> Payer::getFlags(){
    return {this -> SYN, this -> ACK};
}

Header Payer::sendSYN(){
    Header h1{};
    if (this->state != CLOSED){
        h1.statusCode = INVALID_STATE;
        return h1;
    }
    int ISN = this->generateSYN();
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    this->state = SYN_SENT;
    h1.statusCode = OK;
    return h1;
}

Header Payer::receiveACKAndSendSYN_ACK(int seqNum, int ackNum){
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

Header Payer::receiveSYNAndSendACK(int seqNum){
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

Header Payer::receiveSYN_ACK(int seqNum, int ackNum){
    bool flag = this -> setSYN_ACK(seqNum,ackNum);
    Header h1{};
    if (flag == false){
        h1.statusCode = INVALID_STATE;
        return h1;
    }
    h1.seqNum = this -> seqNum;
    h1.ackNum = this-> ackNum;
    h1.SYN = this -> SYN;
    h1.ACK = this -> ACK;
    h1.statusCode = HANDSHAKE_COMPLETE;
    this->state = ESTABLISHED;
    return h1;
}

Packet Payer::sendIDasPayload(){
    Packet p1{};
    if (this->state != ESTABLISHED){
        p1.header.statusCode = INVALID_STATE;
        return p1;
    }
    p1.header.senderID = this->SenderID;
    this->seqNum++;
    p1.header.seqNum = this->seqNum;
    this->seqNum += this->SenderID.size();
    p1.header.ackNum = this->ackNum;
    p1.header.SYN = true;
    p1.header.ACK = true;
    p1.header.statusCode = OK;
    p1.payload.stringData = this->SenderID;
    this->state = ID_SENT;
    return p1;
}

Packet Payer::receiveChallengeandSendDecryptedSecret(int seqNum, std::string challenge){
    Packet p1{};
    if (this->state != ID_SENT){
        p1.header.statusCode = INVALID_PACKET;
        return p1;
    }
    if (seqNum != this->ackNum){
        p1.header.statusCode = INVALID_PACKET;
        return p1;
    }
    this->ackNum += challenge.size();
    string decryptChallenge = decrypt(this->secretKey,challenge);
    p1.header.seqNum = this->seqNum;
    p1.header.ackNum = this->ackNum;
    p1.header.SYN = this->SYN;
    p1.header.ACK = this->ACK;
    p1.header.senderID = this->SenderID;
    p1.payload.stringData = decryptChallenge;
    return p1;
}