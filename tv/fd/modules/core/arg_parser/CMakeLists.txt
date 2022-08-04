cmake_minimum_required(VERSION 2.6.0)

project(arg_parser)

add_library(arg_parser STATIC arg_parser.cpp)

add_executable(test_arg_parser test/test_arg_parser.cpp)
target_link_libraries(test_arg_parser ${TEST_LIB_FILES} arg_parser logging)
