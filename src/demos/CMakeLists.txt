cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(demos)

include_directories(${REPO_DIR}/src/interfaces/cpp)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(demos main.cpp
                     tests/spring/spring.cpp
                     tests/sphere/sphere.cpp
                     tests/sphere/sphere_subdivision/edge.cpp
                     tests/sphere/sphere_subdivision/face.cpp
                     tests/sphere/sphere_subdivision/point.cpp
                     tests/sphere/sphere_subdivision/subdivisor.cpp)
