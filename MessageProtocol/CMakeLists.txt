cmake_minimum_required(VERSION 3.8)
project(MessageProtocol)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Messenger.h Pipe.h)
add_executable(MessageProtocol ${SOURCE_FILES})