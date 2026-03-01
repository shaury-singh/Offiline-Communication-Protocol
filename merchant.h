#ifndef MERCHANT_H
#define MERCHANT_H

#include <string>

class Merchant {
private:
    int globalMasterKey;
    int merchantID;
    int mySeq;
    int peerSeq;
public:
    Merchant(int id){
        merchantID = id;
        globalMasterKey = 17374626;
    };
};

#endif