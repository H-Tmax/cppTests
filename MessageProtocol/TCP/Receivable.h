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

    virtual ~Receivable() {
        while(!receivedRawSendables.empty()) {
            delete[] receivedRawSendables.top()->serializedPayload;
            receivedRawSendables.pop();
        }
        delete[] payloadBuffer;
    }

    void tb_recv() {
        //Initialize buffer in case it still has old buffer values
        this->initializeBuffers();

        //Read header first
        read(this->pipe.getReadFd(), headerBuffer, sizeof(TCPHeader));

        //Get the payload size from the header
        int payloadSize = ((TCPHeader *) this->headerBuffer)->payloadSize;


        //Get the payload type from the header
        int payloadType = ((TCPHeader *) this->headerBuffer)->sendableType;

        bool isBigSendable = ((TCPHeader *) this->headerBuffer)->payloadSplit;

        std::shared_ptr<RawSendable> sashimi(new RawSendable());

        ///////////////////////////////////////////////////////
        //if (isBigSendable) -> handle big message


        ///////////////////////////////////////////////////////
        //WHEN IT IS A SINGLE SENDABLE

        //New buffer for the payload
        payloadBuffer = new unsigned char[payloadSize];

        //Read the payload
        read(this->pipe.getReadFd(), payloadBuffer, payloadSize);

        sashimi->sendableID = payloadType;
        sashimi->size = payloadSize;
        sashimi->serializedPayload = payloadBuffer;



        this->receivedRawSendables.push(sashimi);
//        delete[] payloadBuffer;
    }

    void initializeBuffers() {
        memset(&headerBuffer, 0, sizeof(TCPHeader));
        //TODO: DELETE HERE?
        //delete this->payloadBuffer;
    }

    Pipe pipe;
    byte headerBuffer[sizeof(TCPHeader)];
    byte* payloadBuffer;

    std::stack<std::shared_ptr<RawSendable>> receivedRawSendables;


};

#endif //MESSAGEPROTOCOL_RECEIVABLE_H
