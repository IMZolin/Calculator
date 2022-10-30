cmake_minimum_required (VERSION 3.8)
project ("Calculator")

set(CMAKE_C_STANDARD 17)

add_executable(Calculator Calculator.cpp Calculator.h Operation.h)
