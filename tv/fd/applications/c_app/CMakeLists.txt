cmake_minimum_required(VERSION 2.6.0)

project(c_app)

set(CPP_SOURCE_FILES main.c)
include_directories(${CMAKE_SOURCE_DIR}/modules/c)

add_executable(c_app ${CPP_SOURCE_FILES})
# target_link_libraries(serialization_app logging)
