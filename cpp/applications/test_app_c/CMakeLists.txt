cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(c-test-app C)

include_directories(${REPO_DIR}/interfaces/c)

add_executable(c-test-app main.c)
