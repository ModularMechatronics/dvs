cmake_minimum_required(VERSION 2.6.0)

project(serializer_c)

set(SERIALIZER_TEST_FILES test/test_serialization.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(test_serializer_c ${SERIALIZER_TEST_FILES})
target_link_libraries(test_serializer_c ${TEST_LIB_FILES})