#ifndef MESSAGEPROTOCOL_RECEIVABLE_H
#define MESSAGEPROTOCOL_RECEIVABLE_H

#include <stack>
#include "TCP.h"
#include "Pipe.h"

class Receivable{
public:
    Receivable() : pipe() {
        //NOTHING TO SEE HERE
    }

    void tb_recv() {
        //Initialize buffer in case it still has old buffer values
        this->initializeBuffers();

        //Read header first
        read(this->pipe.getReadFd(), headerBuffer, sizeof(TCPHeader));

        //Get the payload size from the header
        int payloadSize = ((TCPHeader *) this->headerBuffer)->contentsSize;

        //Get the payload type from the header
        int payloadType = ((TCPHeader *) this->headerBuffer)->contentsType;

        bool isBigSendable = ((TCPHeader *) this->headerBuffer)->contentsSplit;

        RawSendable sashimi;

        ///////////////////////////////////////////////////////
        //if (isBigSendable) -> handle big message


        ///////////////////////////////////////////////////////
        //WHEN IT IS A SINGLE SENDABLE

        //New buffer for the payload
        payloadBuffer = new unsigned char[payloadSize];

        //Read the payload
        read(this->pipe.getReadFd(), payloadBuffer, payloadSize);

        sashimi.sendableID = payloadType;
        sashimi.size = payloadSize;
        sashimi.serializedPayload = payloadBuffer;

        this->receivedRawSendables.push(sashimi);
    }

    void initializeBuffers() {
        memset(&headerBuffer, 0, sizeof(TCPHeader));
        //TODO: DELETE HERE?
        delete this->payloadBuffer;
    }

    Pipe pipe;
    byte headerBuffer[sizeof(TCPHeader)];
    byte* payloadBuffer;

    std::stack<RawSendable> receivedRawSendables;


};

#endif //MESSAGEPROTOCOL_RECEIVABLE_H
