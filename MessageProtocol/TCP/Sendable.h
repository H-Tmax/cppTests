#ifndef MESSAGEPROTOCOL_SENDABLE_H
#define MESSAGEPROTOCOL_SENDABLE_H

#include "TCP.h"



/////////////////////////////////
#include "../DevKit/DevKit.h"

template <class DerivedSendable>
class Sendable : public Serializable<DerivedSendable> {
public:
    template<typename Recipient>
    void tb_sendto(Recipient receiver) {

        TCPHeader tcpHeader;
        std::string payload;

        payload = this->marshal();

        this->setHeader(tcpHeader, payload);

        if (!tcpHeader.payloadSplit) {

            this->transmitSendable(receiver->pipe.getWriteFd(), this->amalgamate(tcpHeader));






        } else {
            //Already know that this is a big message

            //Temporarily store total payload size since header will be updated
            int remainingPayloadSize = tcpHeader.payloadSize;

            //Size of the payload that will be split per raw Sendable
            const int splitSize = (PIPE_BUF - sizeof(TCPHeader) - sizeof(PartialHeader));

            //TCPHeader is already set, only needs to update the size of the payload
            this->setHeader(tcpHeader, splitSize);

            PartialHeader partialHeader;

            this->setHeader(partialHeader, remainingPayloadSize, splitSize);

            //all the headers are set at this point
            //TODO: CHECK IF THIS LOOP IS VALID?
            while(remainingPayloadSize > 0){
                //Change the payload size ONLY for the last raw Sendable
                if (tcpHeader.payloadSize < splitSize){
                    tcpHeader.payloadSize = remainingPayloadSize;
                }
                this->transmitSendable(receiver->pipe.getWriteFd(), this->amalgamate(partial, (partial.Sequence - 1)), tcpHeader.payloadSize);
                this->incrementSeq(partial);
                remainingPayloadSize =- splitSize;
            }
        }
    }



    /**
     * pure virtual method that NEEDS TO BE OVERRIDEN
     * @return type of the Sendable based on Enum (TCP_TYPE_LIST)
     */
    virtual int getSendableType() = 0;

private:

    void setHeader(TCPHeader header, std::string payload) {
        int payloadSize = payload.length();
        header.sendableType = this->getSendableType();
        header.payloadSize = payloadSize;
        header.payloadSplit = payloadSize + sizeof(header) > PIPE_BUF ? true : false;
    }

    void setHeader(TCPHeader header, int payloadSize){
        header.payloadSize = payloadSize;
    }

    void setHeader(PartialHeader partial, int payloadSize, int splitSize) {
        partial.ID = this->makeAUniqueID();

        partial.Sequence = 1;

        int lastPayloadSize = payloadSize % splitSize;

        if (lastPayloadSize) {
            partial.totalCount = (payloadSize / splitSize) + 1;
        } else {
            partial.totalCount = (payloadSize / splitSize);
        }
    }

    long int makeAUniqueID() {
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

    void transmitSendable(int targetFD, byte *payload, int payloadSize) {
        write(targetFD, payload, sizeof(TCPHeader) + payloadSize);
        delete[] payload;
    }



    void incrementSeq(PartialHeader partial){
        partial.Sequence =+ 1;
    }



    byte *amalgamate(TCPHeader header) {
        int headerSize = sizeof(TCPHeader);

        byte *concatenated = new unsigned char[headerSize + this->header.contentsSize];

        memcpy(concatenated, &this->header, headerSize);

        memcpy(concatenated + headerSize, this->payload, (size_t)this->header.contentsSize);

        return concatenated;
    }

    byte *amalgamate(PartialHeader partial, int payloadMarker) {
        int headerSize = sizeof(TCPHeader);

        byte *concatenated = new unsigned char[headerSize + sizeof(PartialHeader) + this->header.contentsSize];

        //adding TCP header first
        memcpy(concatenated, &this->header, headerSize);

        //Then adding partial header
        memcpy(concatenated + headerSize, &partial, sizeof(PartialHeader));

        //Adding the payload
        memcpy(concatenated + headerSize + sizeof(PartialHeader),
               this->payload + (payloadMarker * this->header.contentsSize),
               this->header.contentsSize + sizeof(PartialHeader) + this->header.contentsSize);

        return concatenated;
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