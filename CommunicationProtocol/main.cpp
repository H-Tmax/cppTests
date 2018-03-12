#include <iostream>
#include <sstream>

#include "DummyClasses.h"
#include "TCP/Sendable.h"
#include "TCP/TCP_TYPE_LIST.h"
#include "DevKit/DevKit.h"
#include "gtest/gtest.h"


TEST(TCP, testSendRecv) {
    std::unique_ptr<DummyReceivable> receiver(new DummyReceivable());
    std::unique_ptr<DummySendable> to_be_sent(new DummySendable());
    //TODO:OVERFLOW HANDLING?
    to_be_sent->b = 999999999;
    to_be_sent->c = 7777777777777777777;
    to_be_sent->d = 3333333;

    to_be_sent->tb_sendto(*receiver.get());
    receiver->tb_recv();

    //DESERIALIZE TEST CODE
    std::unique_ptr<RawSendable> whichSashimi = receiver->getRawSendable();


    DummySendable received;
    received.unmarshal(whichSashimi->serializedPayload);

    POL("\nreceived b: ", received.b);
    POL("received c: ", received.c);
    POL("received d: ", received.d);
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
    void serialize(Archive &ar, const unsigned int version) {
        ar & intlist;
    }
};

TEST(TCP, testSerializeDeserialize) {
    DummySerializable original;
    original.intlist.push_back(111);
    original.intlist.push_back(2222);
    original.intlist.push_back(33333);

    std::string bufStr = original.marshal();

    DummySerializable deserialized;
    deserialized.unmarshal(bufStr);

    EXPECT_EQ(deserialized, original);
}

//HOW DO I TEST THIS SHIT?
TEST(TCP, testSendSome) {
    std::unique_ptr<DummyReceivable> receiver(new DummyReceivable());
    std::unique_ptr<DummySendable> to_be_sent(new DummySendable());

    to_be_sent->b = 999999999;
    to_be_sent->c = 7777777777777777777;
    to_be_sent->d = 3333333;

    LeftoverNonblock ln = to_be_sent->tb_sendto_some(*receiver.get());
    //TEST:check if this func returns

    EXPECT_EQ(ln.exitCodeFromLastWrite, 0);
    //EXPECT_EQ();
    //EXPECT_EQ();

    receiver->tb_recv();
}

TEST(TCP, testQuickSendable) {
    std::unique_ptr<DummyReceivable> receiver(new DummyReceivable());
    QuickSendable qs1(1012);
    QuickSendable qs2(std::string("Hello"));
    QuickSendable qs3(3.141592);
    QuickSendable qs4(true);

    qs1.tb_sendto(*receiver.get());
//    qs2.tb_sendto(receiver);
//    qs3.tb_sendto(receiver);
//    qs4.tb_sendto(receiver);


    int i;
    double d;
    bool b;
    std::string s;

    for (int j = 0; j < 4; j++) {
        receiver->tb_recv();
        std::unique_ptr<RawSendable> whichSashimi = receiver->getRawSendable();

        QuickSendable received;

        switch (whichSashimi->sendableID) {
            case TYPE_INT:
                i = received.getVal(whichSashimi->serializedPayload, i);
                break;
            case TYPE_DOUBLE:
                d = received.getVal(whichSashimi->serializedPayload, d);
                break;
            case TYPE_BOOL:
                b = received.getVal(whichSashimi->serializedPayload, b);
                break;
            case TYPE_STRING:
                s = received.getVal(whichSashimi->serializedPayload, s);
                break;
        }
    }

    POL("Received i: ", i);
    POL("Received d: ", d);
    POL("Received b: ", b);
    POL("Received s: ", s);

//    DummySendable received;
//    received.unmarshal(whichSashimi->serializedPayload);


}
