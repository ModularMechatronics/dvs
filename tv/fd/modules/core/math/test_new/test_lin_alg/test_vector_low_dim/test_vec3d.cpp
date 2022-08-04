#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "arl/math.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class Vec3DTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Vec3DTest, CrossProduct)
{
    const double eps = 1e-8;

    const double v0_a[3] = {0.5507979026, 0.7081478226, 0.2909047389};
    const double v1_a[3] = {0.5108276052, 0.8929469543, 0.8962930889};
    const double vc_a[3] = {0.3749454987, -0.3450741824, 0.1300918532};

    const Vec3Dd v0(v0_a);
    const Vec3Dd v1(v1_a);
    const Vec3Dd vc_expected(vc_a);

    const Vec3Dd vc_actual = v0.crossProduct(v1);

    ASSERT_NEAR(vc_actual.x, vc_expected.x, eps);
    ASSERT_NEAR(vc_actual.y, vc_expected.y, eps);
    ASSERT_NEAR(vc_actual.z, vc_expected.z, eps);
}

TEST_F(Vec3DTest, CrossProductMatrix)
{
    const double eps = 1e-8;

    const double v_a[3] = {1.0, 2.0, 3.0};
    const Vec3Dd v_expected(v_a);

    // clang-format off
    const double m_array[9] = { 0.0, -3.0,  2.0,
                                3.0,  0.0, -1.0,
                               -2.0,  1.0,  0.0};
    // clang-format off

    Matrixd m_expected(3, 3);

    fillMatrixWithArrayRowMajor(m_expected, m_array);

    Matrixd m_actual = v_expected.toCrossProductMatrix();
    Vec3Dd v_actual = crossProductMatrixToVec3D(m_actual);

    ASSERT_MATRIX_NEAR_MATRIX(m_expected, m_actual, eps);

    ASSERT_NEAR(v_actual.x, v_expected.x, eps);
    ASSERT_NEAR(v_actual.y, v_expected.y, eps);
    ASSERT_NEAR(v_actual.z, v_expected.z, eps);
}

TEST_F(Vec3DTest, Initialization)
{
    const double eps = 1e-8;
    const double x = 0.1, y = 0.2, z = 0.3;
    const Vec3Dd v(x, y, z);

    ASSERT_NEAR(v.x, x, eps);
    ASSERT_NEAR(v.y, y, eps);
    ASSERT_NEAR(v.z, z, eps);
}

TEST_F(Vec3DTest, InitializationFromArray)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    ASSERT_NEAR(v.x, ar[0], eps);
    ASSERT_NEAR(v.y, ar[1], eps);
    ASSERT_NEAR(v.z, ar[2], eps);
}

TEST_F(Vec3DTest, InitializationFromVector)
{
    const double eps = 1e-8;

    // TODO: Initialize Vectord with random numbers
    // Move this test to rapidcheck, and initialize from static array?

    for (size_t k = 0; k < 10; k++)
    {
        const int vector_size = (rand() % 20) + 3;
        const Vectord v(vector_size);
        const Vec3Dd v3 = v.toVec3D();

        ASSERT_NEAR(v3.x, v(0), eps);
        ASSERT_NEAR(v3.y, v(1), eps);
        ASSERT_NEAR(v3.z, v(2), eps);
    }
}

TEST_F(Vec3DTest, EmptyInitialization)
{
    const double eps = 1e-8;

    Vec3Dd v;

    const double x = 0.1, y = 0.2, z = 0.3;

    v.x = x;
    v.y = y;
    v.z = z;

    ASSERT_NEAR(v.x, x, eps);
    ASSERT_NEAR(v.y, y, eps);
    ASSERT_NEAR(v.z, z, eps);
}

TEST_F(Vec3DTest, VectorScalarMultiplication)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    const double f = 2.152352;

    const Vec3Dd v_left = f * v;
    const Vec3Dd v_right = v * f;

    ASSERT_NEAR(v_left.x, f * ar[0], eps);
    ASSERT_NEAR(v_left.y, f * ar[1], eps);
    ASSERT_NEAR(v_left.z, f * ar[2], eps);

    ASSERT_NEAR(v_right.x, f * ar[0], eps);
    ASSERT_NEAR(v_right.y, f * ar[1], eps);
    ASSERT_NEAR(v_right.z, f * ar[2], eps);
}

