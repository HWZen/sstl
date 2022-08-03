project(atomicQueueSample)

add_executable(atomicQueueSample atomicQueueSample.cpp)
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message(STATUS "Compiling with GCC")
    message(STATUS "link pthread")
    target_link_libraries(atomicQueueSample pthread)
endif()