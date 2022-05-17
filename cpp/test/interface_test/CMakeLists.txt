cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(interface-test)

include_directories(${REPO_DIR}/interfaces/cpp)
# include_directories(${REPO_DIR}/cpp/dvs/math)

add_executable(interface-test test_interface_functions.cpp)
target_link_libraries(interface-test ${GTEST_LIB_FILES})
