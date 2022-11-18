cmake_minimum_required(VERSION 3.23)

project(Lab6)
set(CMAKE_CXX_STANDARD 17)

find_package(Libusb1 QUIET)

add_executable(Lab6 main.cpp)

if(LIBUSB1_FOUND)
    message("LIBUSB FOUND")
    target_include_directories(Lab6 PUBLIC  ${LIBUSB1_INCLUDE_DIRS})
    target_link_libraries(Lab6 PUBLIC ${LIBUSB1_LIBRARIES})
    include_directories(Lab6 ${LIBUSB1_INCLUDE_DIRS})
else()
    message("LIBUSB NOT FOUND")
endif()

