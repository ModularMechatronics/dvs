cmake_minimum_required(VERSION 2.6.0)

project(log_to_file)

add_library(log_to_file STATIC log_to_file.cpp)

add_executable(test_log_to_file test/test_log_to_file.cpp)
target_link_libraries(test_log_to_file ${TEST_LIB_FILES} log_to_file)
