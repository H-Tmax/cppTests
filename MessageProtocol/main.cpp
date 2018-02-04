#include <iostream>
#include <sstream>
#include "DummyClasses.h"


int main() {
//    std::string a = "hello world0101010";
//    std::hash<std::string> hash;
//    std::cout << hash(a) << std::endl;
//    //hash, then modulo

    DummySendable *writer = new DummySendable();
    writer->b = 777;
    writer->c = 333;
    DummyReceivable *receiver = new DummyReceivable();
//
//    writer->tb_sendto(receiver);
//    receiver->tb_recv(receiver);

//    std::stringbuf buf;
//    buf.sputn((char *)receiver->buffer, 17);
//    std::istream is(&buf);
//
//
//    DummySendable result;
//    boost::archive::binary_iarchive ia(is, boost::archive::no_header);
//    ia >> result;
//
//
//    std::cout << "reading " << result.b << std::endl;
//    std::cout << "reading " << result.c << std::endl;

    return 0;
}

