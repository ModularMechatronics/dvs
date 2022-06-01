cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(arl-color-map CXX)

# arl-color-map library
add_library(arl-color-map STATIC color_map.cpp)
