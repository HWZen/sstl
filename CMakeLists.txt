cmake_minimum_required(VERSION 3.16)
project(MyStl)

set(CMAKE_CXX_STANDARD 20)

include_directories(include)

# fix clangd in clion no __cpp_concepts defined
option(ENABLE_CLANGD_FIXED "Enable Clangd Fixed" OFF)
if(ENABLE_CLANGD_FIXED)
    message(STATUS "Enable Clangd Fixed")
    add_definitions(-DENABLE_CLANGD_FIXED)
endif()

# if msvc add /W4
if(MSVC)
    message(STATUS "MSVC, add /W4")
    add_compile_options(/W4)
endif()

option(ENABLE_GTEST "Enable GTest" OFF)
if(ENABLE_GTEST)
    add_subdirectory(gtest)
endif()

option(ENABLE_SAMPLE "Enable Sample" OFF)
if(ENABLE_SAMPLE)
    add_subdirectory(sample)
endif()


