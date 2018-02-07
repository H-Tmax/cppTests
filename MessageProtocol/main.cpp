#include <iostream>
#include <sstream>

#include "DummyClasses.h"
#include "TCP/Sendable.h"
#include "TCP/TCP_TYPE_LIST.h"
#include "DevKit.h"
#include "gtest/gtest.h"


TEST(Message, testSendRecv) {
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
    std::shared_ptr<RawSendable> whichSashimi(receiver->receivedRawSendables.top());
    receiver->receivedRawSendables.pop();

    DummySendable ds;

    std::stringbuf buf;
    buf.sputn((char *)receiver->payloadBuffer, whichSashimi->size);
    //buf.sputn((char *)writer->payload, 21);
    std::istream is(&buf);
    boost::archive::binary_iarchive iar(is, boost::archive::no_header);
    iar >> ds;

    PO("reading: ", ds.b, "\n");
    POL("reading: ", ds.c);
    std::cout << "reading: " << ds.d << std::endl;
    delete to_be_sent;
    delete receiver;
}

class DummySerializable : public Serializable<DummySerializable> {
public:
    std::vector<int> intlist;

    bool operator==(const DummySerializable &rhs) const {
        return intlist == rhs.intlist;
    }

    bool operator!=(const DummySerializable &rhs) const {
        return !(rhs == *this);
    }

    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & intlist;
    }
};

TEST(Message, testSerializeDeserialize) {
    DummySerializable original;
    original.intlist.push_back(111);
    original.intlist.push_back(2222);
    original.intlist.push_back(33333);

    std::string bufStr = original.toBytes();

    DummySerializable deserialized;
    deserialized.populateItsOwnData(bufStr);

    EXPECT_EQ(deserialized, original);
}
