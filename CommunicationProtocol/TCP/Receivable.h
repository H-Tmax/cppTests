#ifndef MESSAGEPROTOCOL_RECEIVABLE_H
#define MESSAGEPROTOCOL_RECEIVABLE_H

#include <deque>
#include <map>
#include "TCP.h"
#include "Pipe.h"

class Receivable {
public:

    void test_receive_for_nonblocking(){
        byte * buffer;
        //min size?
        fcntl(this->pipe.getReadFd(), F_SETPIPE_SZ, 5);
        //Any blocking write bigger than 1byte will cause a hang
        read(this->pipe.getReadFd(), buffer, 1);

    }

    void tb_recv() {
        ///////////////////////////////////////////////
        ////////////////PRE PROCESSING/////////////////
        ///////////////////////////////////////////////
        byte headerBuffer[sizeof(TCPHeader)];
        byte *payloadBuffer;
        TCPHeader *tempHeader;

        //Read header first
        read(this->pipe.getReadFd(), headerBuffer, sizeof(TCPHeader));

        //Temporarily store the header
        tempHeader = ((TCPHeader *) headerBuffer);

        //Get the payload type from the header
        int sendableType = tempHeader->sendableType;

        //Get the payload size from the header
        int payloadSize = tempHeader->payloadSize;

        //Check if the Sendable is split
        bool isSplit = tempHeader->payloadSplit;

        //New buffer for the payload
        payloadBuffer = new char[payloadSize];

        //Make an empty Raw Sendable
        RawSendable *sashimi = new RawSendable();

        ///////////////////////////////////////////////
        /////////////PROCESSING SENDABLE///////////////
        ///////////////////////////////////////////////
        if (!isSplit) {
            //If it is a single sendable, simply read the payload, then populate the raw sendable, then push it
            read(this->getReadingEnd(), payloadBuffer, payloadSize);

            sashimi->sendableID = sendableType;
            sashimi->serializedPayload = std::string(reinterpret_cast<const char *>(payloadBuffer), payloadSize);

            this->receivedRawSendables.push_back(sashimi);
        } else {
            //TODO: receive split sendables
            //this is the fun part
        }



//        TODO: THIS CAUSES RECEIVED SENDABLE BROKEN VAL, WHEN TO FREE THEN?
//        PROBABLY AFTER ALL SASHIMI'S HAVE BEEN COLLECTED INTO A RAWSENDABLE
//        delete[] payloadBuffer;
    }

    RawSendable *getRawSendable() {
        RawSendable *rs = this->receivedRawSendables.front();
        this->receivedRawSendables.pop_front();
        return rs;
    }

    int getReadingEnd() {
        return this->pipe.getReadFd();
    }

    int getWritingEnd() {
        return this->pipe.getWriteFd();
    }

    Receivable() : pipe() {
        //NOTHING TO SEE HERE
    }

    virtual ~Receivable() {
        while (!receivedRawSendables.empty()) {
            //TODO: THIS CASUES TYPE ERROR, WHEN TO FREE THEN?
            //delete receivedRawSendables.front()->serializedPayload;
            receivedRawSendables.pop_front();
        }
    }

private:
    Pipe pipe;
    std::deque<RawSendable *> receivedRawSendables;
    std::map<long int, std::deque<byte *>> splitSendableMap;
};

#endif //MESSAGEPROTOCOL_RECEIVABLE_H