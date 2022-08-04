#include <gtest/gtest.h>

#include "arl/math.h"

// clang-format off

#define DEG2RAD(d) (d * M_PI / 180.0)
#define RAD2DEG(r) (r * 180.0 / M_PI)

#define ASSERT_VECTOR_NEAR_VECTOR(v_expected, v_actual, eps) \
    ASSERT_TRUE(v_expected.isAllocated());                   \
    ASSERT_TRUE(v_actual.isAllocated());                     \
    ASSERT_TRUE(v_expected.size() == v_actual.size());       \
    for (size_t k = 0; k < v_expected.size(); k++)           \
    {                                                        \
        ASSERT_NEAR(v_expected(k), v_actual(k), eps);        \
    }

#define ASSERT_VECTOR_EQ_VECTOR(v_expected, v_actual)    \
    ASSERT_TRUE(v_expected.isAllocated());               \
    ASSERT_TRUE(v_actual.isAllocated());                 \
    ASSERT_TRUE(v_expected.size() == v_actual.size());   \
    for (size_t k = 0; k < v_expected.size(); k++)       \
    {                                                    \
        ASSERT_EQ(v_expected(k), v_actual(k));           \
    }


#define ASSERT_VECTOR_NEAR_SCALAR(vec, c, eps)     \
    for (size_t k = 0; k < vec.size(); k++) \
    {                                              \
        ASSERT_NEAR(vec(k), c, eps);               \
    }

#define ASSERT_VECTOR_EQ_SCALAR(vec, c)       \
    for (size_t k = 0; k < vec.size(); k++)   \
    {                                         \
        ASSERT_EQ(vec(k), c);                 \
    }

#define ASSERT_MATRIX_NEAR_MATRIX(m0, m1, eps)     \
    ASSERT_TRUE(m0.isAllocated());                 \
    ASSERT_TRUE(m1.isAllocated());                 \
    ASSERT_TRUE(m0.rows() == m1.rows());           \
    ASSERT_TRUE(m0.cols() == m1.cols());           \
    for (size_t r = 0; r < m0.rows(); r++)         \
    {                                              \
        for (size_t c = 0; c < m0.cols(); c++)     \
        {                                          \
            ASSERT_NEAR(m0(r, c), m1(r, c), eps);  \
        }                                          \
    }

#define ASSERT_MATRIX_EQ_MATRIX(m0, m1)            \
    ASSERT_TRUE(m0.isAllocated());                 \
    ASSERT_TRUE(m1.isAllocated());                 \
    ASSERT_TRUE(m0.rows() == m1.rows());           \
    ASSERT_TRUE(m0.cols() == m1.cols());           \
    for (size_t r = 0; r < m0.rows(); r++)         \
    {                                              \
        for (size_t c = 0; c < m0.cols(); c++)     \
        {                                          \
            ASSERT_EQ(m0(r, c), m1(r, c));         \
        }                                          \
    }

#define ASSERT_MATRIX_NEAR_SCALAR(m, f, eps)   \
    ASSERT_TRUE(m.isAllocated());              \
    ASSERT_TRUE(m.rows() > 0 && m.cols() > 0); \
    for (size_t r = 0; r < m.rows(); r++)      \
    {                                          \
        for (size_t c = 0; c < m.cols(); c++)  \
        {                                      \
            ASSERT_NEAR(m(r, c), f, eps);      \
        }                                      \
    }

// clang-format on
