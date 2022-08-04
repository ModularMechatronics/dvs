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
class HomogeneousLine2DLine2DTest : public testing::Test
{
protected:
    std::vector<double> px_vec;
    std::vector<double> py_vec;
    std::vector<double> vx_vec;
    std::vector<double> vy_vec;
    std::vector<double> t_vec;

    const double test_a[8] = {0.2342, -1.381, -0.982, 2.523, 0.2342, -1.381, -0.982, 2.523};
    const double test_b[8] = {1.231, -0.903, 2.412, -0.124, 1.231, -0.903, 2.412, -0.124};
    const double test_c[8] = {0.251, 0.912, 3.212, 0.553, -0.251, -0.912, -3.212, -0.553};

    const double x_vec[10] = {
        -2.124, -1.141, -0.101, 0.0, 0.141, 1.624, 1.971, 2.012, 3.512, 9.124};
    const double y_vec[10] = {
        -1.224, -1.113, -0.112, 0.0, 0.122, 1.723, 1.992, 2.113, 4.612, 10.324};

    void SetUp() override
    {
        const size_t num_iterations = 10;
        px_vec.clear();
        py_vec.clear();
        vx_vec.clear();
        vy_vec.clear();
        t_vec.clear();
        double phi = 0.0;

        for (size_t k = 0; k < num_iterations; k++)
        {
            phi = 2.0 * M_PI * static_cast<double>(k) / static_cast<double>(num_iterations - 1) -
                  DEG2RAD(2.0);

            px_vec.push_back(std::cos(static_cast<double>(k)));
            py_vec.push_back(std::sin(static_cast<double>(k)));
            t_vec.push_back(std::sin(3.0 * static_cast<double>(k)));
            vx_vec.push_back(std::cos(phi));
            vy_vec.push_back(std::sin(phi));
        }
    }

    void TearDown() override {}
};

TEST_F(HomogeneousLine2DLine2DTest, Initialization)
{
    const double eps = 1e-7;

    for (size_t k = 0; k < 8; k++)
    {
        const double a = test_a[k];
        const double b = test_b[k];
        const double c = test_c[k];
        const HomogeneousLine2Dd line(a, b, c);

        ASSERT_NEAR(a, line.a, eps);
        ASSERT_NEAR(b, line.b, eps);
        ASSERT_NEAR(c, line.c, eps);
    }
}

TEST_F(HomogeneousLine2DLine2DTest, HomogenousFromParametricConstructor)
{
    const double eps = 1e-7;

    for (size_t k = 0; k < px_vec.size(); k++)
    {
        const double px = px_vec[k];
        const double py = py_vec[k];
        const double vx = vx_vec[k];
        const double vy = vy_vec[k];

        const ParametricLine2Dd pl(px, py, vx, vy);

        const HomogeneousLine2Dd line(pl);
        for (size_t i = 0; i < 10; i++)
        {
            const double x = x_vec[i];
            const double y = line.evalX(x);

            const double tx = pl.tFromX(x);

            const Point2Dd ptx = pl.eval(tx);
            const Point2Dd pty = pl.eval(tx);

            ASSERT_NEAR(ptx.x, x, eps);
            ASSERT_NEAR(ptx.y, y, eps);

            ASSERT_NEAR(pty.x, x, eps);
            ASSERT_NEAR(pty.y, y, eps);
        }
    }
}

TEST_F(HomogeneousLine2DLine2DTest, EvalXY)
{
    const double eps = 1e-7;
    const double x = 1.2;
    const double y = -0.5;

    const double a = 0.7;
    const double b = 1.5;
    const double c = 2.0;

    const HomogeneousLine2Dd line(a, b, c);

    const double y_actual = line.evalX(x);
    const double y_expected = -(a * x + c) / b;

    ASSERT_NEAR(y_actual, y_expected, eps);

    const double x_actual = line.evalY(y);
    const double x_expected = -(b * y + c) / a;

    ASSERT_NEAR(x_actual, x_expected, eps);
}

