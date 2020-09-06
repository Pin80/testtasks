cmake_minimum_required(VERSION 3.5)

project(saber_test_task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(saber_test_task custom_list.h main.cpp)
