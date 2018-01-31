#ifndef MESSAGEPROTOCOL_DUMMYCLASSES_H
#define MESSAGEPROTOCOL_DUMMYCLASSES_H

#endif //MESSAGEPROTOCOL_DUMMYCLASSES_H

#include "Pipe.h"
#include "TCP.h"
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

private:
    //DEV REQUIRED
    int sendable_type(){
        return DUMMY_SENDABLE;
    }

    //DEV REQUIRED
    void *sendable_main(){
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
