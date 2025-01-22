cmake_minimum_required(VERSION 3.10)
project(MyProject C CXX)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

add_executable(MyExecutable main.c main.cpp)
