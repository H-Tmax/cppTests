#ifndef MESSAGEPROTOCOL_SENDABLE_H
#define MESSAGEPROTOCOL_SENDABLE_H

#include "TCP.h"

class Sendable : public Serializable {
public:
    TCPHeader header;
    byte *payload;
    int receiverFD;

    template<typename Recipient>
    void tb_sendto(Recipient receiver) {
        receiverFD = receiver->pipe.getWriteFd();

        this->sendable_main();
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

        byte *concatenated = new unsigned char[headerSize + this->header.contentsSize];

        memcpy(concatenated, &this->header, headerSize);

        memcpy(concatenated + headerSize, this->payload, this->header.contentsSize);

        write(this->receiverFD, concatenated, headerSize + this->header.contentsSize);

        delete[] concatenated;
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


#endif //MESSAGEPROTOCOL_SENDABLE_H
