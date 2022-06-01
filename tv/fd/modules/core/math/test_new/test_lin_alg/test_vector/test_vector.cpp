#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class VectorTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(VectorTest, Initialization)
{
    // Create uninitialized
    const Vectord v_uninitialized;
    ASSERT_FALSE(v_uninitialized.isAllocated());

    const Vectord v_initialized(5);
    ASSERT_TRUE(v_initialized.isAllocated());
    ASSERT_TRUE(v_initialized.size() == 5);

    const Vectord v_copy = v_initialized;
    ASSERT_TRUE(v_copy.isAllocated());
    ASSERT_TRUE(v_copy.size() == 5);

    Vectord v_to_be_copied;
    ASSERT_FALSE(v_to_be_copied.isAllocated());
    v_to_be_copied = v_initialized;
    ASSERT_TRUE(v_to_be_copied.isAllocated());
}

TEST_F(VectorTest, InitializerList)
{
    double eps = 1e-6;

    for (size_t r = 0; r < 10; r++)
    {
        double rd = static_cast<double>(r);

        // clang-format off
        Vector<double> v{rd*20+1,
                         -rd*31+21,
                         rd*35+8,
                         -rd*11+3,
                         -rd*2+22,
                         rd*9+99};
        // clang-format on

        ASSERT_NEAR(v(0), rd * 20 + 1, eps);
        ASSERT_NEAR(v(1), -rd * 31 + 21, eps);
        ASSERT_NEAR(v(2), rd * 35 + 8, eps);
        ASSERT_NEAR(v(3), -rd * 11 + 3, eps);
        ASSERT_NEAR(v(4), -rd * 2 + 22, eps);
        ASSERT_NEAR(v(5), rd * 9 + 99, eps);
    }
}

TEST_F(VectorTest, DataPointer)
{
    const double eps = 1e-6;

    Vector<double> v(4);
    v(0) = 0.4;

    double* p = v.getDataPointer();

    ASSERT_NEAR(p[0], 0.4, eps);

    v(0) = 1.74;
    ASSERT_NEAR(p[0], 1.74, eps);
}

TEST_F(VectorTest, EqualityOperator)
{
    Vector<double> v0(4);
    Vector<double> v1(3);

    ASSERT_EQ(v0.size(), 4UL);
    ASSERT_EQ(v1.size(), 3UL);

    v1 = v0;

    ASSERT_NE(v0.getDataPointer(), v1.getDataPointer());

    ASSERT_EQ(v0.size(), 4UL);
    ASSERT_EQ(v1.size(), 4UL);
}

TEST_F(VectorTest, StdVectorInitialization)
{
    double eps = 1e-6;

    for (size_t r = 0; r < 10; r++)
    {
        std::vector<double> std_v;

        for (size_t i = 0; i < r + 5; i++)
        {
            double rd = static_cast<double>(r);
            double id = static_cast<double>(i);

            std_v.push_back((rd + 2.0235) * 2.2342 + id * 0.2523 + 0.25264);
        }
        const Vectord v(std_v);

        for (size_t i = 0; i < r + 5; i++)
        {
            ASSERT_NEAR(std_v[i], v(i), eps);
        }
    }
}

