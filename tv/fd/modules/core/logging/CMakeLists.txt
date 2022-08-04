cmake_minimum_required(VERSION 2.6.0)

project(logging)

add_library(logging STATIC logging.cpp)

add_executable(test_logging test/test_logging.cpp)
target_link_libraries(test_logging ${TEST_LIB_FILES} logging)
