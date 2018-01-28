cmake_minimum_required(VERSION 3.8)
project(MessageProtocol)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Pipe.h DummyClasses.h Sendable.h)
add_executable(MessageProtocol ${SOURCE_FILES})