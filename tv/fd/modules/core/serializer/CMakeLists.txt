cmake_minimum_required(VERSION 2.6.0)

project(serializer)

add_library(serializer STATIC serializer.cpp
                              global_file.cpp)
target_link_libraries(serializer logging)

set(SERIALIZER_TEST_FILES test/test_serializer.cpp
                          test/test_instrumentation.cpp
                          test/test_global_file.cpp
                          test/test_simple_file.cpp)

add_executable(test_serializer ${SERIALIZER_TEST_FILES})
target_link_libraries(test_serializer ${TEST_LIB_FILES} serializer)
