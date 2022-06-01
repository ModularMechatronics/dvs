cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(arl-csv CXX)

set (CMAKE_CXX_STANDARD 14)

# arl-csv library
add_library(arl-csv SHARED csv.cpp)
target_link_libraries(arl-csv arl-textfile arl-string)
