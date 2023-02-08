project(sstl_gtest)

find_package(GTest REQUIRED CONFIG)

file(GLOB_RECURSE src_files "src/*.cpp")

add_executable(sstl_gtest ${src_files})

target_link_libraries(sstl_gtest GTest::gtest GTest::gtest_main)

# if in gcc, ling pthread
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(sstl_gtest pthread)
endif()

option(DISABLE_THREAD_TERMINATION_TEST "Disable thread termination test" OFF)
if (DISABLE_THREAD_TERMINATION_TEST)
    message(STATUS "Disable thread termination test")
    add_definitions(-DDISABLE_THREAD_TERMINATION_TEST)
endif()
