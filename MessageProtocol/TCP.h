/*
 *  TIBERO COMMUNICATION PROTOCOL (TCP)
 */

#ifndef MESSAGEPROTOCOL_TCP_H
#define MESSAGEPROTOCOL_TCP_H

#endif //MESSAGEPROTOCOL_TCP_H

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <typeindex>
#include "TCP_TYPE_LIST.h"



/**
 * Target & payload type agnostic messaging
 * WHATEVER YOU WANT, WHOMEVER YOU WANT (some conditions may apply)
 */


//same with typedef unsigned char byte;
using byte = unsigned char;

struct TCPHeader {
    //To read and cast
    int contentsSize;
    bool contentsSplit;
};

struct PartialHeader {
    //Using hashed value of the message's body
    long int ID;
    int Sequence;
    int Total;
};


class Sendable{
public:
    TCPHeader header;
    byte *payload;
    int receiverFD;

    template<typename Recipient>
    void tb_sendto(Recipient receiver) {
        receiverFD = receiver->pipe.getWriteFd();

        this->sendable_main();

        //
        //TESTING CODE
        //

        //use this as the unique id >>>>> std::cout << this << std::endl;

        //write(receiver->pipe.getWriteFd(), &header, sizeof(TCPHeader));
    }

    void initializeHeader() {
        this->header.contentsSize = 0;
        this->header.contentsSplit = false;
    }

    void setPayloadSize(int size) {
        this->header.contentsSize = size;
    }

    void transmitSendable(){
        write(this->receiverFD, &this->header, sizeof(TCPHeader));
        write(this->receiverFD, this->payload, this->header.contentsSize);
    }

    /**
     * pure virtual method that NEEDS TO BE OVERRIDEN
     * @return type of the Sendable based on Enum (TCP_TYPE_LIST)
     */
    virtual int sendable_type() = 0;

    virtual void *sendable_main() = 0;
};

class Receivable{
public:
    template<typename Reader>
    void tb_recv(Reader receiver) {
        //Initialize buffer in case it still has old buffer values
        this->initializeBuffer();

        //read(receiver->pipe.getReadFd(), &buffer, sizeof(TCPHeader));

        read(receiver->pipe.getReadFd(), &buffer, sizeof(TCPHeader));

//        std::cout << ((TCPHeader *) buffer)->contentsSize << std::endl;
//        std::cout << ((TCPHeader *) buffer)->contentsSplit << std::endl;
//        std::cout << ((TCPHeader *) buffer)->contentsType << std::endl;

        //receiver->partialMessageQueue.enque();
        //read SendableHeader only when contentsSplit != true

    }


    byte buffer[512];

    void initializeBuffer() {
        memset(&buffer, 0, sizeof(buffer));
    }
    //private helper methods
};


template<class Derived>
void *run(Derived *derivedSendable) {
    //Initialize header in case it still has old header values
    derivedSendable->initializeHeader();

    //serializing the derived object
    byte *serialized = reinterpret_cast<byte *>(derivedSendable->payload);

    int tempSize = sizeof(*derivedSendable);

    if (!tempSize){

    }



    derivedSendable->header.contentsSize = tempSize;

    derivedSendable->payload = serialized;

    derivedSendable->setPayloadSize(tempSize);


    //Pipe guarantees atomicity only up to 512 bytes.
    if (tempSize > 512) {
        //makeSendables(serialized)
    } else {
        //derivedSendable->payload = derivedSendable->makeSendable(serialized);
        derivedSendable->transmitSendable();
    }



    //std::cout << serialized << std::endl;
    //Derived * deserialized = reinterpret_cast<Derived *>(serialized);
    //deserialized->foo();

}
