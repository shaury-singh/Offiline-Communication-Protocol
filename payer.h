#ifndef PAYER_H
#define PAYER_H

#include <string>

class Payer {
private:
    int secretKey;
    int SenderID;
    int mySeq;
    int peerSeq;
public:
    Payer(int id, int key){
        SenderID = id;
        secretKey = key;
    };
};

#endif