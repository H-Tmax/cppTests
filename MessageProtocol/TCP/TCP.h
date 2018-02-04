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

//sds
//same with typedef unsigned char byte;
using byte = unsigned char;

struct TCPHeader {
    int contentsType;
    int contentsSize;
    bool contentsSplit;
};

struct PartialHeader : public Serializable {
    //Using hashed value of the message's body + random salt * 2
    long int ID;
    int Sequence;
    int Total;
};


#endif //MESSAGEPROTOCOL_TCP_H

