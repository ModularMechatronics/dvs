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
class Vec2DTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Vec2DTest, Initialization)
{
    const double eps = 1e-8;
    const double x = 0.1, y = 0.2;
    const Vec2Dd v(x, y);

    ASSERT_NEAR(v.x, x, eps);
    ASSERT_NEAR(v.y, y, eps);
}

TEST_F(Vec2DTest, InitializationFromArray)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    ASSERT_NEAR(v.x, ar[0], eps);
    ASSERT_NEAR(v.y, ar[1], eps);
}

TEST_F(Vec2DTest, InitializationFromVector)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const int vector_size = (rand() % 20) + 2;
        const Vectord v(vector_size);
        const Vec2Dd v2 = v.toVec2D();

        ASSERT_NEAR(v2.x, v(0), eps);
        ASSERT_NEAR(v2.y, v(1), eps);
    }
}

TEST_F(Vec2DTest, EmptyInitialization)
{
    const double eps = 1e-8;

    Vec2Dd v;

    const double x = 0.1, y = 0.2;

    v.x = x;
    v.y = y;

    ASSERT_NEAR(v.x, x, eps);
    ASSERT_NEAR(v.y, y, eps);
}

TEST_F(Vec2DTest, VectorScalarMultiplication)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    const double f = 2.152352;

    const Vec2Dd v_left = f * v;
    const Vec2Dd v_right = v * f;

    ASSERT_NEAR(v_left.x, f * ar[0], eps);
    ASSERT_NEAR(v_left.y, f * ar[1], eps);

    ASSERT_NEAR(v_right.x, f * ar[0], eps);
    ASSERT_NEAR(v_right.y, f * ar[1], eps);
}

TEST_F(Vec2DTest, VectorScalarAddition)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    const double f = 2.152352;

    const Vec2Dd v_left = f + v;
    const Vec2Dd v_right = v + f;

    ASSERT_NEAR(v_left.x, f + ar[0], eps);
    ASSERT_NEAR(v_left.y, f + ar[1], eps);

    ASSERT_NEAR(v_right.x, f + ar[0], eps);
    ASSERT_NEAR(v_right.y, f + ar[1], eps);
}

TEST_F(Vec2DTest, VectorScalarSubtraction)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    double f = 2.152352;

    const Vec2Dd v_left = f - v;
    const Vec2Dd v_right = v - f;

    ASSERT_NEAR(v_left.x, f - ar[0], eps);
    ASSERT_NEAR(v_left.y, f - ar[1], eps);

    ASSERT_NEAR(v_right.x, ar[0] - f, eps);
    ASSERT_NEAR(v_right.y, ar[1] - f, eps);
}

TEST_F(Vec2DTest, VectorScalarDivision)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    const double f = 2.152352;

    const Vec2Dd v_left = f / v;
    const Vec2Dd v_right = v / f;

    ASSERT_NEAR(v_left.x, f / ar[0], eps);
    ASSERT_NEAR(v_left.y, f / ar[1], eps);

    ASSERT_NEAR(v_right.x, ar[0] / f, eps);
    ASSERT_NEAR(v_right.y, ar[1] / f, eps);
}

TEST_F(Vec2DTest, DotProduct)
{
    const double eps = 1e-8;
    const double ar0[2] = {0.1, 0.2};
    const double ar1[2] = {7.0, 8.0};
    const Vec2Dd v0(ar0);
    const Vec2Dd v1(ar1);

    const double d = v0 * v1;

    ASSERT_NEAR(d, ar0[0] * ar1[0] + ar0[1] * ar1[1], eps);
}

TEST_F(Vec2DTest, VectorVectorAddition)
{
    const double eps = 1e-8;
    const double ar0[2] = {0.1, 0.2};
    const double ar1[2] = {7.0, 8.0};
    const Vec2Dd v0(ar0);
    const Vec2Dd v1(ar1);

    const Vec2Dd vres = v0 + v1;

    ASSERT_NEAR(vres.x, ar0[0] + ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] + ar1[1], eps);
}

TEST_F(Vec2DTest, VectorVectorSubtraction)
{
    const double eps = 1e-8;
    const double ar0[2] = {0.1, 0.2};
    const double ar1[2] = {7.0, 8.0};
    const Vec2Dd v0(ar0);
    const Vec2Dd v1(ar1);

    const Vec2Dd vres = v0 - v1;

    ASSERT_NEAR(vres.x, ar0[0] - ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] - ar1[1], eps);
}

TEST_F(Vec2DTest, NegateVector)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v0(ar);

    const Vec2Dd vres = -v0;

    ASSERT_NEAR(vres.x, -ar[0], eps);
    ASSERT_NEAR(vres.y, -ar[1], eps);
}

