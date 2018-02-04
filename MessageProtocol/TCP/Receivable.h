#ifndef MESSAGEPROTOCOL_RECEIVABLE_H
#define MESSAGEPROTOCOL_RECEIVABLE_H

#include "TCP.h"

class Receivable{
public:
    template<typename Reader>
    void tb_recv(Reader receiver) {
        //Initialize buffer in case it still has old buffer values
        this->initializeBuffer();

        //read(receiver->pipe.getReadFd(), &buffer, sizeof(TCPHeader));

        read(receiver->pipe.getReadFd(), buffer, sizeof(TCPHeader));

        int sssssize = ((TCPHeader *) receiver->buffer)->contentsSize;

        std::cout << sssssize << std::endl;

        std::cout << ((TCPHeader *) receiver->buffer)->contentsSplit << std::endl;

        read(receiver->pipe.getReadFd(), buffer, sssssize);



//        std::cout << ((TCPHeader *) buffer)->contentsSize << std::endl;
//        std::cout << ((TCPHeader *) buffer)->contentsSplit << std::endl;
//        std::cout << ((TCPHeader *) buffer)->contentsType << std::endl;

        //receiver->partialMessageQueue.enque();
        //read SendableHeader only when contentsSplit != true

    }


    byte buffer[512];

    void initializeBuffer() {
        memset(&buffer, 0, sizeof(buffer));
    }
    //private helper methods
};



#endif //MESSAGEPROTOCOL_RECEIVABLE_H
