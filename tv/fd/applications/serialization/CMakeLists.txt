cmake_minimum_required(VERSION 2.6.0)

project(serialization_app)

set(CPP_SOURCE_FILES main.cpp
                     functions.cpp)

add_executable(serialization_app main.cpp)
target_link_libraries(serialization_app logging)
