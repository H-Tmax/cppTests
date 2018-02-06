/*
 *  TIBERO COMMUNICATION PROTOCOL (TCP)
 */

#ifndef MESSAGEPROTOCOL_TCP_H
#define MESSAGEPROTOCOL_TCP_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <typeindex>
#include "TCP_TYPE_LIST.h"
#include "Serializable.h"


/**
 * Target & payload type agnostic messaging
 * WHATEVER YOU WANT, WHOMEVER YOU WANT (some conditions may apply)
 */

//same with typedef unsigned char byte;
using byte = unsigned char;

struct TCPHeader {
    int contentsType;
    int contentsSize;
    bool contentsSplit;
};

struct PartialHeader {
    long int ID;
    int Sequence;
    int totalCount;
};

struct RawSendable{
    int sendableID;
    int size;
    byte* serializedPayload;
};


#endif //MESSAGEPROTOCOL_TCP_H

