cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(basic-udp)

add_executable(udp-client main_client.cpp another_file.cpp)
add_executable(udp-server main_server.cpp)
# target_link_libraries(basic-udb )
