#include <gtest/gtest.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "arl/math.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class Vec4DTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Vec4DTest, Initialization)
{
    const double eps = 1e-8;
    const double x = 0.1, y = 0.2, z = 0.3, w = 0.4;
    const Vec4Dd v(x, y, z, w);

    ASSERT_NEAR(v.x, x, eps);
    ASSERT_NEAR(v.y, y, eps);
    ASSERT_NEAR(v.z, z, eps);
    ASSERT_NEAR(v.w, w, eps);
}

TEST_F(Vec4DTest, InitializationFromArray)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    ASSERT_NEAR(v.x, ar[0], eps);
    ASSERT_NEAR(v.y, ar[1], eps);
    ASSERT_NEAR(v.z, ar[2], eps);
    ASSERT_NEAR(v.w, ar[3], eps);
}

TEST_F(Vec4DTest, InitializationFromVector)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        int vector_size = (rand() % 20) + 4;
        Vectord v(vector_size);
        Vec4Dd v4 = v.toVec4D();

        ASSERT_NEAR(v4.x, v(0), eps);
        ASSERT_NEAR(v4.y, v(1), eps);
        ASSERT_NEAR(v4.z, v(2), eps);
        ASSERT_NEAR(v4.w, v(3), eps);
    }
}

TEST_F(Vec4DTest, EmptyInitialization)
{
    const double eps = 1e-8;

    Vec4Dd v;

    const double x = 0.1, y = 0.2, z = 0.3, w = 0.4;

    v.x = x;
    v.y = y;
    v.z = z;
    v.w = w;

    ASSERT_NEAR(v.x, x, eps);
    ASSERT_NEAR(v.y, y, eps);
    ASSERT_NEAR(v.z, z, eps);
    ASSERT_NEAR(v.w, w, eps);
}

TEST_F(Vec4DTest, VectorScalarMultiplication)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    const double f = 2.152352;

    const Vec4Dd v_left = f * v;
    const Vec4Dd v_right = v * f;

    ASSERT_NEAR(v_left.x, f * ar[0], eps);
    ASSERT_NEAR(v_left.y, f * ar[1], eps);
    ASSERT_NEAR(v_left.z, f * ar[2], eps);
    ASSERT_NEAR(v_left.w, f * ar[3], eps);

    ASSERT_NEAR(v_right.x, f * ar[0], eps);
    ASSERT_NEAR(v_right.y, f * ar[1], eps);
    ASSERT_NEAR(v_right.z, f * ar[2], eps);
    ASSERT_NEAR(v_right.w, f * ar[3], eps);
}

TEST_F(Vec4DTest, VectorScalarAddition)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    const double f = 2.152352;

    const Vec4Dd v_left = f + v;
    const Vec4Dd v_right = v + f;

    ASSERT_NEAR(v_left.x, f + ar[0], eps);
    ASSERT_NEAR(v_left.y, f + ar[1], eps);
    ASSERT_NEAR(v_left.z, f + ar[2], eps);
    ASSERT_NEAR(v_left.w, f + ar[3], eps);

    ASSERT_NEAR(v_right.x, f + ar[0], eps);
    ASSERT_NEAR(v_right.y, f + ar[1], eps);
    ASSERT_NEAR(v_right.z, f + ar[2], eps);
    ASSERT_NEAR(v_right.w, f + ar[3], eps);
}

TEST_F(Vec4DTest, VectorScalarSubtraction)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    const double f = 2.152352;

    const Vec4Dd v_left = f - v;
    const Vec4Dd v_right = v - f;

    ASSERT_NEAR(v_left.x, f - ar[0], eps);
    ASSERT_NEAR(v_left.y, f - ar[1], eps);
    ASSERT_NEAR(v_left.z, f - ar[2], eps);
    ASSERT_NEAR(v_left.w, f - ar[3], eps);

    ASSERT_NEAR(v_right.x, ar[0] - f, eps);
    ASSERT_NEAR(v_right.y, ar[1] - f, eps);
    ASSERT_NEAR(v_right.z, ar[2] - f, eps);
    ASSERT_NEAR(v_right.w, ar[3] - f, eps);
}

TEST_F(Vec4DTest, VectorScalarDivision)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    const double f = 2.152352;

    const Vec4Dd v_left = f / v;
    const Vec4Dd v_right = v / f;

    ASSERT_NEAR(v_left.x, f / ar[0], eps);
    ASSERT_NEAR(v_left.y, f / ar[1], eps);
    ASSERT_NEAR(v_left.z, f / ar[2], eps);
    ASSERT_NEAR(v_left.w, f / ar[3], eps);

    ASSERT_NEAR(v_right.x, ar[0] / f, eps);
    ASSERT_NEAR(v_right.y, ar[1] / f, eps);
    ASSERT_NEAR(v_right.z, ar[2] / f, eps);
    ASSERT_NEAR(v_right.w, ar[3] / f, eps);
}

