cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(arl-misc CXX)

set (CMAKE_CXX_STANDARD 14)

# arl-misc library
add_library(arl-misc STATIC misc.cpp time.cpp)
