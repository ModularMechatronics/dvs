#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "arl/math.h"
// #include "arl/math/new_functions.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class GeneralMatrixMathFunctionsTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(GeneralMatrixMathFunctionsTest, MatrixMin)
{
    const double min_exp = -1.0;

    const Matrixd mat = {{1, 6, -1}, {7, 4, 13}, {9, 2, 0}};

    const double min_act = mat.min();
    ASSERT_EQ(min_exp, min_act);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixMax)
{
    const double max_exp = 13.0;

    const Matrixd mat = {{1, 6, 3}, {7, 4, 13}, {9, 2, 0}};

    const double max_act = mat.max();
    ASSERT_EQ(max_exp, max_act);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixMinAlongCols)
{
    const double eps = 1e-8;

    const Matrixd mat = {{1, 6, 1, 4}, {7, 4, 3, 6}, {9, 2, 0, 7}};

    const Matrixd min_mat_act = mat.minAlongCols();
    const Matrixd min_mat_exp = {{1, 2, 0, 4}};
    ASSERT_MATRIX_NEAR_MATRIX(min_mat_exp, min_mat_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixMinAlongRows)
{
    const double eps = 1e-8;

    const Matrixd mat = {{1, 6, 2, 4}, {7, 4, 3, 6}, {9, 2, 0, 7}};

    const Matrixd min_mat_act = mat.minAlongRows();
    const Matrixd min_mat_exp = {{1}, {3}, {0}};
    ASSERT_MATRIX_NEAR_MATRIX(min_mat_exp, min_mat_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixMaxAlongCols)
{
    const double eps = 1e-8;

    const Matrixd mat = {{1, 6, 1, 4}, {7, 4, 3, 6}, {9, 2, 0, 7}};

    const Matrixd max_mat_act = mat.maxAlongCols();
    const Matrixd max_mat_exp = {{9, 6, 3, 7}};
    ASSERT_MATRIX_NEAR_MATRIX(max_mat_exp, max_mat_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixMaxAlongRows)
{
    const double eps = 1e-8;

    const Matrixd mat = {{1, 6, 2, 4}, {7, 4, 3, 6}, {9, 2, 0, 7}};

    const Matrixd max_mat_act = mat.maxAlongRows();
    const Matrixd max_mat_exp = {{6}, {7}, {9}};
    ASSERT_MATRIX_NEAR_MATRIX(max_mat_exp, max_mat_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixSum)
{
    const double eps = 1e-8;

    const Matrixd mat = {{1, 6, 2, 4}, {7, 4, 3, 6}, {9, 2, 0, 7}};

    const double sum_act = mat.sum();
    const double sum_exp = 51;
    ASSERT_NEAR(sum_exp, sum_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixSumAlongCols)
{
    const double eps = 1e-8;

    const Matrixd mat = {{1, 6, 2, 4}, {7, 4, 3, 6}, {9, 2, 0, 7}};

    const Matrixd sum_mat_act = mat.sumAlongCols();
    const Matrixd sum_mat_exp = {{17, 12, 5, 17}};
    ASSERT_MATRIX_NEAR_MATRIX(sum_mat_exp, sum_mat_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatrixSumAlongRows)
{
    const double eps = 1e-8;

    const Matrixd mat = {{1, 6, 2, 4}, {7, 4, 3, 6}, {9, 2, 0, 7}};

    const Matrixd sum_mat_act = mat.sumAlongRows();
    const Matrixd sum_mat_exp = {{13}, {20}, {18}};
    ASSERT_MATRIX_NEAR_MATRIX(sum_mat_exp, sum_mat_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, MatConcatenate)
{
    const double eps = 1e-7;

    const Matrixd m0({{4, 2}, {1, 5}});
    const Matrixd m1({{3, 5, 1, 4, 3}, {6, 2, 4, 2, 1}});
    const Matrixd m2({{6, 3, 4}, {0, 4, 3}});
    const Matrixd m3({{6}, {3}});
    const Matrixd m_exp({{4, 2, 3, 5, 1, 4, 3, 6, 3, 4, 6}, {1, 5, 6, 2, 4, 2, 1, 0, 4, 3, 3}});

    const Matrixd m_act = concatenateHorizontally({m0, m1, m2, m3});
    ASSERT_MATRIX_NEAR_MATRIX(m_exp, m_act, eps);
}

TEST_F(GeneralMatrixMathFunctionsTest, HorizontalConcatenationMatrix)
{
    Matrix<int> m0 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix<int> m1 = {{10, 20, 30, 40}, {40, 50, 60, 70}, {70, 80, 90, 10}};
    Matrix<int> m2 = {
        {100, 200, 300, 400, 500}, {400, 500, 600, 700, 800}, {700, 800, 900, 100, 200}};
    Matrix<int> m3 = {{1000, 2000, 3000, 4000, 5000, 6000},
                      {4000, 5000, 6000, 7000, 8000, 9000},
                      {7000, 8000, 9000, 1000, 2000, 3000}};

    auto m_res = hCatMatrices(m0, m1, m2, m3);

    ASSERT_EQ(m0.cols() + m1.cols() + m2.cols() + m3.cols(), m_res.cols());
    ASSERT_EQ(m0.rows(), m_res.rows());
}

TEST_F(GeneralMatrixMathFunctionsTest, VerticalConcatenationMatrix)
{
    Matrix<int> m0 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrix<int> m1 = {{10, 20, 30}, {40, 50, 60}, {70, 80, 90}, {40, 30, 70}};
    Matrix<int> m2 = {
        {100, 200, 300}, {400, 500, 600}, {700, 800, 900}, {400, 100, 600}, {900, 500, 200}};
    Matrix<int> m3 = {{1000, 2000, 3000},
                      {4000, 5000, 6000},
                      {7000, 8000, 9000},
                      {2000, 5000, 1000},
                      {7000, 4000, 2000},
                      {9000, 3000, 4000}};

    auto m_res = vCatMatrices(m0, m1, m2, m3);

    ASSERT_EQ(m0.rows() + m1.rows() + m2.rows() + m3.rows(), m_res.rows());
    ASSERT_EQ(m0.cols(), m_res.cols());
}

}  // namespace
}  // namespace arl
