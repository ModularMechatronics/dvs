cmake_minimum_required(VERSION 3.14.3 FATAL_ERROR)

project(demos)

include_directories(${REPO_DIR}/src/externals/liquidfun/liquidfun/Box2D)

include_directories(${REPO_DIR}/src/externals/reactphysics3d/include)

# DSO
# include_directories(${REPO_DIR}/src/externals/dso/src)
# include_directories(${REPO_DIR}/src/externals/dso/thirdparty/Sophus)
# find_package(Boost COMPONENTS system thread REQUIRED) 
# include_directories(${Boost_INCLUDE_DIRS}) 
# find_package(Eigen3 REQUIRED)
# include_directories(${EIGEN3_INCLUDE_DIR})
# find_package(OpenCV QUIET)

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
                     tests/joints/joints.cpp
                     tests/boxes/boxes.cpp
                     tests/slam/slam.cpp
                     tests/particle_field/particle_field.cpp
                     tests/ad_dataset/ad_dataset.cpp
                     ${REPO_DIR}/src/main_application/misc/color_map.cpp)


# export OpenCV_DIR=/usr/local/Cellar/opencv@3/3.4.16_9
# export pangolin_DIR=/Users/danielpi/work/dvs/src/externals/pangolin
# pangolin_DIR=/usr/local/include/pangolin
# export PANGOLIN_DIR=/Users/danielpi/work/dvs/src/externals/pangolin
# export PANGOLIN_DIR=/Users/danielpi/work/dvs/src/externals/pangolin/cmake


target_link_libraries(demos 
        ${REPO_DIR}/src/externals/liquidfun/liquidfun/Box2D/Box2D/Release/libliquidfun.a
        ${REPO_DIR}/src/externals/reactphysics3d/build/libreactphysics3d.a
        # ${REPO_DIR}/src/externals/dso/build/lib/libdso.a
        # ${OpenCV_LIBS}
        # Boost::thread
        -lncurses)
