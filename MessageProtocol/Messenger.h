#include "Pipe.h"
#include <iostream>

#ifndef MESSAGEPROTOCOL_MESSENGER_H
#define MESSAGEPROTOCOL_MESSENGER_H

#endif //MESSAGEPROTOCOL_MESSENGER_H


//same with typedef unsigned char byte;
using byte = unsigned char;

//Target & payload agnostic messaging



template<typename Receiver, typename MessageBody>
static void sendMessage(Receiver receiver, MessageBody messageBody) {
    //createMessage(messageBody);
    ////if the size is > 512: call
    ////
    //Message header creator
    ////reads the message type, and create header
    ////

    std::cout << "read fd: " << receiver->pipe.getReadFd() << std::endl;
    std::cout << "write fd: " << receiver->pipe.getWriteFd() << std::endl;
}

static void transmitMessage(){

}

//template<typename MessageBody>
//static Message createMessage(MessageBody messageBody) {
//
//}

struct Message {


};

struct BigMessage {


};

struct MessageHeader {
    int messageType;
    int messageLength;
    bool messageSplit;
};

struct partialMessage {
    //Using hashed value of the message's body
    long int messageID;
    int messageSequenceNumber;
    int splitCount;
};

class TestClassOne {
public:
    TestClassOne() : pipe() {
        //NOTHING TO SEE HERE
    }

    Pipe pipe;
};

class TestClassTwo {
public:
    TestClassTwo() : pipe() {
        //NOTHING TO SEE HERE
    }

    Pipe pipe;
};

