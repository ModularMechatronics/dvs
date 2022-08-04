cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(textfile CXX)

set (CMAKE_CXX_STANDARD 14)

# textfile library
add_library(textfile SHARED textfile.cpp)
target_link_libraries(textfile arl-string arl-filesystem)
