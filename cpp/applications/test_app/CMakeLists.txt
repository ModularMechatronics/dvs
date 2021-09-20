cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(test-app)

include_directories(${REPO_DIR}/interfaces/cpp)

add_executable(test-app main.cpp)
