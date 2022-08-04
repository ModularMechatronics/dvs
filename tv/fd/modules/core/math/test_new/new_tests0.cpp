#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "arl/math.h"
#include "arl/math/general_matrix_math_functions.h"
#include "arl/math/matrix_fixed.h"
#include "arl/math/matrix_vector_fixed.h"
#include "arl/math/new_functions.h"
#include "arl/math/vector_fixed.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class NewTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(NewTest, Vec3DAxisAngleBetweenVecs)
{
    /*const double eps = 1e-8;

    // clang-format off
    const Matrixd tm = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1},
                        {1, 1, 0}, {0, 1, 1}, {1, 0, 1},
                        {1, 0, 1}, {1, 1, 0}, {0, 1, 1},
                        {1, 1, 1}};
    const Vectord v_angles = linspaceFromPointsAndCount(-0.01, 3.0 * M_PI, 5);
    const Vec3Dd v0(0.5, 1.2, -0.2);
    // clang-format on

    // for (size_t r = 0; r < tm.rows(); r++)
    {
        // const Vec3Dd vtn(tm(r, 0), tm(r, 1), tm(r, 2));
        const size_t r = 0;
        const Vec3Dd vtn(tm(r, 0), tm(r, 1), tm(r, 2));
        for (size_t c = 0; c < v_angles.size(); c++)
        {
            const AxisAngled ax_exp(v_angles(c), vtn.x, vtn.y, vtn.z);
            const Matrixd rot_mat = ax_exp.toRotationMatrix();

            const Vec3Dd v1 = rot_mat * v0;

            const AxisAngled ax_act = findAxisAngleBetweenVectors(v0, v1);

            // const double angle_between_vecs = v0.angleBetweenVectors(v1);
            const double angle_between_vecs = angleBetweenVectors(v1, v0);
            // std::cout << "[ " << v_angles(c) << ", " << angle_between_vecs << " ]" << std::endl;
            // clang-format off
            // std::cout << "[ " << ax_exp.phi << ", " << ax_exp.x << ", " << ax_exp.y << ", " <<
    ax_exp.z << " ]" << std::endl;
            // std::cout << "[ " << ax_act.phi << ", " << ax_act.x << ", " << ax_act.y << ", " <<
    ax_act.z << " ]" << std::endl << std::endl;
            // std::cout << "m_exp: " << rot_mat << std::endl;
            // std::cout << "m_act: " << ax_act.toRotationMatrix() << std::endl;

            // clang-format on
        }
    }*/
}

TEST_F(NewTest, ConcatenateMatricesVertically)
{
    const double eps = 1e-8;
    const Matrixd m0 = {{1, 2, 3}};
    const Matrixd m1 = {{4, 5, 6}, {7, 8, 9}};
    const Matrixd m2 = {{10, 11, 12}, {13, 14, 15}, {16, 17, 18}};
    const Matrixd m3 = {{19, 20, 21}, {22, 23, 24}, {25, 26, 27}, {28, 29, 30}};

    const Matrixd m_exp = {{1, 2, 3},
                           {4, 5, 6},
                           {7, 8, 9},
                           {10, 11, 12},
                           {13, 14, 15},
                           {16, 17, 18},
                           {19, 20, 21},
                           {22, 23, 24},
                           {25, 26, 27},
                           {28, 29, 30}};
    const Matrixd m_act = concatenateVertically({m0, m1, m2, m3});

    ASSERT_MATRIX_NEAR_MATRIX(m_exp, m_act, eps);
}

