cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(dvs_tests)

include_directories(${REPO_DIR}/cpp/dvs)
include_directories(${REPO_DIR}/cpp/math)

# test-stockdb executable
add_executable(test-dvs test/test_attributes.cpp
                        test/test_function_header.cpp
                        test/test_new.cpp)
target_link_libraries(test-dvs ${GTEST_LIB_FILES})
