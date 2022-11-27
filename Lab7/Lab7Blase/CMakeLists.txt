cmake_minimum_required(VERSION 3.16.3)
project(Lab7Blase)
add_executable(Lab7Blase main.cpp Matrix.h Matrix.cpp)

find_package(MKL CONFIG)
if (${MKL_FOUND})
    message(STATUS "found BLAS")

    target_include_directories(Lab7Blase PUBLIC $<TARGET_PROPERTY:MKL::MKL,INTERFACE_INCLUDE_DIRECTORIES>)
    target_link_libraries(Lab7Blase PUBLIC $<LINK_ONLY:MKL::MKL>)
endif()