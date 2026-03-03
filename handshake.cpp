#include <iostream>
#include "generatingFunctions.h"
#include "merchant.h"
#include "payer.h"
#include "packet.h"
#include <vector>
#include <fstream>

using namespace std;

Merchant merchant(65425);
Payer payer(16073,63524452);

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

void handshakeInitPayer(Merchant &merchant, Payer &payer){
    Header handshakePkt = payer.sendSYN();
    logPacket("Payer", handshakePkt);
    handshakePkt = merchant.receiveSYNAndSendACK(handshakePkt.seqNum); 
    logPacket("Merchant", handshakePkt);
    handshakePkt = payer.receiveACKAndSendSYN_ACK(handshakePkt.seqNum,handshakePkt.ackNum);
    logPacket("Payer", handshakePkt);
}

int main(){
    handshakeInitPayer(merchant,payer);
    return 0;
}