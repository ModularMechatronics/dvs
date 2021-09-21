cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(basic-tests-cpp)

include_directories(${REPO_DIR}/interfaces/cpp)

add_executable(basic-tests-cpp main.cpp)
