cmake_minimum_required(VERSION 3.22)
project(Proj1)

set(CMAKE_CXX_STANDARD 14)

add_executable(Proj1 main.cpp CodeFiles/server.cpp CodeFiles/client.cpp)