TEST_F(HomogeneousLine2DLine2DTest, EvalXYBackAndForth)
{
    const double eps = 1e-7;
    const double x = 1.2;

    const double a = 0.7;
    const double b = 1.5;
    const double c = 2.0;

    const HomogeneousLine2Dd line(a, b, c);

    const double y = line.evalX(x);
    const double x_actual = line.evalY(y);

    ASSERT_NEAR(x_actual, x, eps);
}

TEST_F(HomogeneousLine2DLine2DTest, NormalizeLine)
{
    const double eps = 1e-7;
    const double a = 0.151342;
    const double b = 1.541523;
    const double c = -1.925323;

    const HomogeneousLine2Dd line(a, b, c);
    const HomogeneousLine2Dd normalized_line = line.normalized();

    const double norm_fac = std::sqrt(line.a * line.a + line.b * line.b);

    const double px = 1.2;
    const double py = 5.2;
    const Point2Dd p(px, py);

    const double d_expected = line.eval(p);
    const double y_expected = line.evalX(px);
    const double x_expected = line.evalY(py);

    const double d_actual = normalized_line.eval(p);
    const double y_actual = normalized_line.evalX(px);
    const double x_actual = normalized_line.evalY(py);

    ASSERT_NEAR(x_expected, x_actual, eps);
    ASSERT_NEAR(y_expected, y_actual, eps);
    ASSERT_NEAR(norm_fac, d_expected / d_actual, eps);
}

TEST_F(HomogeneousLine2DLine2DTest, PointSideOfLine)
{
    const double a[4] = {2.0, -1.0, -2.0, 1.0};
    const double b[4] = {1.0, 2.0, -1.0, -2.0};
    const double c[4] = {0.0001, 0.0001, 0.0001, 0.0001};

    const double xl = 0.0;

    for (size_t k = 0; k < 4; k++)
    {
        const HomogeneousLine2Dd line(a[k], b[k], c[k]);
        const double yl = line.evalX(xl);
        Point2Dd point_on_line(xl, yl);
        Point2Dd point_on_positive_side = point_on_line + Point2Dd(a[k], b[k]) * 0.1;
        Point2Dd point_on_negative_side = point_on_line - Point2Dd(a[k], b[k]) * 0.1;
        ASSERT_TRUE(line.isOnNormalVectorSide(point_on_positive_side));
        ASSERT_FALSE(line.isOnNormalVectorSide(point_on_negative_side));
    }
}

TEST_F(HomogeneousLine2DLine2DTest, NegateLine)
{
    const double eps = 1e-7;
    const double a = 0.151342;
    const double b = 1.541523;
    const double c = -1.925323;

    const HomogeneousLine2Dd line(a, b, c);
    const HomogeneousLine2Dd negated_line = line.negated();

    ASSERT_NEAR(line.a, -negated_line.a, eps);
    ASSERT_NEAR(line.b, -negated_line.b, eps);
    ASSERT_NEAR(line.c, -negated_line.c, eps);

    const double px = 1.2;
    const double py = 5.2;
    const Point2Dd p(px, py);

    const double d_expected = line.eval(p);
    const double y_expected = line.evalX(px);
    const double x_expected = line.evalY(py);

    const double d_actual = negated_line.eval(p);
    const double y_actual = negated_line.evalX(px);
    const double x_actual = negated_line.evalY(py);

    ASSERT_NEAR(d_expected, -d_actual, eps);
    ASSERT_NEAR(x_expected, x_actual, eps);
    ASSERT_NEAR(y_expected, y_actual, eps);
}

TEST_F(HomogeneousLine2DLine2DTest, PointDistanceFromLine)
{
    const double eps = 1e-7;

    const double a = 0.151342;
    const double b = 1.541523;
    const double c = -1.925323;

    const double test_distances[5] = {0.25243, 0.8352, 1.5235, 23.62342, 1000.23523};

    const HomogeneousLine2Dd line(a, b, c);

    const double x_ol = 0.63462;
    const double y_ol = line.evalX(x_ol);
    const Point2Dd point_on_line(x_ol, y_ol);

    const Vec2Dd line_normal(line.a, line.b);
    const Vec2Dd line_normal_normalized = line_normal.normalized();

    for (size_t k = 0; k < 5; k++)
    {
        const Point2Dd p_pos = point_on_line + line_normal_normalized * test_distances[k];
        const Point2Dd p_neg = point_on_line - line_normal_normalized * test_distances[k];

        const double dist_pos_actual = line.pointDistanceFromLine(p_pos);
        const double dist_neg_actual = line.pointDistanceFromLine(p_neg);

        ASSERT_NEAR(dist_pos_actual, test_distances[k], eps);
        ASSERT_NEAR(dist_neg_actual, test_distances[k], eps);
    }
}

