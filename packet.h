#ifndef PACKET_H
#define PACKET_H

#include <string>

struct Header{
    int pktCode;
    int senderID;
    int seqNum;
    int ackNum;
    int statusCode;
    int stateCode;
    bool SYN;
    bool ACK;
};

struct Payload{
    int data;
    std::string rachetKey;
};

struct Tail{
    int checkSum;
    int errorCorrection;
};

struct Packet{
    Header header;
    Payload payload;
    Tail tail;
};

#endif