TEST_F(VectorTest, VectorAddition)
{
    const double eps = 1e-8;
    Vectord v0(5);
    Vectord v1(5);

    const double n0[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    const double n1[5] = {5.0, 4.0, 3.0, 2.0, 1.0};

    fillVectorWithArray(v0, n0);
    fillVectorWithArray(v1, n1);

    Vectord v_res = v0 + v1;
    ASSERT_TRUE(v_res.isAllocated());
    ASSERT_TRUE(v_res.size() == 5);
    for (size_t k = 0; k < 5; k++)
    {
        ASSERT_NEAR(v_res(k), 6.0, eps);
    }
}

TEST_F(VectorTest, VectorSubtraction)
{
    const double eps = 1e-8;
    Vectord v0(5);
    Vectord v1(5);

    const double n0[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    const double n1[5] = {5.0, 4.0, 3.0, 2.0, 1.0};
    const double res[5] = {-4.0, -2.0, 0.0, 2.0, 4.0};

    fillVectorWithArray(v0, n0);
    fillVectorWithArray(v1, n1);

    Vectord v_res = v0 - v1;
    ASSERT_TRUE(v_res.isAllocated());
    ASSERT_TRUE(v_res.size() == 5);
    for (size_t k = 0; k < 5; k++)
    {
        ASSERT_NEAR(v_res(k), res[k], eps);
    }
}

TEST_F(VectorTest, DotProduct)
{
    const double eps = 1e-8;
    Vectord v0(5);
    Vectord v1(5);

    const double n0[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    const double n1[5] = {5.0, 4.0, 3.0, 2.0, 1.0};
    const double expected_value = 35.0;

    fillVectorWithArray(v0, n0);
    fillVectorWithArray(v1, n1);

    double actual_value = v0 * v1;

    ASSERT_NEAR(expected_value, actual_value, eps);
}

TEST_F(VectorTest, NormAndSquaredNorm)
{
    const double eps = 1e-8;
    Vectord v(5);

    const double n0[5] = {1.523, 0.523, -2.141, 3.523, 1.111};
    const double norm_expected = 4.563199426;
    const double squared_norm_expected = 20.822789;

    fillVectorWithArray(v, n0);

    double norm_actual = v.norm();
    double squared_norm_actual = v.squaredNorm();

    ASSERT_NEAR(norm_expected, norm_actual, eps);
    ASSERT_NEAR(squared_norm_expected, squared_norm_actual, eps);
}

TEST_F(VectorTest, NormalizeVector)
{
    const double eps = 1e-8;
    Vectord v(5);
    Vectord v_expected(5);

    const double n0[5] = {1.523, 0.523, -2.141, 3.523, 1.111};
    const double v_norm_array[5] = {
        0.3337570546, 0.114612567, -0.469188348, 0.7720460298, 0.2434695257};

    fillVectorWithArray(v, n0);
    fillVectorWithArray(v_expected, v_norm_array);

    Vectord v_actual = v.normalized();

    ASSERT_VECTOR_NEAR_VECTOR(v_expected, v_actual, eps);

    ASSERT_NEAR(v_actual.norm(), 1.0, 1e-7);
}

TEST_F(VectorTest, VectorScalarMultiplication)
{
    const double eps = 1e-8;
    Vectord v(5);
    Vectord v_expected(5);

    const double n0[5] = {1.523, 0.523, -2.141, 3.523, 1.111};
    const double v_exp_array[5] = {0.22101776, 0.07589776, -0.31070192, 0.51125776, 0.16122832};
    const double f = 0.14512;

    fillVectorWithArray(v, n0);
    fillVectorWithArray(v_expected, v_exp_array);

    Vectord v_actual_left = f * v;
    Vectord v_actual_right = v * f;

    ASSERT_VECTOR_NEAR_VECTOR(v_expected, v_actual_left, eps);
    ASSERT_VECTOR_NEAR_VECTOR(v_expected, v_actual_right, eps);
}

TEST_F(VectorTest, VectorScalarAddition)
{
    const double eps = 1e-8;
    Vectord v(5);
    Vectord v_expected(5);

    const double n0[5] = {1.523, 0.523, -2.141, 3.523, 1.111};
    const double v_exp_array[5] = {1.66812, 0.66812, -1.99588, 3.66812, 1.25612};
    const double f = 0.14512;

    fillVectorWithArray(v, n0);
    fillVectorWithArray(v_expected, v_exp_array);

    Vectord v_actual_left = f + v;
    Vectord v_actual_right = v + f;

    ASSERT_VECTOR_NEAR_VECTOR(v_expected, v_actual_left, eps);
    ASSERT_VECTOR_NEAR_VECTOR(v_expected, v_actual_right, eps);
}

TEST_F(VectorTest, VectorScalarSubtraction)
{
    const double eps = 1e-8;
    Vectord v(5);
    Vectord v_expected_right(5);
    Vectord v_expected_left(5);

    const double n0[5] = {1.523, 0.523, -2.141, 3.523, 1.111};
    const double v_exp_array_right[5] = {1.37788, 0.37788, -2.28612, 3.37788, 0.96588};
    const double v_exp_array_left[5] = {-1.37788, -0.37788, 2.28612, -3.37788, -0.96588};

    const double f = 0.14512;

    fillVectorWithArray(v, n0);
    fillVectorWithArray(v_expected_right, v_exp_array_right);
    fillVectorWithArray(v_expected_left, v_exp_array_left);

    Vectord v_actual_left = f - v;
    Vectord v_actual_right = v - f;

    ASSERT_VECTOR_NEAR_VECTOR(v_expected_left, v_actual_left, eps);
    ASSERT_VECTOR_NEAR_VECTOR(v_expected_right, v_actual_right, eps);
}

TEST_F(VectorTest, VectorNegation)
{
    const double eps = 1e-8;
    Vectord v(5);
    Vectord v_expected(5);

    const double n0[5] = {1.523, 0.523, -2.141, 3.523, 1.111};
    const double v_exp_array[5] = {-1.523, -0.523, 2.141, -3.523, -1.111};

    fillVectorWithArray(v, n0);
    fillVectorWithArray(v_expected, v_exp_array);

    Vectord v_actual = -v;

    ASSERT_VECTOR_NEAR_VECTOR(v_expected, v_actual, eps);
}

TEST_F(VectorTest, VectorScalarDivision)
{
    const double eps = 1e-8;
    Vectord v(5);
    Vectord v_expected_scalar_den(5);
    Vectord v_expected_vec_den(5);

    const double n0[5] = {1.523, 0.523, -2.141, 3.523, 1.111};
    const double v_exp_array_scalar_den[5] = {
        10.49476295, 3.603914002, -14.75330761, 24.27646086, 7.655733186};
    const double v_exp_array_vec_den[5] = {
        0.09528562049, 0.2774760994, -0.06778141056, 0.04119216577, 0.1306210621};
    const double f = 0.14512;

    fillVectorWithArray(v, n0);
    fillVectorWithArray(v_expected_scalar_den, v_exp_array_scalar_den);
    fillVectorWithArray(v_expected_vec_den, v_exp_array_vec_den);

    Vectord v_actual_scalar_den = v / f;
    Vectord v_actual_vector_den = f / v;

    ASSERT_VECTOR_NEAR_VECTOR(v_expected_scalar_den, v_actual_scalar_den, eps);
    ASSERT_VECTOR_NEAR_VECTOR(v_expected_vec_den, v_actual_vector_den, eps);
}

TEST_F(VectorTest, VectorFillWithArray)
{
    const double eps = 1e-8;
    Vectord v2(2);
    Vectord v4(4);
    Vectord v6(6);
    Vectord v8(8);
    Vectord v11(11);

    const double n0[11] = {0.849129,
                           0.933993,
                           0.678735,
                           0.75774,
                           0.743132,
                           0.392227,
                           0.655478,
                           0.171187,
                           0.706046,
                           0.0318328,
                           0.276923};

    fillVectorWithArray(v2, n0);
    fillVectorWithArray(v4, n0);
    fillVectorWithArray(v6, n0);
    fillVectorWithArray(v8, n0);
    fillVectorWithArray(v11, n0);

    for (size_t k = 0; k < 2; k++)
    {
        ASSERT_NEAR(v2(k), n0[k], eps);
    }

    for (size_t k = 0; k < 4; k++)
    {
        ASSERT_NEAR(v4(k), n0[k], eps);
    }

    for (size_t k = 0; k < 6; k++)
    {
        ASSERT_NEAR(v6(k), n0[k], eps);
    }

    for (size_t k = 0; k < 8; k++)
    {
        ASSERT_NEAR(v8(k), n0[k], eps);
    }

    for (size_t k = 0; k < 11; k++)
    {
        ASSERT_NEAR(v11(k), n0[k], eps);
    }
}

TEST_F(VectorTest, VectorBetweenPoints)
{
    const double eps = 1e-8;
    Vectord p0(6);
    Vectord p1(6);
    Vectord vres_expected(6);

    const double p0_a[6] = {
        0.5507979026, 0.7081478226, 0.2909047389, 0.5108276052, 0.8929469543, 0.8962930889};
    const double p1_a[6] = {
        0.1255853105, 0.2072428781, 0.0514672033, 0.4408098437, 0.02987621088, 0.4568332244};
    const double vres_a[6] = {
        -0.4252125921, -0.5009049445, -0.2394375356, -0.07001776155, -0.8630707435, -0.4394598645};

    fillVectorWithArray(p0, p0_a);
    fillVectorWithArray(p1, p1_a);
    fillVectorWithArray(vres_expected, vres_a);

    Vectord vres_actual = p0.vectorBetweenPoints(p1);

    ASSERT_VECTOR_NEAR_VECTOR(vres_expected, vres_actual, eps);
}

TEST_F(VectorTest, MatrixVectorMultiplicationRight)
{
    const double eps = 1e-8;
    Vectord v(4);
    Matrixd m(3, 4);
    Vectord vres_expected(3);

    const double m_array[12] = {0.5507979026,
                                0.5108276052,
                                0.1255853105,
                                0.4408098437,
                                0.7081478226,
                                0.8929469543,
                                0.2072428781,
                                0.02987621088,
                                0.2909047389,
                                0.8962930889,
                                0.0514672033,
                                0.4568332244};
    const double v_array[4] = {0.6491440476, 0.2784872826, 0.676254902, 0.5908628174};
    const double vres_array[3] = {0.8451919996, 0.8661660691, 0.7431760211};

    fillMatrixWithArrayRowMajor(m, m_array);
    fillVectorWithArray(v, v_array);
    fillVectorWithArray(vres_expected, vres_array);

    Vectord vres_actual = m * v;
    ASSERT_TRUE(vres_actual.size() == 3);
    ASSERT_VECTOR_NEAR_VECTOR(vres_expected, vres_actual, eps);
}

TEST_F(VectorTest, MatrixVectorMultiplicationLeft)
{
    const double eps = 1e-8;
    Vectord v(3);
    Matrixd m(3, 4);
    Vectord vres_expected(4);

    const double m_array[12] = {0.5507979026,
                                0.5108276052,
                                0.1255853105,
                                0.4408098437,
                                0.7081478226,
                                0.8929469543,
                                0.2072428781,
                                0.02987621088,
                                0.2909047389,
                                0.8962930889,
                                0.0514672033,
                                0.4568332244};
    const double v_array[3] = {0.6491440476, 0.2784872826, 0.676254902};
    const double vres_array[4] = {0.7514830984, 1.186397665, 0.1740424113, 0.6034049383};

    fillMatrixWithArrayRowMajor(m, m_array);
    fillVectorWithArray(v, v_array);
    fillVectorWithArray(vres_expected, vres_array);

    Vectord vres_actual = v * m;

    ASSERT_TRUE(vres_actual.size() == 4);
    ASSERT_VECTOR_NEAR_VECTOR(vres_expected, vres_actual, eps);
}

TEST_F(VectorTest, ExtractSubVector)
{
    const double eps = 1e-8;

    Vectord v{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};

    for (size_t k = 0; k < 10; k++)
    {
        size_t min_idx = k;
        size_t max_idx = std::min(v.size() - 1, k + 3);
        Vectord vsub = v({min_idx, max_idx});
        ASSERT_EQ(vsub.size(), max_idx - min_idx + 1);
        for (size_t i = min_idx; i < max_idx + 1; i++)
        {
            ASSERT_NEAR(vsub(i - min_idx), v(i), eps);
        }
    }
}

TEST_F(VectorTest, FillVector)
{
    const double eps = 1e-8;

    Vectord v(10);

    ASSERT_EQ(v.size(), static_cast<size_t>(10));
    ASSERT_TRUE(v.isAllocated());

    for (size_t k = 0; k < 5; k++)
    {
        const double f = static_cast<double>(k);
        v.fill(f);
        for (size_t i = 0; i < 10; i++)
        {
            ASSERT_NEAR(v(i), f, eps);
        }
    }
}

TEST_F(VectorTest, ResizeVectorSimple)
{
    Vectord v_uninitialized;
    ASSERT_FALSE(v_uninitialized.isAllocated());
    ASSERT_EQ(v_uninitialized.size(), static_cast<size_t>(0));

    v_uninitialized.resize(0);
    ASSERT_FALSE(v_uninitialized.isAllocated());
    ASSERT_EQ(v_uninitialized.size(), static_cast<size_t>(0));

    v_uninitialized.resize(31);
    ASSERT_TRUE(v_uninitialized.isAllocated());
    ASSERT_EQ(v_uninitialized.size(), static_cast<size_t>(31));

    v_uninitialized.resize(31);
    ASSERT_TRUE(v_uninitialized.isAllocated());
    ASSERT_EQ(v_uninitialized.size(), static_cast<size_t>(31));
}

TEST_F(VectorTest, ResizeVector)
{
    Vectord v_initialized(10);

    ASSERT_EQ(v_initialized.size(), static_cast<size_t>(10));
    ASSERT_TRUE(v_initialized.isAllocated());

    for (size_t k = 0; k < 5; k++)
    {
        Vectord v_uninitialized;

        ASSERT_FALSE(v_uninitialized.isAllocated());

        v_initialized.resize((k + 2) * 5);

        ASSERT_TRUE(v_initialized.isAllocated());
        ASSERT_EQ(v_initialized.size(), static_cast<size_t>((k + 2) * 5));

        v_uninitialized.resize((k + 2) * 5);
        ASSERT_TRUE(v_uninitialized.isAllocated());
        ASSERT_EQ(v_uninitialized.size(), static_cast<size_t>((k + 2) * 5));
    }
}

TEST_F(VectorTest, EndIndexAndSize)
{
    for (size_t k = 0; k < 20; k++)
    {
        const size_t size_expected = k + 2;
        const size_t end_index_expected = size_expected - 1;
        Vectord v(size_expected);

        ASSERT_EQ(v.size(), size_expected);
        ASSERT_EQ(v.endIndex(), end_index_expected);
    }
}

TEST_F(VectorTest, ElementWiseMultiplyAndDivide)
{
    const double eps = 1e-8;

    Vectord v0(10);
    Vectord v1(10);

    std::vector<double> std_resvec_mul;
    std::vector<double> std_resvec_div;

    for (size_t k = 0; k < 10; k++)
    {
        v0(k) = (k + 2) * 3;
        v1(k) = (k + 1) * 4;

        std_resvec_mul.push_back(v0(k) * v1(k));
        std_resvec_div.push_back(v0(k) / v1(k));
    }

    Vectord vres_mul = v0 ^ v1;
    Vectord vres_div = v0 / v1;

    for (size_t k = 0; k < 10; k++)
    {
        ASSERT_NEAR(vres_mul(k), std_resvec_mul[k], eps);
        ASSERT_NEAR(vres_div(k), std_resvec_div[k], eps);
    }
}

TEST_F(VectorTest, AddVectorToMatrixCols)
{
    /*const double eps = 1e-8;

    Vectord v(10);
    Matrixd m(10, 4);

    for (size_t r = 0; r < 10; r++)
    {
        v(r) = (r + 3) * 4;
        for (size_t c = 0; c < 4; c++)
        {
            m(r, c) = (r + 2) * (c + 3) * 3;
        }
    }

    Matrixd mres = vCat(m, v);

    for (size_t r = 0; r < 10; r++)
    {
        for (size_t c = 0; c < 4; c++)
        {
            ASSERT_NEAR(mres(r, c), (r + 2) * (c + 3) * 3 + (r + 3) * 4, eps);
        }
    }*/
}

TEST_F(VectorTest, AddVectorToMatrixRows)
{
    /*const double eps = 1e-8;

    Vectord v(10);
    Matrixd m(4, 10);

    for (size_t c = 0; c < 10; c++)
    {
        v(c) = (c + 3) * 4;
        for (size_t r = 0; r < 4; r++)
        {
            m(r, c) = (r + 2) * (c + 3) * 3;
        }
    }

    Matrixd mres = hCat(m, v);

    for (size_t r = 0; r < 4; r++)
    {
        for (size_t c = 0; c < 10; c++)
        {
            ASSERT_NEAR(mres(r, c), (r + 2) * (c + 3) * 3 + (c + 3) * 4, eps);
        }
    }*/
}

TEST_F(VectorTest, VectorEqVector)
{
    const Vector<int> v0({1, 4, 3, 2, 6, 3, 1, 0, -3, 2, 5, 4});

    const Vector<int> v1({1, 4, 3, 2, 6, 3, 1, 0, -3, 2, 5, 4});
    const Vector<int> v2({1, 3, 3, 1, 6, 3, 4, 0, -4, 2, 3, 4});
    const Vector<int> v_exp02({1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1});

    const Vector<bool> v_act01 = v0 == v1;

    const Vector<bool> v_act02 = v0 == v2;
    ASSERT_TRUE(all(v_act01));

    ASSERT_VECTOR_EQ_VECTOR(v_act02, v_exp02);
}

TEST_F(VectorTest, InitializerListEqual)
{
    const Vector<int> v0 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    ASSERT_EQ(v0.size(), static_cast<size_t>(10));

    for (size_t k = 0; k < v0.size(); k++)
    {
        ASSERT_EQ(v0(k), static_cast<int>(k));
    }
}

TEST_F(VectorTest, Sorting)
{
    Vector<int> v0 = {0, 2, 6,  1, 3, 7,  10, 1,  3,  11, 23, 3,  1, 2, 5, 2,  7, 4, 2,  4,
                      7, 3, 99, 1, 5, 42, 65, 14, 92, 42, 10, 32, 6, 3, 1, 53, 1, 4, 23, 4};
    v0.sort();
    for (size_t k = 0; k < v0.size() - 1; k++)
    {
        ASSERT_LE(v0(k), v0(k + 1));
    }
}

TEST_F(VectorTest, SortingIndices)
{
    const Vector<int> v0 = {0, 2, 6,  1, 3, 7,  10, 1,  3,  11, 23, 3,  1, 2, 5, 2,  7, 4, 2,  4,
                            7, 3, 99, 1, 5, 42, 65, 14, 92, 42, 10, 32, 6, 3, 1, 53, 1, 4, 23, 4};

    const Vector<size_t> v_indices = sortedIndices(v0);

    for (size_t k = 0; k < v0.size() - 1; k++)
    {
        const size_t idx0 = v_indices(k);
        const size_t idx1 = v_indices(k + 1);
        ASSERT_LE(v0(idx0), v0(idx1));
    }
}

TEST_F(VectorTest, SortingValuesAndIndices)
{
    const Vector<int> v0 = {0, 2, 6,  1, 3, 7,  10, 1,  3,  11, 23, 3,  1, 2, 5, 2,  7, 4, 2,  4,
                            7, 3, 99, 1, 5, 42, 65, 14, 92, 42, 10, 32, 6, 3, 1, 53, 1, 4, 23, 4};

    const std::pair<Vector<int>, Vector<size_t>> p = sortValuesAndIndices(v0);
    const Vector<int> v_values = p.first;
    const Vector<size_t> v_indices = p.second;

    for (size_t k = 0; k < v0.size() - 1; k++)
    {
        ASSERT_LE(v_values(k), v_values(k + 1));

        const size_t idx0 = v_indices(k);
        const size_t idx1 = v_indices(k + 1);
        ASSERT_LE(v0(idx0), v0(idx1));
    }
}

TEST_F(VectorTest, VectorCasting)
{
    const Vector<float> v0 = {-2.1, -1.8, -1.2, -0.7, -0.3, 0.1, 0.7, 1.1, 1.7, 2.2};
    Vector<int> v1;
    v1 = v0;

    const Vector<int> v2 = v0;

    const Vector<int> v_exp = {-2, -1, -1, 0, 0, 0, 0, 1, 1, 2};

    ASSERT_VECTOR_EQ_VECTOR(v_exp, v1);
    ASSERT_VECTOR_EQ_VECTOR(v_exp, v2);
}

TEST_F(VectorTest, IntegerLinspace)
{
    const float l0 = -3.0f;
    const float l1 = 7.0f;
    const Vector<float> v0 = integerLinspace(l0, l1);

    const size_t num_elements = static_cast<size_t>(l1 - l0) + 1;

    ASSERT_EQ(num_elements, v0.size());
    for (size_t k = 0; k < num_elements; k++)
    {
        ASSERT_EQ(static_cast<float>(k + l0), v0(k));
    }
}

TEST_F(VectorTest, SumMeanVarRMS)
{
    const double eps = 1e-5;
    const Vector<double> v0 = {2.2, 5.3, 0.2, -0.12, 0.3, 45.0, 32, 97.3, -78.5};
    const double sum_exp = 103.68;
    const double mean_exp = 11.52;
    const double var_exp = 2189.65;
    const double rms_exp = 45.5968;

    ASSERT_NEAR(sum_exp, sum(v0), eps);
    ASSERT_NEAR(mean_exp, mean(v0), eps);
    // ASSERT_NEAR(var_exp, variance(v0), eps);
    ASSERT_NEAR(rms_exp, rootMeanSquare(v0), eps);
}

TEST_F(VectorTest, FindItemsInVector)
{
    const Vector<int> v0 = {0, 2, 4, 2, 5, 6, 2, 3, 4, 6, 4, 9, 1, 0};

    const Vector<size_t> indices_of_1_exp = {12};
    const Vector<size_t> indices_of_2_exp = {1, 3, 6};
    const Vector<size_t> indices_of_4_exp = {2, 8, 10};
    const Vector<size_t> indices_of_6_exp = {5, 9};
    const Vector<size_t> indices_of_0_exp;

    const Vector<size_t> indices_of_2_act = v0.findIndicesOf(2);
    const Vector<size_t> indices_of_4_act = v0.findIndicesOf(4);
    const Vector<size_t> indices_of_6_act = v0.findIndicesOf(6);
    const Vector<size_t> indices_of_1_act = v0.findIndicesOf(1);
    const Vector<size_t> indices_of_0_act = v0.findIndicesOf(-1);

    ASSERT_VECTOR_EQ_VECTOR(indices_of_2_exp, indices_of_2_act);
    ASSERT_VECTOR_EQ_VECTOR(indices_of_4_exp, indices_of_4_act);
    ASSERT_VECTOR_EQ_VECTOR(indices_of_6_exp, indices_of_6_act);
    ASSERT_VECTOR_EQ_VECTOR(indices_of_1_exp, indices_of_1_act);
    ASSERT_EQ(indices_of_0_act.size(), indices_of_0_exp.size());
    ASSERT_EQ(indices_of_0_act.size(), static_cast<size_t>(0));
}

TEST_F(VectorTest, VectorWithOnes)
{
    for (size_t num_values = 1; num_values < 10; num_values++)
    {
        const Vector<int> ones_vec_int = vectorWithOnes<int>(num_values);
        const Vector<double> ones_vec_double = vectorWithOnes<double>(num_values);
        for (size_t k = 0; k < num_values; k++)
        {
            ASSERT_EQ(ones_vec_int(k), 1);
            ASSERT_EQ(ones_vec_double(k), 1.0);
        }
    }
}

TEST_F(VectorTest, RoundAndCast)
{
    const Vector<float> v0 = {-2.1, -1.8, -1.2, -0.7, -0.3, 0.1, 0.7, 1.1, 1.7, 2.2};
    Vector<int> v1;
    v1 = roundAndCast<int>(v0);

    const Vector<int> v2 = roundAndCast<int>(v0);

    const Vector<int> v_exp = {-2, -2, -1, -1, 0, 0, 1, 1, 2, 2};

    ASSERT_VECTOR_EQ_VECTOR(v_exp, v1);
    ASSERT_VECTOR_EQ_VECTOR(v_exp, v2);
}

TEST_F(VectorTest, VectorMin)
{
    const double eps = 1e-8;

    const Vectord vec = {4, 1, 6, 3, 2, 5, 0.5, 2, 5, 1};

    const double min_vec_act = vec.min();
    const double min_vec_exp = 0.5;
    ASSERT_NEAR(min_vec_exp, min_vec_act, eps);
}

TEST_F(VectorTest, VectorMax)
{
    const double eps = 1e-8;

    const Vectord vec = {4, 1, 6, 3, 2, 5, 0.5, 11.3, 5, 1};

    const double max_vec_act = vec.max();
    const double max_vec_exp = 11.3;
    ASSERT_NEAR(max_vec_exp, max_vec_act, eps);
}

TEST_F(VectorTest, VectorSum)
{
    const double eps = 1e-8;

    const Vectord vec = {4, 1, 6, 3, 2, 5, 0.5, 11.3, 5, 1};

    const double sum_vec_act = vec.sum();
    const double sum_vec_exp = 38.8;
    ASSERT_NEAR(sum_vec_exp, sum_vec_act, eps);
}

}  // namespace
}  // namespace arl
