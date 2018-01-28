#include <iostream>
#include "DummyClasses.h"

int main() {
    std::string a = "hello world0101010";
    std::hash<std::string> hash;
    std::cout << hash(a) << std::endl;
    //hash, then modulo


    DummyClassA dca = DummyClassA();
    DummyClassA dca2 = DummyClassA();

    dca.sendsTo(&dca2);

    return 0;
}

//would this be needed?
////appendsTo(SendableObject)