TEST_F(Vec2DTest, MatrixVectorMultiplication)
{
    const double eps = 1e-8;

    const Matrixd m = {{0.5507979026, 0.8929469543}, {0.7081478226, 0.8962930889}};

    const Vec2Dd v(0.02398188238, 0.558854088);
    const Vec2Dd vres_right_expected(0.5122362263, 0.5178797746);
    const Vec2Dd vres_left_expected(0.4089604761, 0.5223116056);

    const Vec2Dd vres_right_actual = m * v;
    const Vec2Dd vres_left_actual = v * m;

    ASSERT_NEAR(vres_right_actual.x, vres_right_expected.x, eps);
    ASSERT_NEAR(vres_right_actual.y, vres_right_expected.y, eps);

    ASSERT_NEAR(vres_left_actual.x, vres_left_expected.x, eps);
    ASSERT_NEAR(vres_left_actual.y, vres_left_expected.y, eps);
}

TEST_F(Vec2DTest, OutputStream)
{
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    const std::string expected_string = "[ 0.100000, 0.200000 ]";
    std::stringstream out_stream;
    out_stream << v;
    const std::string actual_string = out_stream.str();

    ASSERT_EQ(expected_string, actual_string);
}

TEST_F(Vec2DTest, NormalizeVector)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    const double d = std::sqrt(ar[0] * ar[0] + ar[1] * ar[1]);

    const Vec2Dd vnorm = v.normalized();

    ASSERT_NEAR(vnorm.x, ar[0] / d, eps);
    ASSERT_NEAR(vnorm.y, ar[1] / d, eps);
}

TEST_F(Vec2DTest, NormAndSquaredNorm)
{
    const double eps = 1e-8;
    const double ar[2] = {0.1, 0.2};
    const Vec2Dd v(ar);

    const double squared_norm_expected = ar[0] * ar[0] + ar[1] * ar[1];
    const double norm_expected = std::sqrt(squared_norm_expected);

    const double squared_norm_actual = v.squaredNorm();
    const double norm_actual = v.norm();

    ASSERT_NEAR(squared_norm_expected, squared_norm_actual, eps);
    ASSERT_NEAR(norm_expected, norm_actual, eps);
}

TEST_F(Vec2DTest, VectorVectorElementwiseMultiply)
{
    const double eps = 1e-8;
    const double ar0[2] = {0.1, 0.2};
    const double ar1[2] = {7.0, 8.0};
    const Vec2Dd v0(ar0);
    const Vec2Dd v1(ar1);

    const Vec2Dd vres = v0.elementWiseMultiply(v1);

    ASSERT_NEAR(vres.x, ar0[0] * ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] * ar1[1], eps);
}

TEST_F(Vec2DTest, VectorVectorElementwiseDivide)
{
    const double eps = 1e-8;
    const double ar0[2] = {0.1, 0.2};
    const double ar1[2] = {7.0, 8.0};
    const Vec2Dd v0(ar0);
    const Vec2Dd v1(ar1);

    const Vec2Dd vres = v0.elementWiseDivide(v1);

    ASSERT_NEAR(vres.x, ar0[0] / ar1[0], eps);
    ASSERT_NEAR(vres.y, ar0[1] / ar1[1], eps);
}

TEST_F(Vec2DTest, OuterProduct)
{
    const double eps = 1e-8;
    const Matrixd m_expected = {{0.4918333096, 0.4936763535}, {0.6323384414, 0.6347079993}};

    const Vec2Dd v0(0.5507979026, 0.7081478226);
    const Vec2Dd v1(0.8929469543, 0.8962930889);

    const Matrixd m_actual = v0.outerProduct(v1);

    ASSERT_MATRIX_NEAR_MATRIX(m_expected, m_actual, eps);
}

TEST_F(Vec2DTest, VectorBetweenPoints)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.5507979026, 0.7081478226, 0.2909047389};
    const double ar1[3] = {0.8929469543, 0.8962930889, 0.1255853105};

    const Vec2Dd v0(ar0);
    const Vec2Dd v1(ar1);

    const Vec2Dd vres_expected = v1 - v0;

    const Vec2Dd vres_actual = v0.vectorBetweenPoints(v1);

    ASSERT_NEAR(vres_expected.x, vres_actual.x, eps);
    ASSERT_NEAR(vres_expected.y, vres_actual.y, eps);
}

TEST_F(Vec2DTest, NormalizedVectorBetweenPoints)
{
    const double eps = 1e-8;
    const double ar0[3] = {0.5507979026, 0.7081478226, 0.2909047389};
    const double ar1[3] = {0.8929469543, 0.8962930889, 0.1255853105};

    const Vec2Dd v0(ar0);
    const Vec2Dd v1(ar1);

    const Vec2Dd vres_expected = (v1 - v0).normalized();

    const Vec2Dd vres_actual = v0.normalizedVectorBetweenPoints(v1);

    ASSERT_NEAR(vres_expected.x, vres_actual.x, eps);
    ASSERT_NEAR(vres_expected.y, vres_actual.y, eps);
}

}  // namespace
}  // namespace arl
