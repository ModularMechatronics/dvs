cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(hello_c C)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(hello_c hello_c/hello_c)
target_link_libraries(hello_c m)
target_include_directories(hello_c PUBLIC ${REPO_DIR}/src/interfaces/c)

project(hello_cpp CXX)

add_executable(hello_cpp hello_cpp/hello_cpp.cpp)
target_include_directories(hello_cpp PUBLIC ${REPO_DIR}/src/interfaces/cpp)

project(some_separate_app CXX)

add_executable(some_separate_app gui_backend/gui_backend.cpp)
target_include_directories(some_separate_app PUBLIC ${REPO_DIR}/src/interfaces/cpp)
