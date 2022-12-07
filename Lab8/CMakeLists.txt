cmake_minimum_required(VERSION 3.16.3)
project(Lab8)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O1 -g")
set(CMAKE_CXX_STANDARD 17)

add_executable(Lab8 main.cpp
                    Matrix.h
                    Matrix.cpp)