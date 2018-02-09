#include <iostream>
#include <sstream>

#include "DummyClasses.h"
#include "TCP/Sendable.h"
#include "TCP/TCP_TYPE_LIST.h"
#include "DevKit/DevKit.h"
#include "gtest/gtest.h"


TEST(TCP, testSendRecv) {
    DummyReceivable *receiver = new DummyReceivable();
    DummySendable *to_be_sent = new DummySendable();
    //TODO:OVERFLOW HANDLING?
    to_be_sent->b = 999999999;
    to_be_sent->c = 7777777777777777777;
    to_be_sent->d = 3333333;

    to_be_sent->tb_sendto(receiver);
    receiver->tb_recv();

    //DESERIALIZE TEST CODE
    std::shared_ptr<RawSendable> whichSashimi(receiver->receivedRawSendables.top());
    receiver->receivedRawSendables.pop();

    DummySendable received;
    std::string byteArray((char *)whichSashimi->serializedPayload, whichSashimi->size);
    received.unmarshal(byteArray);

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
