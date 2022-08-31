cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(advanced-tests-cpp)

include_directories(${REPO_DIR}/src/interfaces/cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(advanced-tests-cpp main.cpp)
