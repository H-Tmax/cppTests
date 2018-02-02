cmake_minimum_required(VERSION 3.8)
project(MessageProtocol)

set(CMAKE_CXX_STANDARD 11)

#BOOST LIBRARY INCLUDED
set(Boost_INCLUDE_DIR /home/hspark/DevWorks/boost_1_61_0)
link_directories(/home/hspark/DevWorks/boost_1_61_0/stage/lib)

include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES main.cpp Pipe.h DummyClasses.h TCP/TCP_TYPE_LIST.h TCP/TCP.h TCP/Serializable.h TCP/Sendable.h TCP/Receivable.h)

add_executable(MessageProtocol ${SOURCE_FILES})

target_link_libraries(MessageProtocol boost_serialization)