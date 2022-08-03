project(sstl_gtest)

find_package(GTest REQUIRED)

file(GLOB_RECURSE src_files "src/*.cpp")

add_executable(sstl_gtest ${src_files})

target_link_libraries(sstl_gtest ${GTEST_BOTH_LIBRARIES})

# if in gcc, ling pthread
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(sstl_gtest pthread)
endif()
