//
// Created by hspark on 17. 11. 24.
//

#ifndef POLYMORPHISMTEST_PERSON_H
#define POLYMORPHISMTEST_PERSON_H
#include <string>
#include <iostream>

class Person {
public:

    virtual std::string printName() = 0;
    virtual int printAge() = 0;

protected:
    Person(std::string _name, int _age) : name(_name), age(_age){}
    std::string name;
    int age;
};

class Student : public Person {
public:
    Student(std::string _name, int _age) : Person(_name, _age){}
    std::string printName();
    int printAge();
};

class Teacher : public Person {
public:
    Teacher(std::string _name, int _age) : Person(_name, _age){}
    std::string printName();
    int printAge();
};


#endif //POLYMORPHISMTEST_PERSON_H