TEST_F(HomogeneousLine2DLine2DTest, ClosestPointOnLine)
{
    const double eps = 1e-7;

    const double a = 0.151342;
    const double b = 1.541523;
    const double c = -1.925323;

    const double test_distances[5] = {0.25243, 0.8352, 1.5235, 23.62342, 1000.23523};

    const HomogeneousLine2Dd line(a, b, c);

    const double x_ol = 0.63462;
    const double y_ol = line.evalX(x_ol);
    const Point2Dd point_on_line_expected(x_ol, y_ol);

    const Vec2Dd line_normal(line.a, line.b);
    const Vec2Dd line_normal_normalized = line_normal.normalized();

    for (size_t k = 0; k < 5; k++)
    {
        const Point2Dd p_pos = point_on_line_expected + line_normal_normalized * test_distances[k];
        const Point2Dd p_neg = point_on_line_expected - line_normal_normalized * test_distances[k];

        Point2Dd point_on_line_actual_pos = line.closestPointOnLineFromPoint(p_pos);
        Point2Dd point_on_line_actual_neg = line.closestPointOnLineFromPoint(p_neg);

        ASSERT_NEAR(point_on_line_expected.x, point_on_line_actual_pos.x, eps);
        ASSERT_NEAR(point_on_line_expected.y, point_on_line_actual_pos.y, eps);

        ASSERT_NEAR(point_on_line_expected.x, point_on_line_actual_neg.x, eps);
        ASSERT_NEAR(point_on_line_expected.y, point_on_line_actual_neg.y, eps);
    }
}

TEST_F(HomogeneousLine2DLine2DTest, LineFromPoints)
{
    const double eps = 1e-7;

    const double x0 = 0.523;
    const double x1 = 2.634;

    const double a = 0.151342;
    const double b = 1.541523;
    const double c = -1.925323;

    const HomogeneousLine2Dd line_expected(a, b, c);
    const HomogeneousLine2Dd line_expected_normalized = line_expected.normalized();

    const double y0 = line_expected.evalX(x0);
    const double y1 = line_expected.evalX(x1);

    const Point2Dd p0(x0, y0);
    const Point2Dd p1(x1, y1);

    HomogeneousLine2Dd line_actual = homogeneousLineFromPoints(p0, p1);
    const HomogeneousLine2Dd line_actual_normalized = line_actual.normalized();

    ASSERT_NEAR(line_expected_normalized.a, line_actual_normalized.a, eps);
    ASSERT_NEAR(line_expected_normalized.b, line_actual_normalized.b, eps);
    ASSERT_NEAR(line_expected_normalized.c, line_actual_normalized.c, eps);

    ASSERT_NEAR(line_expected_normalized.evalX(x0), line_actual_normalized.evalX(x0), eps);
    ASSERT_NEAR(line_expected_normalized.evalX(x1), line_actual_normalized.evalX(x1), eps);

    ASSERT_NEAR(line_expected_normalized.evalY(y0), line_actual_normalized.evalY(y0), eps);
    ASSERT_NEAR(line_expected_normalized.evalY(y1), line_actual_normalized.evalY(y1), eps);

    ASSERT_NEAR(line_expected_normalized.eval(p0), line_actual_normalized.eval(p0), eps);
    ASSERT_NEAR(line_expected_normalized.eval(p1), line_actual_normalized.eval(p1), eps);
}

