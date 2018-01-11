#include <iostream>
#include <vector>

#include "Person.h"
#include "Tests.h"

int main() {
    Student *s = new Student("Stu", 10);
    Teacher *t = new Teacher("Tim", 20);

    Person *p1 = new Student("Stan", 15);
    Person *p2 = new Teacher("Tad", 20);

    Tests tests;
    tests.testFunc(s);
    tests.testFunc(t);
    tests.testFunc(p1);
    tests.testFunc(p2);

    return 0;
}
