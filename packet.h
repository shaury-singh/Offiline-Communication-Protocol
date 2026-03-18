#ifndef PACKET_H
#define PACKET_H

#include <string>

enum connState{
    CLOSED,
    SYN_SENT,
    ACK_RECEIVED,
    SYN_RECEIVED,
    ACK_SENT,
    ESTABLISHED,
    CHALLENGE_SENT,
    ID_SENT,
    INVALID_PAYER,
    VALIDATED
};

enum StatusCode{
    OK = 0,
    INVALID_STATE,
    INVALID_PACKET,
    UNEXPECTED_ACK,
    UNEXPECTED_SYN,
    HANDSHAKE_COMPLETE,
    CHALLENGE_DECRYPTED,
    INVALID_USER
};

struct Header{
    int pktCode;
    std::string senderID;
    int seqNum;
    int ackNum;
    StatusCode statusCode;
    int stateCode;
    bool SYN;
    bool ACK;
};

struct Payload{
    int data;
    std::string stringData;
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