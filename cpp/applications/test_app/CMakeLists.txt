cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(test-app)

include_directories(${REPO_DIR}/cpp/dvs)

add_executable(test-app main.cpp another_file.cpp)
