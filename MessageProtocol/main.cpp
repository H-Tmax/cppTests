#include <iostream>
#include <sstream>
#include "DummyClasses.h"


///////REMOVE THIS INCLUDE AFTER THE TEST
#include "TCP/TCP.h"

int main() {

    DummySendable *writer = new DummySendable();
    writer->b = 777;
    writer->c = 333;
    DummyReceivable *receiver = new DummyReceivable();

    writer->tb_sendto(receiver);
    receiver->tb_recv();

    DummySendable result;

//    std::cout << "reading " << result.b << std::endl;
//    std::cout << "reading " << result.c << std::endl;

    return 0;
}

