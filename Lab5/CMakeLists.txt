cmake_minimum_required(VERSION 3.23)
project(Lab5)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable(Lab5 main.cpp)

target_link_libraries(Lab5 ${OpenCV_LIBS} )