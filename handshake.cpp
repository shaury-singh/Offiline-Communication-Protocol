#include <iostream>
#include "generatingFunctions.h"
#include "merchant.h"
#include "payer.h"
#include "packet.h"
#include <vector>
#include <fstream>

using namespace std;

Merchant merchant(65425);
Payer payer(16073);

void logPacket(const std::string& sender, const Header& pkt) {
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

void controllerHandshake(int pktCode, Merchant &merchant, Payer &payer){
    switch (pktCode){
        case 0:{
            Header h = merchant.sendSYN();
            logPacket("Merchant",h);
            break;
        }
        case 1:{
            int seqNum = merchant.getNum()[0];
            Header h = payer.receiveSYNAndSendACK(seqNum);
            logPacket("Payer",h);
            break;
        }
        case 2:{
            int seqNum = payer.getNum()[0];
            int ackNum = payer.getNum()[1];
            Header h = merchant.receiveACKAndSendSYN_ACK(seqNum,ackNum);
            logPacket("Merchant",h);
            break;
        }
        case 3:{
            Header h = payer.sendSYN();
            logPacket("Payer",h);
            break;
        }
        case 4:{
            int seqNum = payer.getNum()[0];
            Header h = merchant.receiveSYNAndSendACK(seqNum);
            logPacket("Merchant",h);
            break;
        }
        case 5:{
            int seqNum = merchant.getNum()[0];
            int ackNum = merchant.getNum()[1];
            Header h = payer.receiveACKAndSendSYN_ACK(seqNum,ackNum);
            logPacket("Payer",h);
            break;
        }
        case 6:{
            int seqNum = merchant.getNum()[0];
            int ackNum = merchant.getNum()[1];
            Header h = payer.receiveSYN_ACK(seqNum,ackNum);
            logPacket("Payer",h);
            break;
        }
        case 7:{
            int seqNum = payer.getNum()[0];
            int ackNum = payer.getNum()[1];
            Header h = merchant.receiveSYN_ACK(seqNum,ackNum);
            logPacket("Merchant",h);
            break;
        }
        default:
            break;
        }
}

int main(){
    controllerHandshake(3,merchant,payer);
    controllerHandshake(4,merchant,payer);
    controllerHandshake(5,merchant,payer);
    controllerHandshake(7,merchant,payer);
    return 0;
}