TEST_F(HomogeneousLine2DLine2DTest, LineIntersection)
{
    const double eps = 1e-7;

    const Point2Dd intersection_expected(0.5, -3.2);
    const Vec2Dd v0(1.0, 2.4);
    const Vec2Dd v1(-0.3, 5.8);

    const Point2Dd p0 = intersection_expected + v0;
    const Point2Dd p1 = intersection_expected + v1;

    HomogeneousLine2Dd line0 = homogeneousLineFromPoints(intersection_expected, p0);
    HomogeneousLine2Dd line1 = homogeneousLineFromPoints(intersection_expected, p1);

    Point2Dd intersection_actual = line0.lineIntersection(line1);

    ASSERT_NEAR(intersection_actual.x, intersection_expected.x, eps);
    ASSERT_NEAR(intersection_actual.y, intersection_expected.y, eps);
}

TEST_F(HomogeneousLine2DLine2DTest, LineFromPointAndVector)
{
    const double eps = 1e-7;

    const Point2Dd p(0.5, -3.2);
    const Vec2Dd v(1.0, 2.4);

    const Point2Dd p0 = p;
    const Point2Dd p1 = p + v;

    HomogeneousLine2Dd line_expected = homogeneousLineFromPoints(p0, p1);

    HomogeneousLine2Dd line_actual = homogeneousLineFromPointAndVector(p, v);

    ASSERT_NEAR(line_expected.a, line_actual.a, eps);
    ASSERT_NEAR(line_expected.b, line_actual.b, eps);
    ASSERT_NEAR(line_expected.c, line_actual.c, eps);
}

TEST_F(HomogeneousLine2DLine2DTest, PointReflectionInLine)
{
    const double eps = 1e-7;

    Vec2Dd line_normal = Vec2Dd(0.5, -1.7).normalized();
    Vec2Dd vector_along_line = Vec2Dd(-line_normal.y, line_normal.x);
    Point2Dd center_point(2.7, 4.1);

    HomogeneousLine2Dd line = homogeneousLineFromPointAndVector(center_point, vector_along_line);

    Point2Dd p0 = center_point + line_normal;
    Point2Dd p_expected = center_point - line_normal;

    Point2Dd p_actual = line.pointReflection(p0);

    ASSERT_NEAR(p_expected.x, p_actual.x, eps);
    ASSERT_NEAR(p_expected.y, p_actual.y, eps);
}

TEST_F(HomogeneousLine2DLine2DTest, NormalAndVectorAlongLine)
{
    const double eps = 1e-7;

    Vec2Dd line_normal_original = Vec2Dd(1.0, 0.9).normalized();
    Point2Dd point;

    double angle = 0.0;
    Matrixd r_90 = rotationMatrix2D(M_PI / 2.0);
    for (size_t k = 0; k < 10; k++)
    {
        Matrixd r = rotationMatrix2D(angle);

        Vec2Dd line_normal = r * line_normal_original;
        Vec2Dd vector_along_line = r_90 * line_normal;

        if (line_normal.y < 0)
        {
            line_normal = -line_normal;
        }

        if (vector_along_line.x < 0)
        {
            vector_along_line = -vector_along_line;
        }

        HomogeneousLine2Dd line = homogeneousLineFromPointAndVector(point, vector_along_line);

        double dot_product = line.normal() * line.vectorAlongLine();
        ASSERT_NEAR(dot_product, 0.0, 1e-10);

        ASSERT_NEAR(line_normal.x, line.normal().x, eps);
        ASSERT_NEAR(line_normal.y, line.normal().y, eps);
        ASSERT_NEAR(vector_along_line.x, line.vectorAlongLine().x, eps);
        ASSERT_NEAR(vector_along_line.y, line.vectorAlongLine().y, eps);

        angle = angle + 0.7;
    }
}

TEST_F(HomogeneousLine2DLine2DTest, ParametricLineTFromXAndY)
{
    const double eps = 1e-7;
    for (size_t k = 0; k < vx_vec.size(); k++)
    {
        const ParametricLine2Dd pl(px_vec[k], py_vec[k], vx_vec[k], vy_vec[k]);

        for (size_t i = 0; i < t_vec.size(); i++)
        {
            const double t = t_vec[i];
            const Point2Dd p = pl.eval(t);

            const double tx = pl.tFromX(p.x);
            const double ty = pl.tFromY(p.y);

            ASSERT_NEAR(t, tx, eps);
            ASSERT_NEAR(t, ty, eps);
        }
    }
}

}  // namespace
}  // namespace arl
