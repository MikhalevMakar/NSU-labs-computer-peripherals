cmake_minimum_required(VERSION 3.16.3)
project(Lab7Blase)

find_package(MKL REQUIRED)

add_executable(Lab7Blase main.cpp Matrix.h Matrix.cpp)

target_compile_options(Lab7Blase PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_COMPILE_OPTIONS>)

target_link_libraries(Lab7Blase PUBLIC $<LINK_ONLY:MKL::MKL>)
