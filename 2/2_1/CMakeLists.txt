cmake_minimum_required(VERSION 3.10)
project(MatVec LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)

add_executable(matvec main.cpp)

target_link_libraries(matvec OpenMP::OpenMP_CXX)

target_compile_options(matvec PRIVATE -O3 -march=native -DNDEBUG)