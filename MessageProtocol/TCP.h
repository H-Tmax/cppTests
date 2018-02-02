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

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/vector.hpp>

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
    //Using hashed value of the message's body + random salt * 2
    long int ID;
    int Sequence;
    int Total;
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class Sendable {
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

    //for all Sendable object
//    void serializeHeader(){
//        serialize()
//    }
//
//    template<class Archive>
//    void serialize(Archive & ar, const unsigned int version)
//    {
//        ar & b;
//        ar & c;
//    }

    void initializeHeader() {
        this->header.contentsSize = 0;
        this->header.contentsSplit = false;
    }



    void setPayloadSize(int size) {
        this->header.contentsSize = size;
    }

    void transmitSendable(){
        int headerSize = sizeof(TCPHeader);

        byte *concatted = new unsigned char[headerSize + this->header.contentsSize];

        memcpy(concatted, &this->header, headerSize);

        memcpy(concatted + headerSize, this->payload, this->header.contentsSize);

        write(this->receiverFD, concatted, headerSize + this->header.contentsSize);

        delete[] concatted;
    }


    //Has to call serialize function of
    void serializeBody(){

    }



    /**
     * pure virtual method that NEEDS TO BE OVERRIDEN
     * @return type of the Sendable based on Enum (TCP_TYPE_LIST)
     */
    virtual int sendable_type() = 0;

    virtual void *sendable_main() = 0;

};

template<class Derived>
void *run(Derived *derivedSendable) {
    //Initialize header in case it still has old header values
    derivedSendable->initializeHeader();

    //serializing the derived object
    //byte *serialized = reinterpret_cast<byte *>(derivedSendable->payload);

    derivedSendable->oar << *derivedSendable;

    int tempSize = derivedSendable->buf.str().length();

    Derived ddd;


    derivedSendable->iar >> ddd;

    std::cout << ddd.b << std::endl;
    std::cout << ddd.c << std::endl;



    derivedSendable->payload = (byte *)derivedSendable->buf.str().c_str();

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

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class Receivable{
public:
    template<typename Reader>
    void tb_recv(Reader receiver) {
        //Initialize buffer in case it still has old buffer values
        this->initializeBuffer();

        //read(receiver->pipe.getReadFd(), &buffer, sizeof(TCPHeader));

        read(receiver->pipe.getReadFd(), buffer, sizeof(TCPHeader));

        int sssssize = ((TCPHeader *) receiver->buffer)->contentsSize;

        std::cout << sssssize << std::endl;

        std::cout << ((TCPHeader *) receiver->buffer)->contentsSplit << std::endl;

        read(receiver->pipe.getReadFd(), buffer, sssssize);



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

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
/**
 *
 */
class Serializable {
public:

    friend class boost::serialization::access;


    std::stringbuf buf;
    std::ostream os;
    boost::archive::binary_oarchive oar;

    std::istream is;
    boost::archive::binary_iarchive iar;
    //virtual void serialize() = 0;

    Serializable() : buf(), os(&buf), oar(os, boost::archive::no_header),  is(&buf), iar(is, boost::archive::no_header) {
        //NOTHING TO SEE HERE
    }

//    virtual void serialize(boost::archive::binary_oarchive & oar) = 0;

};





#endif //MESSAGEPROTOCOL_TCP_H

