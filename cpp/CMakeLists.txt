cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(dvs CXX)

set(REPO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings -Wno-deprecated \
    -Wall -Wextra -pedantic -fdiagnostics-color")

# if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++17")
# endif()

include_directories(${REPO_DIR}/cpp/)

# Googletest
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/googletest/build/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/googletest/googletest/include)

# Nlohmann
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/externals/nlohmann/include)

# ****** wxWidgets ******
find_package(wxWidgets COMPONENTS core base gl adv)
include_directories(${wxWidgets_INCLUDE_DIRS})
link_directories(${wxWidgets_LIBRARY_DIRS})
include(${wxWidgets_USE_FILE})

set(GTEST_LIB_FILES libgtest.a
                    libgtest_main.a
                    pthread)

# ****** OpenGL ******
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

# Adding subdirectories
add_subdirectory(applications/basic_udp)
add_subdirectory(applications/test_app)
add_subdirectory(dvs)
add_subdirectory(main_application)
