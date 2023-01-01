cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(demos)

include_directories(${REPO_DIR}/src/externals/liquidfun/liquidfun/Box2D)

include_directories(${REPO_DIR}/src/interfaces/cpp)
include_directories(${REPO_DIR}/src/main_application)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${REPO_DIR}/src/externals/rapidxml)

add_executable(demos main.cpp
                     tests/spring/spring.cpp
                     tests/sphere/sphere.cpp
                     tests/sphere/sphere_subdivision/edge.cpp
                     tests/sphere/sphere_subdivision/face.cpp
                     tests/sphere/sphere_subdivision/point.cpp
                     tests/sphere/sphere_subdivision/subdivisor.cpp
                     ${REPO_DIR}/src/main_application/misc/color_map.cpp)

target_link_libraries(demos 
        ${REPO_DIR}/src/externals/liquidfun/liquidfun/Box2D/Box2D/Release/libliquidfun.a)
