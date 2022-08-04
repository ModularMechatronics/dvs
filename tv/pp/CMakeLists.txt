cmake_minimum_required(VERSION 2.6.0)

project(pp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

include_directories(/home/s1000632/misc/dvs/interfaces/cpp)

set(CPP_FILES main.cpp)

add_executable(pp ${CPP_FILES})
# target_link_libraries(motor-sim gsl m blas ${BLAS_LIBRARIES})

