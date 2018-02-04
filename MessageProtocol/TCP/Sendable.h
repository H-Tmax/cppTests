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

    void initializeHeader() {
        this->header.contentsType = 0;
        this->header.contentsSize = 0;
        this->header.contentsSplit = false;
    }

    byte *prepareTransmission() {
        int headerSize = sizeof(TCPHeader);

        byte *concatenated = new unsigned char[headerSize + this->header.contentsSize];

        memcpy(concatenated, &this->header, headerSize);

        memcpy(concatenated + headerSize, this->payload, this->header.contentsSize);

        return concatenated;
    }

    void transmitSendable(byte *payload) {
        write(this->receiverFD, payload, sizeof(TCPHeader) + this->header.contentsSize);
        delete[] payload;
    }

    void setHeader(int sendable_type, int contentsSize, bool isSplit) {
        this->header.contentsType = sendable_type;
        this->header.contentsSize = contentsSize;
        this->header.contentsSplit = isSplit;
    }

    void setPayload(byte *payload) {
        this->payload = payload;
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

    //Serialize the contents (derived Sendable)
    derivedSendable->oar << *derivedSendable;

    //Get the size of the contents
    int totalSize = derivedSendable->buf.str().length();

    //Pipe guarantees atomicity only up to 512 bytes.
    if (totalSize > 512) {
//        std::string a = "hello world0101010";
//        std::hash<std::string> hash;
//        std::cout << hash(a) << std::endl;

        //makeSendables(serialized)
    } else {
        //Set the Sendable header properly
        derivedSendable->setHeader(derivedSendable->sendable_type(), totalSize, false);

        //Set the payload, which is the serialized object
        derivedSendable->setPayload((byte *) derivedSendable->buf.str().c_str());

        derivedSendable->transmitSendable(derivedSendable->prepareTransmission());
    }

}


#endif //MESSAGEPROTOCOL_SENDABLE_H
