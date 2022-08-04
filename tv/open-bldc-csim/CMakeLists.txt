cmake_minimum_required(VERSION 2.6.0)

project(motor-sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

include_directories(/home/s1000632/misc/dvs/interfaces/cpp)

find_package( BLAS REQUIRED )

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/gtest/build/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/gtest/googletest/include)

set(TEST_LIB_FILES libgtest.a
                   libgtest_main.a
                   pthread)

set(TEST_FILES test/test0.cpp)

set(SERIALIZE_TEST_FILES serialize/test_simple_file.cpp
                         serialize/test_serializer.cpp
                         serialize/test_deserializer.cpp
                         serialize/test_global_serializer.cpp
                         serialize/test_data_logging.cpp
                         serialize/data_logging.cpp)

set(CPP_FILES src/main.cpp
              src/controller.c
              src/dyn_model.c
              src/misc_utils.c
              src/switch.c)

include_directories(${BLAS_INCLUDE_DIR})

include_directories(./src)

include_directories(./external/nlohmann/include)

link_directories(/home/s1000632/gsl/lib)
include_directories(/home/s1000632/gsl/include)

add_executable(motor-sim ${CPP_FILES})
target_link_libraries(motor-sim gsl m blas ${BLAS_LIBRARIES})

add_executable(tests ${TEST_FILES})
target_link_libraries(tests ${TEST_LIB_FILES})

add_executable(test-serialize ${SERIALIZE_TEST_FILES})
target_link_libraries(test-serialize ${TEST_LIB_FILES})