TEST_F(Vec3DTest, VectorScalarAddition)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    const double f = 2.152352;

    const Vec3Dd v_left = f + v;
    const Vec3Dd v_right = v + f;

    ASSERT_NEAR(v_left.x, f + ar[0], eps);
    ASSERT_NEAR(v_left.y, f + ar[1], eps);
    ASSERT_NEAR(v_left.z, f + ar[2], eps);

    ASSERT_NEAR(v_right.x, f + ar[0], eps);
    ASSERT_NEAR(v_right.y, f + ar[1], eps);
    ASSERT_NEAR(v_right.z, f + ar[2], eps);
}

TEST_F(Vec3DTest, VectorScalarSubtraction)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    double f = 2.152352;

    const Vec3Dd v_left = f - v;
    const Vec3Dd v_right = v - f;

    ASSERT_NEAR(v_left.x, f - ar[0], eps);
    ASSERT_NEAR(v_left.y, f - ar[1], eps);
    ASSERT_NEAR(v_left.z, f - ar[2], eps);

    ASSERT_NEAR(v_right.x, ar[0] - f, eps);
    ASSERT_NEAR(v_right.y, ar[1] - f, eps);
    ASSERT_NEAR(v_right.z, ar[2] - f, eps);
}

TEST_F(Vec3DTest, VectorScalarDivision)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    const double f = 2.152352;

    const Vec3Dd v_left = f / v;
    const Vec3Dd v_right = v / f;

    ASSERT_NEAR(v_left.x, f / ar[0], eps);
    ASSERT_NEAR(v_left.y, f / ar[1], eps);
    ASSERT_NEAR(v_left.z, f / ar[2], eps);

    ASSERT_NEAR(v_right.x, ar[0] / f, eps);
    ASSERT_NEAR(v_right.y, ar[1] / f, eps);
    ASSERT_NEAR(v_right.z, ar[2] / f, eps);
}

TEST_F(Vec3DTest, DotProduct)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.1, 0.2, 0.3};
    const double ar1[3] = {7.0, 8.0, 2.0};
    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const double d = v0 * v1;

    ASSERT_NEAR(d, ar0[0] * ar1[0] + ar0[1] * ar1[1] + ar0[2] * ar1[2], eps);
}

TEST_F(Vec3DTest, VectorVectorAddition)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.1, 0.2, 0.3};
    const double ar1[3] = {7.0, 8.0, 2.0};
    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const Vec3Dd vres = v0 + v1;

    ASSERT_NEAR(vres.x, ar0[0] + ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] + ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] + ar1[2], eps);
}

TEST_F(Vec3DTest, VectorVectorSubtraction)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.1, 0.2, 0.3};
    const double ar1[3] = {7.0, 8.0, 2.0};
    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const Vec3Dd vres = v0 - v1;

    ASSERT_NEAR(vres.x, ar0[0] - ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] - ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] - ar1[2], eps);
}

TEST_F(Vec3DTest, NegateVector)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v0(ar);

    const Vec3Dd vres = -v0;

    ASSERT_NEAR(vres.x, -ar[0], eps);
    ASSERT_NEAR(vres.y, -ar[1], eps);
    ASSERT_NEAR(vres.z, -ar[2], eps);
}

TEST_F(Vec3DTest, MatrixVectorMultiplication)
{
    const double eps = 1e-8;
    const double ar[3] = {0.02398188238, 0.558854088, 0.2592524469};

    const Matrixd m = {{0.5507979026, 0.8929469543, 0.0514672033},
                       {0.7081478226, 0.8962930889, 0.4408098437},
                       {0.2909047389, 0.1255853105, 0.02987621088}};

    const Vec3Dd v(ar);
    const Vec3Dd vres_right_expected(0.5255792247, 0.6321608052, 0.08490578817);
    const Vec3Dd vres_left_expected(0.4843782415, 0.5548699046, 0.2553281444);

    const Vec3Dd vres_right_actual = m * v;
    const Vec3Dd vres_left_actual = v * m;

    ASSERT_NEAR(vres_right_actual.x, vres_right_expected.x, eps);
    ASSERT_NEAR(vres_right_actual.y, vres_right_expected.y, eps);
    ASSERT_NEAR(vres_right_actual.z, vres_right_expected.z, eps);

    ASSERT_NEAR(vres_left_actual.x, vres_left_expected.x, eps);
    ASSERT_NEAR(vres_left_actual.y, vres_left_expected.y, eps);
    ASSERT_NEAR(vres_left_actual.z, vres_left_expected.z, eps);
}

TEST_F(Vec3DTest, OutputStream)
{
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    const std::string expected_string = "[ 0.100000, 0.200000, 0.300000 ]";
    std::stringstream out_stream;
    out_stream << v;
    const std::string actual_string = out_stream.str();

    ASSERT_EQ(expected_string, actual_string);
}

