cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(arl-string CXX)

set (CMAKE_CXX_STANDARD 14)

# arl-string library
add_library(arl-string STATIC string.cpp)
