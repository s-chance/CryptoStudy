cmake_minimum_required(VERSION 3.10)
project(DES)

set(CMAKE_CXX_STANDARD 11)

add_executable(DES main.cpp des.cpp utils.cpp)