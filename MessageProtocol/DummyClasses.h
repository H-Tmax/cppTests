#ifndef MESSAGEPROTOCOL_DUMMYCLASSES_H
#define MESSAGEPROTOCOL_DUMMYCLASSES_H


#include "Pipe.h"
#include "TCP/TCP.h"
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

private:
    //DEV REQUIRED
    int sendable_type() {
        return 0;
    }

    //DEV REQUIRED
    void *sendable_main() {
        return run(this);
    }





};

class DummyReceivable : public Receivable {
public:
    DummyReceivable() : pipe() {
        //NOTHING TO SEE HERE
    }

    void foo() {
        std::cout << "class A's foo" << std::endl;
    }

    Pipe pipe;
};


#endif //MESSAGEPROTOCOL_DUMMYCLASSES_H
