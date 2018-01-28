#ifndef MESSAGEPROTOCOL_DUMMYCLASSES_H
#define MESSAGEPROTOCOL_DUMMYCLASSES_H

#endif //MESSAGEPROTOCOL_DUMMYCLASSES_H

#include "Pipe.h"
#include "Sendable.h"
#include <iostream>

class DummyClassA : public Sendable {
private:


public:
    DummyClassA() : pipe() {
        //NOTHING TO SEE HERE
    }

    void foo() {
        std::cout << "class A's foo" << std::endl;
    }

//    template<typename Recipient>
//    void sendsTo(Recipient receiver) {
//        std::cout << "ooops, overriden!" << std::endl;
//    }

    Pipe pipe;
};