TEST_F(Vec3DTest, NormalizeVector)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    const double d = std::sqrt(ar[0] * ar[0] + ar[1] * ar[1] + ar[2] * ar[2]);

    const Vec3Dd vnorm = v.normalized();

    ASSERT_NEAR(vnorm.x, ar[0] / d, eps);
    ASSERT_NEAR(vnorm.y, ar[1] / d, eps);
    ASSERT_NEAR(vnorm.z, ar[2] / d, eps);
}

TEST_F(Vec3DTest, NormAndSquaredNorm)
{
    const double eps = 1e-8;
    const double ar[3] = {0.1, 0.2, 0.3};
    const Vec3Dd v(ar);

    const double squared_norm_expected = ar[0] * ar[0] + ar[1] * ar[1] + ar[2] * ar[2];
    const double norm_expected = std::sqrt(squared_norm_expected);

    const double squared_norm_actual = v.squaredNorm();
    const double norm_actual = v.norm();

    ASSERT_NEAR(squared_norm_expected, squared_norm_actual, eps);
    ASSERT_NEAR(norm_expected, norm_actual, eps);
}

TEST_F(Vec3DTest, VectorVectorElementwiseMultiply)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.1, 0.2, 0.3};
    const double ar1[3] = {7.0, 8.0, 2.0};
    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const Vec3Dd vres = v0.elementWiseMultiply(v1);

    ASSERT_NEAR(vres.x, ar0[0] * ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] * ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] * ar1[2], eps);
}

TEST_F(Vec3DTest, VectorVectorElementwiseDivide)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.1, 0.2, 0.3};
    const double ar1[3] = {7.0, 8.0, 2.0};
    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const Vec3Dd vres = v0.elementWiseDivide(v1);

    ASSERT_NEAR(vres.x, ar0[0] / ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] / ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] / ar1[2], eps);
}

TEST_F(Vec3DTest, OuterProduct)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.5507979026, 0.7081478226, 0.2909047389};
    const double ar1[3] = {0.8929469543, 0.8962930889, 0.1255853105};
    // clang-format off
    const double m_ar[3][3] = {{0.4918333096, 0.4936763535, 0.06917212562},
                               {0.6323384414, 0.6347079993, 0.08893296418},
                               {0.2597625006, 0.260735907, 0.03653336196}};
    // clang-format on

    const Matrixd m_expected = Matrix<double>(m_ar);
    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const Matrixd m_actual = v0.outerProduct(v1);

    ASSERT_MATRIX_NEAR_MATRIX(m_expected, m_actual, eps);
}

TEST_F(Vec3DTest, VectorBetweenPoints)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.5507979026, 0.7081478226, 0.2909047389};
    const double ar1[3] = {0.8929469543, 0.8962930889, 0.1255853105};

    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const Vec3Dd vres_expected = v1 - v0;

    const Vec3Dd vres_actual = v0.vectorBetweenPoints(v1);

    ASSERT_NEAR(vres_expected.x, vres_actual.x, eps);
    ASSERT_NEAR(vres_expected.y, vres_actual.y, eps);
    ASSERT_NEAR(vres_expected.z, vres_actual.z, eps);
}

TEST_F(Vec3DTest, NormalizedVectorBetweenPoints)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.5507979026, 0.7081478226, 0.2909047389};
    const double ar1[3] = {0.8929469543, 0.8962930889, 0.1255853105};

    const Vec3Dd v0(ar0);
    const Vec3Dd v1(ar1);

    const Vec3Dd vres_expected = (v1 - v0).normalized();

    const Vec3Dd vres_actual = v0.normalizedVectorBetweenPoints(v1);

    ASSERT_NEAR(vres_expected.x, vres_actual.x, eps);
    ASSERT_NEAR(vres_expected.y, vres_actual.y, eps);
    ASSERT_NEAR(vres_expected.z, vres_actual.z, eps);
}

TEST_F(Vec3DTest, Vec3DPerp)
{
    const double eps = 1e-8;

    // clang-format off
    const Matrixd tm = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1},
                        {1, 1, 0}, {0, 1, 1}, {1, 0, 1},
                        {1, 0, 1}, {1, 1, 0}, {0, 1, 1},
                        {1, 1, 1}};
    // clang-format on

    for (size_t r = 0; r < tm.rows(); r++)
    {
        const Vec3Dd vt(tm(r, 0), tm(r, 1), tm(r, 2));

        const Vec3Dd vperp = vt.perpendicularVector();
        const double dotp_res = vt * vperp;
        ASSERT_NEAR(dotp_res, 0.0, eps);
    }
}

}  // namespace
}  // namespace arl
