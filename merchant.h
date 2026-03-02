#ifndef MERCHANT_H
#define MERCHANT_H

#include <vector>

class Merchant {
    private:
        int globalMasterKey;
        int merchantID;
        int seqNum;
        int ackNum;
        bool SYN;
        bool ACK;
    public:
        Merchant(int id);
        int generateSYN();
        bool setACK(int seqNum, int ackNum);
        std::vector<int> returnSYN_ACK();
        bool setSYN(int seqNum);
        int returnACK();
        bool setSYN_ACK(int seqNum, int ackNum, int payloadSize);
};

#endif