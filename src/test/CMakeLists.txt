cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(duoplot_tests)

include_directories(${REPO_DIR}/src/interfaces/cpp/)

# test-duoplot executable
add_executable(test-duoplot test/test_vector.cpp)
target_link_libraries(test-duoplot ${GTEST_LIB_FILES})
