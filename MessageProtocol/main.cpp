#include <iostream>



#include "DummyClasses.h"


//
//class Serializable {
//public:
//    Serializable(){}
//    virtual ~Serializable(){}
//
//    virtual void serialize(std::ostream& stream);
//    virtual void deserialize(std::istream& stream);
//};


int main() {
    std::string a = "hello world0101010";
    std::hash<std::string> hash;
    std::cout << hash(a) << std::endl;
    //hash, then modulo


    DummyClassA dca = DummyClassA();
    DummyClassA dca2 = DummyClassA();

    dca.sendsTo(&dca2);

    ////////////////////serialization test



    return 0;
}

//would this be needed?
////appendsTo(SendableObject)
//
//int main()
//{
//    using namespace mydata;
//    MyData data { "this is a name", "this is a type", boost::make_shared<MyInfo>("this is info") };
//
//    std::ostringstream oss;
//    {
//        boost::archive::text_oarchive oa(oss);
//        oa << data;
//    }
//
//    std::istringstream iss(oss.str());
//    {
//        boost::archive::text_iarchive ia(iss);
//        ia >> data;
//    }
//}