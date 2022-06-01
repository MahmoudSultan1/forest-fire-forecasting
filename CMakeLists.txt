cmake_minimum_required(VERSION 3.16.3)

project(forest_forecast)

set(CMAKE_CXX_STANDARD 11)

add_executable(server serverRun.cpp)
add_executable(client clientRun.cpp)
add_executable(test test.cpp)

