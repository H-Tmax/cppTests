#include <iostream>
#include "Messenger.h"

int main() {
    TestClassOne tco = TestClassOne();
    TestClassTwo tct = TestClassTwo();


    sendMessage(&tco, NULL);
    sendMessage(&tct, NULL);

    std::string a = "hello world0101010";
    std::hash<std::string> hash;
    std::cout << hash(a) << std::endl;

    return 0;
}