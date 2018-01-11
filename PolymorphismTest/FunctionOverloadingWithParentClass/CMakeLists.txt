cmake_minimum_required(VERSION 3.8)
project(PolymorphismTest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Person.cpp Person.h Tests.cpp Tests.h)
add_executable(PolymorphismTest ${SOURCE_FILES})