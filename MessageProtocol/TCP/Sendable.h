#ifndef MESSAGEPROTOCOL_SENDABLE_H
#define MESSAGEPROTOCOL_SENDABLE_H

#include "TCP.h"

template <class DerivedSendable>
class Sendable : public Serializable<DerivedSendable> {
public:
    TCPHeader header;
    byte *payload;

    template<typename Recipient>
    void tb_sendto(Recipient receiver) {
        int receiverFD = receiver->pipe.getWriteFd();

        //Initialize header in case it still has old header values
        this->initialize();

        //populate the header and the payload
        this->getSendableInfo();

        this->makeAUniqueID();


        if (!this->header.contentsSplit) {

            this->transmitSendable(receiverFD, this->amalgamate(this->header));

        } else {
            //Already know that this is a big message

            //this->setHeaders()
            //Temporarily store total payload size since header will be updated
            int remainedPayloadSize = this->header.contentsSize;

            //Size of the payload that will be split per raw Sendable
            int splitSize = (PIPE_BUF - sizeof(TCPHeader) - sizeof(PartialHeader));

            //TCPHeader is already set, only needs to update the size of the payload
            this->setHeader(splitSize);

            PartialHeader partial;
            this->initialize(partial);

            //all the headers are set at this point
            //TODO: CHECK IF THIS LOOP IS VALID?
            while(remainedPayloadSize > 0){
                //Change the payload size ONLY for the last raw Sendable
                if (this->header.contentsSize != splitSize){
                    this->header.contentsSize = remainedPayloadSize;
                }
                this->transmitSendable(receiverFD, this->amalgamate(partial, (partial.Sequence - 1)));
                this->incrementSeq(partial);
                remainedPayloadSize =- splitSize;
            }
        }
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
    virtual int getSendableType() = 0;

    virtual void *getSendableInfo() = 0;

private:
    void setHeader(int contentsSize){
        this->header.contentsSize = contentsSize;
    }

    void incrementSeq(PartialHeader partial){
        partial.Sequence =+ 1;
    }

    void initialize() {
        this->header.contentsType = 0;
        this->header.contentsSize = 0;
        this->header.contentsSplit = false;
        this->os.flush();
    }

    void initialize(PartialHeader partial) {
        partial.ID = this->makeAUniqueID();

        partial.Sequence = 1;

        int lastPayloadSize = this->header.contentsSize % this->header.contentsSize;

        if (lastPayloadSize) {
            partial.totalCount = (this->header.contentsSize / this->header.contentsSize) + 1;
        } else {
            partial.totalCount = (this->header.contentsSize / this->header.contentsSize);
        }
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

    void transmitSendable(int targetFD, byte *payload) {
        write(targetFD, payload, sizeof(TCPHeader) + this->header.contentsSize);
        delete[] payload;
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

};

//Covariant return type function to get "this" of a derived object
template<typename Derived>
void *run(Derived *derivedSendable) {
    //Serialize the contents (derived Sendable)
    derivedSendable->oar << *derivedSendable;

    //Get the size of the contents
    int totalPayloadSize = derivedSendable->buf.str().length();

    //Mark the header if the payload size is bigger than the size which the kernel's pipe can guarantee the atomicity
    bool isBigSendable = (totalPayloadSize + sizeof(TCPHeader)) > PIPE_BUF ? true : false;

    //Set the Sendable header properties
    derivedSendable->setHeader(derivedSendable->getSendableType(), totalPayloadSize, isBigSendable);

    //Set the payload, which is a byte pointer of the serialized object
    derivedSendable->setPayload((byte *) derivedSendable->buf.str().c_str());
}

#endif //MESSAGEPROTOCOL_SENDABLE_H