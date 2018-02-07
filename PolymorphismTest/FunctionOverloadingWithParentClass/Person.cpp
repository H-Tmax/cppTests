//
// Created by hspark on 17. 11. 24.
//

#include "Person.h"

int Person::printAge() {
    std::cout << this->age << std::endl;
}
std::string Person::printName() {
    std::cout << this->name << std::endl;
    std::cout << "This is Person: This is the default" << std::endl;
}

int Student::printAge() {
    std::cout << this->age << std::endl;
}
std::string Student::printName() {
    std::cout << this->name << std::endl;
    std::cout << "This is Student" << std::endl;
}

int Teacher::printAge() {
    std::cout << this->age << std::endl;
}
std::string Teacher::printName() {
    std::cout << this->name << std::endl;
    std::cout << "This is Teacher" << std::endl;
}