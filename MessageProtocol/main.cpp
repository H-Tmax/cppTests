#include <iostream>
#include <sstream>


#include "DummyClasses.h"
#include "TCP/Sendable.h"
#include "TCP/TCP_TYPE_LIST.h"
#include "DevKit.h"





int main() {

    DummyReceivable *receiver = new DummyReceivable();


    DummySendable *to_be_sent = new DummySendable();
    //TODO:OVERFLOW HANDLING?
    to_be_sent->b = 999999999;
    to_be_sent->c = 7777777777777777777;
    to_be_sent->d = 3333333;


    START();

    to_be_sent->tb_sendto(receiver);
    receiver->tb_recv();

    END();


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

    PO("reading: ", ds.b, "\n");
    POL("reading: ", ds.c);
    std::cout << "reading: " << ds.d << std::endl;



    return 0;
}






