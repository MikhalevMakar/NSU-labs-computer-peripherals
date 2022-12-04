cmake_minimum_required(VERSION 3.16.3)
project(Lab7Blas)
set(CMAKE_CXX_STANDARD 11)
#find_package(OPENBLAS REQUIRED)

add_executable(Lab7Blas main.cpp Matrix.h Matrix.cpp)

target_include_directories(Lab7Blas PUBLIC /opt/homebrew/Cellar/openblas/0.3.21/include)

#target_compile_options(Lab7Blas PUBLIC -I/opt/homebrew/Cellar/openblas/0.3.21/include)

target_link_libraries(Lab7Blas PUBLIC -L/opt/homebrew/Cellar/openblas/0.3.21/lib/ -lopenblas)
