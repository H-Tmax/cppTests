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

    //DEV REQUIRED
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & b;
        ar & c;
    }


    //DEV REQUIRED
    int sendable_type() {
        return DUMMY_SENDABLE;
    }

    //DEV REQUIRED
    void *sendable_main() {
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