TEST_F(Vec4DTest, DotProduct)
{
    const double eps = 1e-8;
    const double ar0[4] = {0.1, 0.2, 0.3, 0.4};
    const double ar1[4] = {7.0, 8.0, 2.0, 4.0};
    const Vec4Dd v0(ar0);
    const Vec4Dd v1(ar1);

    const double d = v0 * v1;

    ASSERT_NEAR(d, ar0[0] * ar1[0] + ar0[1] * ar1[1] + ar0[2] * ar1[2] + ar0[3] * ar1[3], eps);
}

TEST_F(Vec4DTest, VectorVectorAddition)
{
    const double eps = 1e-8;
    const double ar0[4] = {0.1, 0.2, 0.3, 0.4};
    const double ar1[4] = {7.0, 8.0, 2.0, 4.0};
    const Vec4Dd v0(ar0);
    const Vec4Dd v1(ar1);

    const Vec4Dd vres = v0 + v1;

    ASSERT_NEAR(vres.x, ar0[0] + ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] + ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] + ar1[2], eps);
    ASSERT_NEAR(vres.w, ar0[3] + ar1[3], eps);
}

TEST_F(Vec4DTest, VectorVectorSubtraction)
{
    const double eps = 1e-8;
    const double ar0[4] = {0.1, 0.2, 0.3, 0.4};
    const double ar1[4] = {7.0, 8.0, 2.0, 4.0};
    const Vec4Dd v0(ar0);
    const Vec4Dd v1(ar1);

    const Vec4Dd vres = v0 - v1;

    ASSERT_NEAR(vres.x, ar0[0] - ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] - ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] - ar1[2], eps);
    ASSERT_NEAR(vres.w, ar0[3] - ar1[3], eps);
}

TEST_F(Vec4DTest, NegateVector)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v0(ar);

    const Vec4Dd vres = -v0;

    ASSERT_NEAR(vres.x, -ar[0], eps);
    ASSERT_NEAR(vres.y, -ar[1], eps);
    ASSERT_NEAR(vres.z, -ar[2], eps);
    ASSERT_NEAR(vres.w, -ar[3], eps);
}

TEST_F(Vec4DTest, MatrixVectorMultiplication)
{
    const double eps = 1e-8;
    const double ar[4] = {0.02398188238, 0.558854088, 0.2592524469, 0.415101197};

    const Matrixd m = {{0.5507979026, 0.8929469543, 0.0514672033, 0.6491440476},
                       {0.7081478226, 0.8962930889, 0.4408098437, 0.2784872826},
                       {0.2909047389, 0.1255853105, 0.02987621088, 0.676254902},
                       {0.5108276052, 0.2072428781, 0.4568332244, 0.5908628174}};

    const Vec4Dd v(ar);
    const Vec4Dd vres_right_expected(0.7950396959, 0.7477612095, 0.3656200074, 0.4917721312);
    const Vec4Dd vres_left_expected(0.6964233918, 0.6408966714, 0.4449601626, 0.5917900534);

    const Vec4Dd vres_right_actual = m * v;
    const Vec4Dd vres_left_actual = v * m;

    ASSERT_NEAR(vres_right_actual.x, vres_right_expected.x, eps);
    ASSERT_NEAR(vres_right_actual.y, vres_right_expected.y, eps);
    ASSERT_NEAR(vres_right_actual.z, vres_right_expected.z, eps);
    ASSERT_NEAR(vres_right_actual.w, vres_right_expected.w, eps);

    ASSERT_NEAR(vres_left_actual.x, vres_left_expected.x, eps);
    ASSERT_NEAR(vres_left_actual.y, vres_left_expected.y, eps);
    ASSERT_NEAR(vres_left_actual.z, vres_left_expected.z, eps);
    ASSERT_NEAR(vres_left_actual.w, vres_left_expected.w, eps);
}

TEST_F(Vec4DTest, OutputStream)
{
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    const std::string expected_string = "[ 0.100000, 0.200000, 0.300000, 0.400000 ]";
    std::stringstream out_stream;
    out_stream << v;
    const std::string actual_string = out_stream.str();

    ASSERT_EQ(expected_string, actual_string);
}

TEST_F(Vec4DTest, NormalizeVector)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    const double d = std::sqrt(ar[0] * ar[0] + ar[1] * ar[1] + ar[2] * ar[2] + ar[3] * ar[3]);

    const Vec4Dd vnorm = v.normalized();

    ASSERT_NEAR(vnorm.x, ar[0] / d, eps);
    ASSERT_NEAR(vnorm.y, ar[1] / d, eps);
    ASSERT_NEAR(vnorm.z, ar[2] / d, eps);
    ASSERT_NEAR(vnorm.w, ar[3] / d, eps);
}