TEST_F(NewTest, ConcatenateMatricesHorizontally)
{
    const double eps = 1e-8;
    // clang-format off
    const Matrixd m0 = {{1},
                        {2},
                        {3}};
    const Matrixd m1 = {{4, 7},
                        {5, 8},
                        {6, 9}};
    const Matrixd m2 = {{10, 13, 16},
                        {11, 14, 17},
                        {12, 15, 18}};
    const Matrixd m3 = {{19, 22, 25, 28},
                        {20, 23, 26, 29},
                        {21, 24, 27, 30}};

    const Matrixd m_exp = {{1, 4, 7, 10, 13, 16, 19, 22, 25, 28},
                           {2, 5, 8, 11, 14, 17, 20, 23, 26, 29},
                           {3, 6, 9, 12, 15, 18, 21, 24, 27, 30}};
    // clang-format off
    const Matrixd m_act = concatenateHorizontally({m0, m1, m2, m3});

    ASSERT_MATRIX_NEAR_MATRIX(m_exp, m_act, eps);
}

TEST_F(NewTest, MatrixFixedTestHCat)
{
    const double eps = 1e-8;
    const MatrixFixed<3, 3, double> m0 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    const MatrixFixed<3, 4, double> m1 = {{10, 11, 12, 13}, {14, 15, 16, 17}, {18, 19, 20, 21}};
    const MatrixFixed<3, 7, double> mres_exp = {{1, 2, 3, 10, 11, 12, 13},
                                                {4, 5, 6, 14, 15, 16, 17},
                                                {7, 8, 9, 18, 19, 20, 21}};
    auto mres_act = hCatFixed(m0, m1);

    ASSERT_FIXED_MATRIX_NEAR_FIXED_MATRIX(mres_exp, mres_act, eps);
}

TEST_F(NewTest, MatrixFixedTestVCat)
{
    const double eps = 1e-8;
    const MatrixFixed<3, 3, double> m0 = {{1, 2, 3},
                                          {4, 5, 6},
                                          {7, 8, 9}};
    const MatrixFixed<4, 3, double> m1 = {{10, 11, 12},
                                          {13, 14, 15},
                                          {16, 17, 18},
                                          {19, 20, 21}};
    const MatrixFixed<7, 3, double> mres_exp = {{1, 2, 3},
                                                {4, 5, 6},
                                                {7, 8, 9},
                                                {10, 11, 12},
                                                {13, 14, 15},
                                                {16, 17, 18},
                                                {19, 20, 21}};
    auto mres_act = vCatFixed(m0, m1);
    ASSERT_FIXED_MATRIX_NEAR_FIXED_MATRIX(mres_exp, mres_act, eps);
}


TEST_F(NewTest, MatrixFixedTestMultiplication)
{
    const double eps = 1e-8;
    const MatrixFixed<3, 4, double> m0 = {{1, 2, 3, 6},
                                          {4, 5, 6, 2},
                                          {7, 8, 9, 1}};
    const MatrixFixed<4, 3, double> m1 = {{10, 11, 12},
                                          {13, 14, 15},
                                          {16, 17, 18},
                                          {19, 20, 21}};
    const MatrixFixed<3, 3, double> mres_exp = {{198, 210, 222},
                                                {239, 256, 273},
                                                {337, 362, 387}};
    auto mres_act = m0*m1;

    ASSERT_FIXED_MATRIX_NEAR_FIXED_MATRIX(mres_exp, mres_act, eps);
}

TEST_F(NewTest, MatrixFixedTestOtherOperatorsMatrixMatrix)
{
    const double eps = 1e-8;
    const MatrixFixed<5, 6, double> m0 = {{4, 1, 7, 4, 2, 6},
                                          {9, 4, 3, 7, 0, 2},
                                          {0, 1, 4, 2, 4, 6},
                                          {5, 3, 7, 5, 3, 5},
                                          {8, 6, 3, 9, 5, 1}};
    const MatrixFixed<5, 6, double> m1 = {{6, 3, 2, 5, 3, 7},
                                          {1, 4, 2, 6, 3, 6},
                                          {1, 2, 7, 4, 7, 4},
                                          {9, 4, 6, 2, 4, 9},
                                          {4, 1, 5, 3, 2, 4}};
    auto mres_add = m0 + m1;
    auto mres_sub = m0 - m1;
    auto mres_elem_div = m0 / m1;
    auto mres_elem_mul = m0 ^ m1;
    auto mres_negative = -m0;

    for(size_t r = 0; r < m0.rows(); r++)
    {
        for(size_t c = 0; c < m0.cols(); c++)
        {
            ASSERT_NEAR(mres_add(r, c), m0(r, c) + m1(r, c), eps);
            ASSERT_NEAR(mres_sub(r, c), m0(r, c) - m1(r, c), eps);
            ASSERT_NEAR(mres_elem_div(r, c), m0(r, c) / m1(r, c), eps);
            ASSERT_NEAR(mres_elem_mul(r, c), m0(r, c) * m1(r, c), eps);
            ASSERT_NEAR(mres_negative(r, c), -m0(r, c), eps);
        }
    }
}


