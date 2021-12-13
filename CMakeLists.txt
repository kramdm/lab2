cmake_minimum_required(VERSION 3.20)
project(lab2)

add_subdirectory(googletest-release-v1.11.0)
include_directories(googletest-release-v1.11.0/googletest/include/gtest)

add_executable(lab2 main.cpp)
target_link_libraries(lab2 gtest gtest_main)


set(CMAKE_CXX_STANDARD 17)
