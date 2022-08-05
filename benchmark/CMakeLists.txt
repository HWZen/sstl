project(benchmark)

add_executable(qsortBenchmark qsortBenchmark.cpp)
add_executable(listBenchMark listBenchMark.cpp)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Compiling with GCC")
    message(STATUS "link pthread")
    target_link_libraries(qsortBenchmark pthread)
endif()