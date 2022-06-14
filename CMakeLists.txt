cmake_minimum_required(VERSION 3.16)
project(MyStl)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

option(ENABLE_GTEST "Enable GTest" OFF)
if(ENABLE_GTEST)
    add_subdirectory(gtest)
endif()

option(ENABLE_SAMPLE "Enable Sample" OFF)
if(ENABLE_SAMPLE)
    add_subdirectory(sample)
endif()