#ifndef MESSAGEPROTOCOL_SENDABLE_H
#define MESSAGEPROTOCOL_SENDABLE_H

#include "TCP.h"

class Sendable : public Serializable {
public:
    TCPHeader header;
    byte *payload;

    template<typename Recipient>
    void tb_sendto(Recipient receiver) {
        int receiverFD = receiver->pipe.getWriteFd();

        this->sendable_main();

        this->transmitSendable(receiverFD, this->makeCompleteSendable());
    }

    void initializeHeader() {
        this->header.contentsType = 0;
        this->header.contentsSize = 0;
        this->header.contentsSplit = false;
    }

    byte *makeCompleteSendable() {
        int headerSize = sizeof(TCPHeader);

        byte *concatenated = new unsigned char[headerSize + this->header.contentsSize];

        memcpy(concatenated, &this->header, headerSize);

        memcpy(concatenated + headerSize, this->payload, this->header.contentsSize);

        return concatenated;
    }

    void transmitSendable(int targetFD, byte *payload) {
        write(targetFD, payload, sizeof(TCPHeader) + this->header.contentsSize);
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
    }
}

#endif //MESSAGEPROTOCOL_SENDABLE_H
