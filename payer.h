#ifndef PAYER_H
#define PAYER_H

#include <string>

class Payer {
private:
    int secretKey;
    int SenderID;
    int seqNum;
    int ackNum;
    bool SYN;
    bool ACK;
public:
    Payer(int id, int key){
        SenderID = id;
        secretKey = key;
    };
};

#endif