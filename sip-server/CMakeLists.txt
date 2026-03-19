cmake_minimum_required(VERSION 3.10)
project(sip-server-cpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(SOURCES 
    src/main.cpp 
    src/net/UdpServer.cpp
    src/sip/RequestLine.cpp
)
add_executable(sip-server ${SOURCES})
target_include_directories(sip-server PRIVATE include)