TEST_F(Vec4DTest, NormAndSquaredNorm)
{
    const double eps = 1e-8;
    const double ar[4] = {0.1, 0.2, 0.3, 0.4};
    const Vec4Dd v(ar);

    const double squared_norm_expected =
        ar[0] * ar[0] + ar[1] * ar[1] + ar[2] * ar[2] + ar[3] * ar[3];
    const double norm_expected = std::sqrt(squared_norm_expected);

    const double squared_norm_actual = v.squaredNorm();
    const double norm_actual = v.norm();

    ASSERT_NEAR(squared_norm_expected, squared_norm_actual, eps);
    ASSERT_NEAR(norm_expected, norm_actual, eps);
}

TEST_F(Vec4DTest, VectorVectorElementwiseMultiply)
{
    const double eps = 1e-8;
    const double ar0[4] = {0.1, 0.2, 0.3, 0.4};
    const double ar1[4] = {7.0, 8.0, 2.0, 4.0};
    const Vec4Dd v0(ar0);
    const Vec4Dd v1(ar1);

    const Vec4Dd vres = v0.elementWiseMultiply(v1);

    ASSERT_NEAR(vres.x, ar0[0] * ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] * ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] * ar1[2], eps);
    ASSERT_NEAR(vres.w, ar0[3] * ar1[3], eps);
}

TEST_F(Vec4DTest, VectorVectorElementwiseDivide)
{
    const double eps = 1e-8;
    const double ar0[4] = {0.1, 0.2, 0.3, 0.4};
    const double ar1[4] = {7.0, 8.0, 2.0, 4.0};
    const Vec4Dd v0(ar0);
    const Vec4Dd v1(ar1);

    const Vec4Dd vres = v0.elementWiseDivide(v1);

    ASSERT_NEAR(vres.x, ar0[0] / ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] / ar1[1], eps);
    ASSERT_NEAR(vres.z, ar0[2] / ar1[2], eps);
    ASSERT_NEAR(vres.w, ar0[3] / ar1[3], eps);
}

TEST_F(Vec4DTest, OuterProduct)
{
    const double eps = 1e-8;
    const Matrixd m_expected = {{0.4918333096, 0.4936763535, 0.0691721256, 0.1141489426},
                                {0.6323384414, 0.6347079994, 0.08893296416, 0.1467585929},
                                {0.2597625006, 0.260735907, 0.03653336195, 0.06028793536},
                                {0.4561419543, 0.4578512522, 0.06415244339, 0.1058653831}};

    const Vec4Dd v0(0.5507979026, 0.7081478226, 0.2909047389, 0.5108276052);
    const Vec4Dd v1(0.8929469543, 0.8962930889, 0.1255853105, 0.2072428781);

    const Matrixd m_actual = v0.outerProduct(v1);

    ASSERT_MATRIX_NEAR_MATRIX(m_expected, m_actual, eps);
}

TEST_F(Vec4DTest, VectorBetweenPoints)
{
    const double eps = 1e-8;
    const double ar0[4] = {0.5507979026, 0.7081478226, 0.2909047389, 0.5108276052};
    const double ar1[4] = {0.8929469543, 0.8962930889, 0.1255853105, 0.2072428781};

    const Vec4Dd v0(ar0);
    const Vec4Dd v1(ar1);

    const Vec4Dd vres_expected = v1 - v0;

    const Vec4Dd vres_actual = v0.vectorBetweenPoints(v1);

    ASSERT_NEAR(vres_expected.x, vres_actual.x, eps);
    ASSERT_NEAR(vres_expected.y, vres_actual.y, eps);
    ASSERT_NEAR(vres_expected.z, vres_actual.z, eps);
    ASSERT_NEAR(vres_expected.w, vres_actual.w, eps);
}

TEST_F(Vec4DTest, NormalizedVectorBetweenPoints)
{
    const double eps = 1e-8;
    const double ar0[4] = {0.5507979026, 0.7081478226, 0.2909047389, 0.5108276052};
    const double ar1[4] = {0.8929469543, 0.8962930889, 0.1255853105, 0.2072428781};

    const Vec4Dd v0(ar0);
    const Vec4Dd v1(ar1);

    const Vec4Dd vres_expected = (v1 - v0).normalized();

    const Vec4Dd vres_actual = v0.normalizedVectorBetweenPoints(v1);

    ASSERT_NEAR(vres_expected.x, vres_actual.x, eps);
    ASSERT_NEAR(vres_expected.y, vres_actual.y, eps);
    ASSERT_NEAR(vres_expected.z, vres_actual.z, eps);
    ASSERT_NEAR(vres_expected.w, vres_actual.w, eps);
}

}  // namespace
}  // namespace arl
