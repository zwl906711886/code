cmake_minimum_required(VERSION 3.20)
project(code)

set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cpp)

add_executable(compile Compiler.cpp)

add_executable(a a.cpp)


