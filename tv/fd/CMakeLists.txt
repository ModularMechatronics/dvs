cmake_minimum_required(VERSION 2.6.0)

project(motor-sim)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE STRING "" FORCE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/modules/)

link_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/gtest/build/lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/gtest/googletest/include)

include_directories(./external/nlohmann/include)

set(TEST_LIB_FILES libgtest.a
                   libgtest_main.a
                   pthread)

set(TYPES_TEST_FILES modules/core/types/test/test_type.cpp
                     modules/core/types/test/test_type_ptr.cpp)

set(RANDOM_TEST_FILES modules/core/random/test/test_random.cpp)

set(MATH_TEST_FILES modules/core/math/test/test_math.cpp)

add_subdirectory(applications/serialization)
add_subdirectory(applications/c_app)

add_subdirectory(modules/core/logging)
add_subdirectory(modules/core/log_to_file)
add_subdirectory(modules/core/arg_parser)
add_subdirectory(modules/core/serializer)

add_subdirectory(modules/c/serialization)

add_executable(test_types ${TYPES_TEST_FILES})
target_link_libraries(test_types ${TEST_LIB_FILES})

add_executable(test_random ${RANDOM_TEST_FILES})
target_link_libraries(test_random ${TEST_LIB_FILES})

add_executable(test_math ${MATH_TEST_FILES})
target_link_libraries(test_math ${TEST_LIB_FILES})
