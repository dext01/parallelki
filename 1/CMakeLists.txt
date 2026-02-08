cmake_minimum_required(VERSION 3.10)
project(SineSum LANGUAGES CXX)

option(USE_DOUBLE "Использовать double вместо float" ON)

if(USE_DOUBLE)
    add_compile_definitions(USE_DOUBLE)
    message(STATUS "Тип данных: double")
else()
    message(STATUS "Тип данных: float")
endif()

add_executable(sine_sum main.cpp)

target_compile_options(sine_sum PRIVATE -O3)
