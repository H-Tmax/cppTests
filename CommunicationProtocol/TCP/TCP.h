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
 * WHATEVER YOU WANT, TO WHOMEVER YOU WANT (some conditions may apply)
 */

//same with typedef unsigned char byte;
using byte = char;

struct TCPHeader {
    int sendableType;
    int payloadSize;
    bool payloadSplit;
};

struct PartialHeader {
    long int ID;
    int sequence;
    int totalCount;
};

struct RawSendable{
    int sendableID;
    int size;
    byte* serializedPayload;
};

using const int TRANSMISSION_SUCCESS = 0;



#endif //MESSAGEPROTOCOL_TCP_H

