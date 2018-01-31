#include <iostream>
#include <sstream>
#include "DummyClasses.h"


int main() {
//    std::string a = "hello world0101010";
//    std::hash<std::string> hash;
//    std::cout << hash(a) << std::endl;
//    //hash, then modulo


    DummySendable *writer = new DummySendable();
    DummyReceivable *receiver = new DummyReceivable();

    writer->tb_sendto(receiver);
    receiver->tb_recv(receiver);
    std::cout << ((TCPHeader *) receiver->buffer)->contentsSize << std::endl;
    std::cout << ((TCPHeader *) receiver->buffer)->contentsSplit << std::endl;



//    byte * serialized_writer = reinterpret_cast<byte *>(writer);
//    std::cout << serialized_writer << std::endl;
//
//    DummySendable * deserialized_writer = reinterpret_cast<DummySendable *>(serialized_writer);
//    deserialized_writer->foo();








    ////////////////////serialization test

    return 0;
}

