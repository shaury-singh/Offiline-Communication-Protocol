#ifndef PAYER_H
#define PAYER_H

#include <string>
#include "packet.h"
#include <vector> 

class Payer {
    private:
        int secretKey;
        int SenderID;
        int seqNum;
        int ackNum;
        bool SYN;
        bool ACK;
    public:
        Payer(int id, int key);
        int generateSYN();
        bool setACK(int seqNum, int ackNum);
        std::vector<int> returnSYN_ACK();
        bool setSYN(int seqNum);
        int returnACK();
        bool setSYN_ACK(int seqNum, int ackNum, int payloadSize);
        std::vector<bool> getFlags();
        std::vector<int> getNum();
        Header sendSYN();
        Header receiveACKAndSendSYN_ACK(int seqNum, int ackNum);
        Header receiveSYNAndSendACK(int seqNum);
        Header receiveSYN_ACK(int seqNum, int ackNum, int payloadSize);
};

#endif