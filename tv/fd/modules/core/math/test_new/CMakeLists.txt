
cmake_minimum_required(VERSION 2.8.11 FATAL_ERROR)

project(math-core-tests CXX)

set (CMAKE_CXX_STANDARD 14)

set(TEST_CPP_FILES test_transformations/test_axis_angle.cpp
                   test_lin_alg/test_matrix/test_matrix.cpp
                   test_lin_alg/test_matrix/test_matrix_initialization.cpp
                   test_geometry/test_line2d_par.cpp
                   test_geometry/test_line2d_hom.cpp
                   test_transformations/test_quaternion.cpp
                   test_geometry/test_line3d.cpp
                   test_transformations/test_roll_pitch_yaw.cpp
                   test_coordinates/test_complex.cpp
                   test_lin_alg/test_vector/test_vector.cpp
                   test_lin_alg/test_vector/test_vector_initialization.cpp
                   test_lin_alg/test_vector/test_vector_indexing.cpp
                   test_lin_alg/test_vector_matrix/test_vector_matrix.cpp
                   test_lin_alg/test_vector_low_dim/test_vec2d.cpp
                   test_lin_alg/test_vector_low_dim/test_vec3d.cpp
                   test_lin_alg/test_vector_low_dim/test_vec4d.cpp
                   test_geometry/test_circle.cpp
                   test_lin_alg/test_vector/test_vector_math_functions.cpp
                   test_lin_alg/test_matrix/test_matrix_math_functions.cpp
                   test_structures/test_structures.cpp)

set(TEST_UTILS_FILES test_data/test_data_rotations.cpp)

set(TEST_LIB_FILES libgtest.a
                   libgtest_main.a
                   pthread)

# test-math-core executable
add_executable(test-math-core ${TEST_CPP_FILES} ${TEST_UTILS_FILES})
target_link_libraries(test-math-core ${TEST_LIB_FILES})
# cotire(test-math-core)

# set(RAPIDCHECK_CPP rapidcheck/rc_math.cpp)

# rapidcheck-math-core executable
# add_executable(rapidcheck-math-core
#                rapidcheck/rc_math_main.cpp
#                ${RAPIDCHECK_CPP})
# target_link_libraries(rapidcheck-math-core rapidcheck libgtest.a pthread)


# Individual targets

add_executable(test-math-core-structures test_structures/test_structures.cpp
                                         test_structures/test_lookup_grid.cpp
                                         ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-structures ${TEST_LIB_FILES})

add_executable(test-math-core-gen-matrix-math test_lin_alg/test_matrix/test_matrix_math_functions.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-gen-matrix-math ${TEST_LIB_FILES})
# cotire(test-math-core-gen-matrix-math)

add_executable(test-math-core-gen-math test_lin_alg/test_vector/test_vector_math_functions.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-gen-math ${TEST_LIB_FILES})
# cotire(test-math-core-gen-math)

add_executable(test-math-core-axis-angle test_transformations/test_axis_angle.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-axis-angle ${TEST_LIB_FILES})
# cotire(test-math-core-axis-angle)

add_executable(test-math-core-matrix test_lin_alg/test_matrix/test_matrix.cpp
                                     test_lin_alg/test_matrix/test_matrix_initialization.cpp
                                     ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-matrix ${TEST_LIB_FILES})
# cotire(test-math-core-matrix)

add_executable(test-math-core-matrix-fixed test_lin_alg/test_matrix_fixed/test_matrix_fixed.cpp
                                     ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-matrix-fixed ${TEST_LIB_FILES})

add_executable(test-math-core-line2d test_geometry/test_line2d_par.cpp test_geometry/test_line2d_hom.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-line2d ${TEST_LIB_FILES})
# cotire(test-math-core-line2d)

add_executable(test-math-core-quaternion test_transformations/test_quaternion.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-quaternion ${TEST_LIB_FILES})
# cotire(test-math-core-quaternion)

add_executable(test-math-core-line3d test_geometry/test_line3d.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-line3d ${TEST_LIB_FILES})
# cotire(test-math-core-line3d)

add_executable(test-math-core-rpy test_transformations/test_roll_pitch_yaw.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-rpy ${TEST_LIB_FILES})
# cotire(test-math-core-rpy)

add_executable(test-math-core-complex test_coordinates/test_complex.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-complex ${TEST_LIB_FILES})
# cotire(test-math-core-complex)

add_executable(test-math-core-vector test_lin_alg/test_vector/test_vector.cpp
                                     test_lin_alg/test_vector/test_vector_indexing.cpp
                                     test_lin_alg/test_vector/test_vector_initialization.cpp
                                     ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-vector ${TEST_LIB_FILES})
# cotire(test-math-core-vector)

add_executable(test-math-core-vector-matrix test_lin_alg/test_vector_matrix/test_vector_matrix.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-vector-matrix ${TEST_LIB_FILES})
# cotire(test-math-core-vector-matrix)

add_executable(test-math-core-vec2d test_lin_alg/test_vector_low_dim/test_vec2d.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-vec2d ${TEST_LIB_FILES})
# cotire(test-math-core-vec2d)

add_executable(test-math-core-vec3d test_lin_alg/test_vector_low_dim/test_vec3d.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-vec3d ${TEST_LIB_FILES})
# cotire(test-math-core-vec3d)

add_executable(test-math-core-vec4d test_lin_alg/test_vector_low_dim/test_vec4d.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-vec4d ${TEST_LIB_FILES})
# cotire(test-math-core-vec4d)

add_executable(test-math-core-circle test_geometry/test_circle.cpp ${TEST_UTILS_FILES})
target_link_libraries(test-math-core-circle ${TEST_LIB_FILES})
# cotire(test-math-core-circle)
