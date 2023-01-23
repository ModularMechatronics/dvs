cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(dvs_tests)

include_directories(${REPO_DIR}/src/interfaces/cpp/)

# test-dvs executable
add_executable(test-dvs test/test_vector.cpp)
target_link_libraries(test-dvs ${GTEST_LIB_FILES})
