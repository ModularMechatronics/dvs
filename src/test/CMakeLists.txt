cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(duoplot_tests)

include_directories(${REPO_DIR}/src/interfaces/cpp/)
include_directories(${REPO_DIR}/src/main_application/)

# test-duoplot executable
# add_executable(test-duoplot test/test_vector.cpp)
# target_link_libraries(test-duoplot ${GTEST_LIB_FILES})

add_executable(test-number-formatting number_formatting/number_formatting_test.cpp
                                      ../main_application/misc/number_formatting.cpp)
target_link_libraries(test-number-formatting ${GTEST_LIB_FILES})