TEST_F(NewTest, MatrixFixedTestOtherOperatorsMatrixScalar)
{
    const double eps = 1e-8;
    const MatrixFixed<5, 6, double> m0 = {{4, 1, 7, 4, 2, 6},
                                          {9, 4, 3, 7, 5, 2},
                                          {3, 1, 4, 2, 4, 6},
                                          {5, 3, 7, 5, 3, 5},
                                          {8, 6, 3, 9, 5, 1}};
    const double f = 0.1241;
    auto mres_add0 = m0 + f;
    auto mres_add1 = f + m0;

    auto mres_sub0 = m0 - f;
    auto mres_sub1 = f - m0;
    auto mres_mul0 = m0 * f;
    auto mres_mul1 = f * m0;
    auto mres_div0 = m0 / f;
    auto mres_div1 = f / m0;

    for(size_t r = 0; r < m0.rows(); r++)
    {
        for(size_t c = 0; c < m0.cols(); c++)
        {
            ASSERT_NEAR(mres_add0(r, c), m0(r, c) + f, eps);
            ASSERT_NEAR(mres_add1(r, c), m0(r, c) + f, eps);

            ASSERT_NEAR(mres_sub0(r, c), m0(r, c) - f, eps);
            ASSERT_NEAR(mres_sub1(r, c), f - m0(r, c), eps);
        
            ASSERT_NEAR(mres_mul0(r, c), m0(r, c) * f, eps);
            ASSERT_NEAR(mres_mul1(r, c), m0(r, c) * f, eps);

            ASSERT_NEAR(mres_div0(r, c), m0(r, c) / f, eps);
            ASSERT_NEAR(mres_div1(r, c), f / m0(r, c), eps);

        }
    }

}

TEST_F(NewTest, CircleFromPoints)
{
    const double eps = 1e-8;
    const double radiuses[5] = {1.0, 2.3, 3.6, 5.5, 7.4};
    const double angles[5][3] = {{0.1, 0.4, -0.5},
                                 {3.2, -0.7, 0.2},
                                 {5.2, 3.2, 4.7},
                                 {4.5, 0.6, -1.5},
                                 {0.2, 0.5, -2.3}};
    const double offsets[5][2] = {{0.3, 0.1}, {-0.4, 4}, {0.3, 0.5}, {2.0, 3.0}, {-1.0, 2.0}};

    for(size_t k = 0; k < 5; k++)
    {
        
        const double radius = radiuses[k];
        const double a0 = angles[k][0];
        const double a1 = angles[k][1];
        const double a2 = angles[k][2];
        const Point2Dd center_point(offsets[k][0], offsets[k][1]);
        const Point2Dd p0 = radius * Point2Dd(std::cos(a0), std::sin(a0)) + center_point;
        const Point2Dd p1 = radius * Point2Dd(std::cos(a1), std::sin(a1)) + center_point;
        const Point2Dd p2 = radius * Point2Dd(std::cos(a2), std::sin(a2)) + center_point;

        const Circle2Dd circle_act = circleFromThreePoints(p0, p1, p2);

        ASSERT_NEAR(radius, circle_act.r, eps);
        ASSERT_NEAR(circle_act.c.x, center_point.x, eps);
        ASSERT_NEAR(circle_act.c.y, center_point.y, eps);

    }
}

}  // namespace
}  // namespace arl
