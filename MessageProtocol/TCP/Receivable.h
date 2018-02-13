#ifndef MESSAGEPROTOCOL_RECEIVABLE_H
#define MESSAGEPROTOCOL_RECEIVABLE_H

#include <deque>
#include <map>
#include "TCP.h"
#include "Pipe.h"

class Receivable{
public:
    Receivable() : pipe() {
        //NOTHING TO SEE HERE
    }

    virtual ~Receivable() {
        while(!receivedRawSendables.empty()) {
            delete[] receivedRawSendables.front()->serializedPayload;
            receivedRawSendables.pop_front();
        }

    }

    void tb_recv() {

        ///////////////////////////////////////////////
        ////////////////PRE PROCESSING/////////////////
        ///////////////////////////////////////////////
        byte headerBuffer[sizeof(TCPHeader)];
        byte* payloadBuffer;

        //Read header first
        read(this->pipe.getReadFd(), headerBuffer, sizeof(TCPHeader));

        //Get the payload size from the header
        int payloadSize = ((TCPHeader *) headerBuffer)->payloadSize;


        //Get the payload type from the header
        int payloadType = ((TCPHeader *) headerBuffer)->sendableType;

        bool isBigSendable = ((TCPHeader *) headerBuffer)->payloadSplit;

        RawSendable *sashimi = new RawSendable();
        ////////////////////////////////////////////////
        ////////////////////////////////////////////////
        ////////////////////////////////////////////////


        ///////////////////////////////////////////////////////
        //if (isBigSendable) -> handle big message


        ///////////////////////////////////////////////////////
        //WHEN IT IS A SINGLE SENDABLE

        //New buffer for the payload
        payloadBuffer = new char[payloadSize];

        //Read the payload
        read(this->pipe.getReadFd(), payloadBuffer, payloadSize);

        sashimi->sendableID = payloadType;
        sashimi->size = payloadSize;
        sashimi->serializedPayload = payloadBuffer;



        this->receivedRawSendables.push_back(sashimi);

//        TODO: THIS CAUSES RECEIVED SENDABLE BROKEN VAL, WHEN TO FREE THEN?
//        PROBABLY AFTER ALL SASHIMI'S HAVE BEEN COLLECTED INTO A RAWSENDABLE
//        delete[] payloadBuffer;
    }

    RawSendable* getRawSendable() {
        RawSendable* rs = this->receivedRawSendables.front();
        this->receivedRawSendables.pop_front();
        return rs;
    }

    int getReadingEnd(){
        return this->pipe.getReadFd();
    }

    int getWritingEnd(){
        return this->pipe.getWriteFd();
    }

private:
    Pipe pipe;
    std::deque<RawSendable*> receivedRawSendables;
    std::map<long int, std::deque<byte *>> splitSendableMap;


};

#endif //MESSAGEPROTOCOL_RECEIVABLE_H
