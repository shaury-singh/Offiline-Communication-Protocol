#include <iostream>
#include "generatingFunctions.h"
#include "merchant.h"
#include "payer.h"
#include "packet.h"
#include "fsk.h"
#include <vector>
#include <fstream>
#include <cstdint>
#include <cmath>

using namespace std;

Merchant merchant(65425);
Payer payer("16073");
Packet p{};

void logHeader(const std::string& sender, const Header& pkt) {
    std::ofstream logFile("handshake_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << sender << " : "
                << "[" << pkt.seqNum << "]"
                << "[" << pkt.ackNum << "]"
                << "[" << pkt.SYN << "]"
                << "[" << pkt.ACK << "]"
                << std::endl;
    }
    logFile.close();
}

void logPacket(const std::string& sender, const Packet& pkt) {
    std::ofstream logFile("handshake_log.txt", std::ios::app);
    if (logFile.is_open()) {
        logFile << sender << " : "
                << "[" << pkt.header.seqNum << "]"
                << "[" << pkt.header.ackNum << "]"
                << "[" << pkt.header.SYN << "]"
                << "[" << pkt.header.ACK << "]"
                << "[" << pkt.header.senderID << "]"
                << "[" << pkt.payload.data << "]"
                << "[" << pkt.payload.stringData << "]"
                << std::endl;
    }
    logFile.close();
}

void controller(int pktCode, Merchant &merchant, Payer &payer){
    switch (pktCode){
        case 0:{
            Header h = merchant.sendSYN();
            logHeader("65425",h);
            break;
        }
        case 1:{
            int seqNum = merchant.getNum()[0];
            Header h = payer.receiveSYNAndSendACK(seqNum);
            logHeader("16073",h);
            break;
        }
        case 2:{
            int seqNum = payer.getNum()[0];
            int ackNum = payer.getNum()[1];
            Header h = merchant.receiveACKAndSendSYN_ACK(seqNum,ackNum);
            logHeader("65425",h);
            break;
        }
        case 3:{
            Header h = payer.sendSYN();
            logHeader("16073",h);
            break;
        }
        case 4:{
            int seqNum = payer.getNum()[0];
            Header h = merchant.receiveSYNAndSendACK(seqNum);
            logHeader("65425",h);
            break;
        }
        case 5:{
            int seqNum = merchant.getNum()[0];
            int ackNum = merchant.getNum()[1];
            Header h = payer.receiveACKAndSendSYN_ACK(seqNum,ackNum);
            logHeader("16073",h);
            break;
        }
        case 6:{
            p = payer.sendIDasPayload();
            logPacket("16073",p);
            auto bytes = serializePacket(p);
            auto bits = bytesToBits(bytes);
            auto signal = fskModulate(bits);
            writeWav("ID_as_payload.wav", signal);
            cout << "WAV written: ID_as_payload.wav" << endl;
            break;
        }
        case 7:{
            std::string userID = p.payload.stringData;
            int seqNum = p.header.seqNum;
            cout << "Seq Num: " << seqNum << endl;
            p = merchant.validatePacketAndgenerateChallenge(seqNum,userID);
            logPacket("65425",p);
            auto bytes = serializePacket(p);
            auto bits = bytesToBits(bytes);
            auto signal = fskModulate(bits);
            writeWav("challenge.wav", signal);
            cout << "WAV written: challenge.wav" << endl;
            break;
        }
        case 8:{
            int seqNum = p.header.seqNum;
            std::string challenge = p.payload.stringData;
            p = payer.receiveChallengeandSendDecryptedSecret(seqNum,challenge);
            logPacket("16073",p);
            auto bytes = serializePacket(p);
            auto bits = bytesToBits(bytes);
            auto signal = fskModulate(bits);
            writeWav("decryptedChallenge.wav", signal);
            cout << "WAV written: decryptedChallenge.wav" << endl;
            break;
        }
        case 9:{
            int seqNum = p.header.seqNum;
            std::string decryptedChallenge = p.payload.stringData;
            p = merchant.authenticateUser(seqNum,decryptedChallenge);
            logPacket("65425",p);
            auto bytes = serializePacket(p);
            auto bits = bytesToBits(bytes);
            auto signal = fskModulate(bits);
            writeWav("userAuthenticatedDecision.wav", signal);
            cout << "WAV written: userAuthenticatedDecision.wav" << endl;
            break;
        }
        case -1:{
            int seqNum = merchant.getNum()[0];
            int ackNum = merchant.getNum()[1];
            Header h = payer.receiveSYN_ACK(seqNum,ackNum);
            break;
        }
        case -2:{
            int seqNum = payer.getNum()[0];
            int ackNum = payer.getNum()[1];
            Header h = merchant.receiveSYN_ACK(seqNum,ackNum);
            logHeader("65425",h);
            break;
        }
        default:
            break;
        }
}


int main(){
    controller(3,merchant,payer);
    controller(4,merchant,payer);
    controller(5,merchant,payer);
    controller(-2,merchant,payer);
    controller(6,merchant,payer);
    controller(7,merchant,payer);
    controller(8,merchant,payer);
    controller(9,merchant,payer);
    return 0;
}