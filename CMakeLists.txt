cmake_minimum_required(VERSION 3.14)
project(IOScheduler)

set(CMAKE_CXX_STANDARD 14)

add_executable(IOScheduler main.cpp IO_Request.cpp)