cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

project(main-app)

include_directories(${REPO_DIR}/src/interfaces/cpp)

set(CPP_SOURCE_FILES main.cpp
                    udp_server.cpp)

add_executable(fast-udp ${CPP_SOURCE_FILES})
