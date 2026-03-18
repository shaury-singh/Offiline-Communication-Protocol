#ifndef PAYER_H
#define PAYER_H

#include <string>
#include "packet.h"
#include <vector> 

class Payer {
    private:
        connState state;
        std::string secretKey;
        std::string SenderID;
        int seqNum;
        int ackNum;
        bool SYN;
        bool ACK;
    public:
        Payer(std::string id);
        int generateSYN();
        bool setACK(int seqNum, int ackNum);
        std::vector<int> returnSYN_ACK();
        bool setSYN(int seqNum);
        int returnACK();
        bool setSYN_ACK(int seqNum, int ackNum);
        std::vector<bool> getFlags();
        std::vector<int> getNum();
        Header sendSYN();
        Header receiveACKAndSendSYN_ACK(int seqNum, int ackNum);
        Header receiveSYNAndSendACK(int seqNum);
        Header receiveSYN_ACK(int seqNum, int ackNum);
        Packet sendIDasPayload();
        Packet receiveChallengeandSendDecryptedSecret(int seqNum, std::string challenge);
};

#endif