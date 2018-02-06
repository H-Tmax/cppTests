#include <iostream>
#include <sstream>
#include <map>
#include "DummyClasses.h"
#include "TCP/Sendable.h"
#include "TCP/TCP_TYPE_LIST.h"

int main() {

    DummyReceivable *receiver = new DummyReceivable();


    DummySendable *to_be_sent = new DummySendable();
    //TODO:OVERFLOW HANDLING?
    to_be_sent->b = 999999999;
    to_be_sent->c = 7777777777777777777;
    to_be_sent->d = 3333333;


    to_be_sent->tb_sendto(receiver);
    receiver->tb_recv();

//timer start
//    clock_t begin = clock();
//
//
//
//
//    //timer ends
//    clock_t end = clock();
//    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
//    std::cout << "time = " << elapsed_secs << std::endl;
//


//30025



    //DESERIALIZE TEST CODE
    RawSendable whichSashimi;
    whichSashimi = receiver->receivedRawSendables.top();
    receiver->receivedRawSendables.pop();

    DummySendable ds;

    std::stringbuf buf;
    buf.sputn((char *)receiver->payloadBuffer, whichSashimi.size);
    //buf.sputn((char *)writer->payload, 21);
    std::istream is(&buf);
    boost::archive::binary_iarchive iar(is, boost::archive::no_header);
    iar >> ds;

    std::cout << "reading " << ds.b << std::endl;
    std::cout << "reading " << ds.c << std::endl;
    std::cout << "reading " << ds.d << std::endl;



    return 0;
}






