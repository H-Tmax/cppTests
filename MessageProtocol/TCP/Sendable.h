#ifndef MESSAGEPROTOCOL_SENDABLE_H
#define MESSAGEPROTOCOL_SENDABLE_H

#include "TCP.h"

template<class DerivedSendable>
class Sendable : public Serializable<DerivedSendable> {
public:
    template<typename Recipient>
    void tb_sendto(Recipient receiver) {

        std::string payload;
        TCPHeader tcpHeader;

        payload = this->marshal();
        tcpHeader = initializeHeader(payload);

        if (!tcpHeader.payloadSplit) {
            this->transmitSendable(receiver, tcpHeader, payload);
        } else {
            this->transmitSplitSendable(receiver, tcpHeader, payload);
        }
    }
    /**
     * pure virtual method that NEEDS TO BE OVERRIDEN
     * @return type of the Sendable based on Enum (TCP_TYPE_LIST)
     */
    virtual int getSendableType() = 0;

private:

    TCPHeader initializeHeader(std::string payload) {
        TCPHeader header;

        int payloadSize = payload.length();

        header.sendableType = this->getSendableType();
        header.payloadSize = payloadSize;
        header.payloadSplit = payloadSize + sizeof(header) > PIPE_BUF ? true : false;

        return header;
    }

    byte *fuse(TCPHeader header, std::string payload) {
        int headerSize = sizeof(TCPHeader);

        byte *finalPayload = new unsigned char[headerSize + header.payloadSize];

        memcpy(finalPayload, &header, headerSize);
        memcpy(finalPayload + headerSize, payload.c_str(), header.payloadSize);

        return finalPayload;
    }

    template<typename Recipient>
    void transmitSendable(Recipient target, TCPHeader header, std::string payload) {

        int targetFD = target->pipe.getWriteFd();
        byte *finalPayload = this->fuse(header, payload);
        int finalPayloadSize = sizeof(TCPHeader) + header.payloadSize;

        write(targetFD, finalPayload, finalPayloadSize);

        delete finalPayload;
    }

    template<typename Recipient>
    void transmitSplitSendable(Recipient target, TCPHeader tcpHeader, std::string payload) {

        int targetFD = target->pipe.getWriteFd();

        //Temporarily store total payload size since header will be updated
        int remainingPayloadSize = tcpHeader.payloadSize;

        //Size of the payload that will be split per raw Sendable
        const int splitSize = (PIPE_BUF - sizeof(TCPHeader) - sizeof(PartialHeader));

        //TCPHeader is already set, only needs to update the size of the payload
        tcpHeader.payloadSize = splitSize;


        PartialHeader partialHeader;

        partialHeader = initializeHeader(payload, splitSize);

        //all the headers are set at this point
        //TODO: CHECK IF THIS LOOP IS VALID?
        while (remainingPayloadSize > 0) {
            //Change the payload size ONLY for the last raw Sendable
            if (tcpHeader.payloadSize < splitSize) {
                tcpHeader.payloadSize = remainingPayloadSize;
            }
            /**
             *
             */
            //this->transmitSendable(
            // targetFD,
            // this->amalgamate(partial, (partial.Sequence - 1)),
            // tcpHeader.payloadSize);

            //post process
            partialHeader.sequence++;
            remainingPayloadSize = -splitSize;
        }
    }

    long int makeUniqueID() {
        //To get a random value based on time
        std::srand(std::time(nullptr));

        //Get two random values to be used as salts
        int salt = std::rand();
        int pepper = std::rand();
        uint64_t pointerAddress = reinterpret_cast<uint64_t>(this);

        //Setup the hash function to be used for the type (long int)
        std::hash<long int> hash;

        //Sprinkle some spices, then brutally hash it away
        return hash(pointerAddress + salt + pepper);
    }


    PartialHeader initializeHeader(std::string payload, int splitSize){
        PartialHeader partial;

        partial.ID = this->makeUniqueID();
        partial.sequence = 1;

        int payloadSize = payload.length();

        int lastPayloadSize = payloadSize % splitSize;

        if (lastPayloadSize) {
            partial.totalCount = (payloadSize / splitSize) + 1;
        } else {
            partial.totalCount = (payloadSize / splitSize);
        }
    }


    byte *amalgamate(PartialHeader partial, int payloadMarker) {
        int headerSize = sizeof(TCPHeader);

        /**
         *
         */
        //byte *concatenated = new unsigned char[headerSize + sizeof(PartialHeader) + this->header.contentsSize];

        //adding TCP header first
        //memcpy(concatenated, &this->header, headerSize);

        //Then adding partial header
        //memcpy(concatenated + headerSize, &partial, sizeof(PartialHeader));

        //Adding the payload
        //memcpy(concatenated + headerSize + sizeof(PartialHeader),
        //       this->payload + (payloadMarker * this->header.contentsSize),
        //      this->header.contentsSize + sizeof(PartialHeader) + this->header.contentsSize);

        //return concatenated;
    }


};

#endif //MESSAGEPROTOCOL_SENDABLE_H




////
///////testing deserialize code
//DerivedSendable ds;
//ds.unmarshal(payload);
//POL(ds.b);
//POL(ds.c);
//POL(ds.d);
///////