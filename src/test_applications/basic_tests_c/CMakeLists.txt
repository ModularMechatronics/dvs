cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(basic-tests-c C)

include_directories(${REPO_DIR}/src/interfaces/c)

add_executable(basic-tests-c main.c)
target_link_libraries(basic-tests-c m)