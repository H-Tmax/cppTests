#ifndef MESSAGEPROTOCOL_SENDABLE_H
#define MESSAGEPROTOCOL_SENDABLE_H

#endif //MESSAGEPROTOCOL_SENDABLE_H


#include <iostream>

//same with typedef unsigned char byte;
using byte = unsigned char;


//read SendableHeader only when contentsSplit != true
//
struct SendableHeader {
    int contentsType;
    int contentsLength;
    bool contentsSplit;
};


struct PartialContentsHeader {
    //Using hashed value of the message's body
    long int messageID;
    int messageSequenceNumber;
    int splitCount;
};



//Target & payload agnostic messaging

class Sendable {
public:
    template<typename Recipient>
    void sendsTo(Recipient receiver) {
        //createMessage(messageBody);
        ////if the size is > 512: call
        ////
        //Message header creator
        ////reads the message type, and create header
        ////

        std::cout << "read fd: " << receiver->pipe.getReadFd() << std::endl;
        std::cout << "write fd: " << receiver->pipe.getWriteFd() << std::endl;
    }

    //whatabout the reading end? do they inherit sendable as well?
    Sendable getSendable() {

    }

private:
    //private helper methods
    SendableHeader sh;
};


