#ifndef MESSAGEPROTOCOL_DUMMYCLASSES_H
#define MESSAGEPROTOCOL_DUMMYCLASSES_H



#include "TCP/TCP.h"
#include "TCP/Sendable.h"
#include "TCP/Receivable.h"
#include <iostream>

class DummySendable : public Sendable {
public:
    DummySendable() {
        //NOTHING TO SEE HERE
    }

    void foo() {
        std::cout << "class A's foo" << std::endl;
    }

    int b;
    long int c;
    int d;

    /////////////////FOR BIGMESSAGE TESTING//////////////////
    //char hh[30000];
    /////////////////////////////////////////////////////////



    //DEV REQUIRED
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & b;
        ar & c;
        ar & d;
        /////////////////FOR BIGMESSAGE TESTING//////////////////
        //ar & hh;
        /////////////////////////////////////////////////////////

    }

    //DEV REQUIRED
    int getSendableType() {
        return DUMMY_SENDABLE;
    }

    //DEV REQUIRED
    void *getSendableInfo() {
        return run(this);
    }





};

class DummyReceivable : public Receivable {
public:
    DummyReceivable() {
        //NOTHING TO SEE HERE
    }

    void foo() {
        std::cout << "class A's foo" << std::endl;
    }



};


#endif //MESSAGEPROTOCOL_DUMMYCLASSES_H
