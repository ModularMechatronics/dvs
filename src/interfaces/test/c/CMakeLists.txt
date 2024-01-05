cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(interface-test-c C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(interface-test-c main.c)
target_link_libraries(interface-test-c m)
target_include_directories(interface-test-c PUBLIC ${REPO_DIR}/src/interfaces/c)
