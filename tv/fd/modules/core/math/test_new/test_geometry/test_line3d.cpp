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
class Line3DTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(Line3DTest, Initialization)
{
    const double eps = 1e-7;

    const Point3Dd p(0.4124, 2.1262, -5.2324);
    const Vec3Dd v(4.3343, 0.4252, -5.5233);

    const Line3Dd line(p, v);

    ASSERT_NEAR(line.p.x, p.x, eps);
    ASSERT_NEAR(line.p.y, p.y, eps);
    ASSERT_NEAR(line.p.z, p.z, eps);

    ASSERT_NEAR(line.v.x, v.x, eps);
    ASSERT_NEAR(line.v.y, v.y, eps);
    ASSERT_NEAR(line.v.z, v.z, eps);
}

TEST_F(Line3DTest, LineFromTwoPoints)
{
    /*const double eps = 1e-7;

    const Point3Dd p(0.4124, 2.1262, -5.2324);
    const Vec3Dd v(4.3343, 0.4252, -5.5233);
    const Vec3Dd v_n = v.normalized();
    double t = 0.23523;
    const Point3Dd p0_expected = p;
    const Point3Dd p1_expected = p + t * v_n;

    const Line3Dd line(p, v);

    const Line3Dd line_fp = lineFromTwoPoints(p0_expected, p1_expected);

    // ASSERT_NEAR(line.p.x, line_fp.p.x, eps);
    // ASSERT_NEAR(line.p.y, line_fp.p.y, eps);
    // ASSERT_NEAR(line.p.z, line_fp.p.z, eps);

    // ASSERT_NEAR(line.v.x, line_fp.v.x, eps);
    // ASSERT_NEAR(line.v.y, line_fp.v.y, eps);
    // ASSERT_NEAR(line.v.z, line_fp.v.z, eps);

    const Point3Dd p0_actual = evalLine3D(line, 0.0);
    const Point3Dd p1_actual = evalLine3D(line, t);*/

    // ASSERT_NEAR(p0_actual.x, p0_expected.x, eps);
    // ASSERT_NEAR(p0_actual.y, p0_expected.y, eps);
    // ASSERT_NEAR(p0_actual.z, p0_expected.z, eps);

    // ASSERT_NEAR(p1_actual.x, p1_expected.x, eps);
    // ASSERT_NEAR(p1_actual.y, p1_expected.y, eps);
    // ASSERT_NEAR(p1_actual.z, p1_expected.z, eps);
}

TEST_F(Line3DTest, ClosestPointOnLine)
{
    /*const double eps = 1e-7;

    // Point3D<T> closestPointOnLine3DFromPoint(const Line3D<T>& line, const Point3D<T>& q)

    double t = 0.23523;
    const Point3Dd p(0.4124, 2.1262, -5.2324);
    const Vec3Dd v(4.3343, 0.4252, -5.5233);
    const Vec3Dd v_n = v.normalized();

    const Point3Dd p0_expected = p;
    const Point3Dd p1_expected = p + t * v_n;

    const Line3Dd line(p, v);

    const Line3Dd line_fp = lineFromTwoPoints(p0_expected, p1_expected);

    const Point3Dd p0_actual = evalLine3D(line, 0.0);
    const Point3Dd p1_actual = evalLine3D(line, t);*/

    // ASSERT_NEAR(p1_actual.x, p1_expected.x, eps);
    // ASSERT_NEAR(p1_actual.y, p1_expected.y, eps);
    // ASSERT_NEAR(p1_actual.z, p1_expected.z, eps);
}

TEST_F(Line3DTest, LineNormalVector)
{
    /*const double eps = 1e-7;

    double t = 0.23523;

    for (size_t k = 0; k < 10; k++)
    {
        const Point3Dd p(0.4124, 2.1262, -5.2324);
        const Vec3Dd v(4.3343, 0.4252, -5.5233);
        const Vec3Dd v_n = v.normalized();

        // line.vectorNormalToLine();
    }*/

    // ASSERT_NEAR(p1_actual.x, p1_expected.x, eps);
    // ASSERT_NEAR(p1_actual.y, p1_expected.y, eps);
    // ASSERT_NEAR(p1_actual.z, p1_expected.z, eps);
}

}  // namespace
}  // namespace arl
