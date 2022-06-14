project(mystl_gtest)

find_package(GTest REQUIRED)

include_directories(${GTest_INCLUDE_DIRS})

file(GLOB_RECURSE src_files "src/*.cpp")

add_executable(mystl_gtest ${src_files})

target_link_libraries(mystl_gtest ${GTEST_BOTH_LIBRARIES})

# if in gcc, ling pthread
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(mystl_gtest pthread)
endif()
