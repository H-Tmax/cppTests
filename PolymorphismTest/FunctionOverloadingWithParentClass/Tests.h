//
// Created by hspark on 17. 11. 29.
//

#ifndef POLYMORPHISMTEST_TESTS_H
#define POLYMORPHISMTEST_TESTS_H

#include <iostream>
#include "Person.h"

class Tests {
public:
    void testFunc(Person* in){
        std::cout << "PERSON TYPE" << std::endl;
    }
    void testFunc(Teacher* in){
        std::cout << "TEACHER TYPE" << std::endl;
    }
    void testFunc(Student* in){
        std::cout << "STUDENT TYPE" << std::endl;
    }

};


#endif //POLYMORPHISMTEST_TESTS_H
