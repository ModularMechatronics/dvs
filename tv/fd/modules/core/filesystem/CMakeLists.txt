cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(arl-filesystem CXX)

set (CMAKE_CXX_STANDARD 14)

# arl-filesystem library
add_library(arl-filesystem SHARED filesystem.cpp)
target_link_libraries(arl-filesystem ${Boost_LIBRARIES})
