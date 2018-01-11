cmake_minimum_required(VERSION 3.8)
project(DoubleDispatchTest)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp EventTask.cpp EventTask.h)
add_executable(DoubleDispatchTest ${SOURCE_FILES})