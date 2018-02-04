#ifndef MESSAGEPROTOCOL_RECEIVABLE_H
#define MESSAGEPROTOCOL_RECEIVABLE_H

#include "TCP.h"
#include "Pipe.h"

class Receivable {
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

        //New buffer for the payload
        payloadBuffer = new unsigned char[payloadSize];

        //Read the payload
        read(this->pipe.getReadFd(), payloadBuffer, payloadSize);

        //deserialize the contents
        std::stringbuf buf;
        buf.sputn((char *)this->payloadBuffer, payloadSize);
        std::istream is(&buf);
        boost::archive::binary_iarchive iar(is, boost::archive::no_header);

        //iar >> s;



    }

    void initializeBuffers() {
        memset(&headerBuffer, 0, sizeof(TCPHeader));
        delete this->payloadBuffer;
    }

    Pipe pipe;
    byte headerBuffer[sizeof(TCPHeader)];
    byte* payloadBuffer;

};


#endif //MESSAGEPROTOCOL_RECEIVABLE_H
