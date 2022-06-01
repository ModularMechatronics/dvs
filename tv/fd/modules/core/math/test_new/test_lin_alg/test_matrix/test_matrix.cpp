#include <gtest/gtest.h>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "arl/math.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class MatrixTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(MatrixTest, Initialization)
{
    Matrixd m2(2, 2);
    Matrixd m3(3, 3);
    Matrixd m4(4, 4);

    for (size_t r = 2; r < 10; r++)
    {
        for (size_t c = 2; c < 10; c++)
        {
            Matrixd m(r, c);
            ASSERT_TRUE(m.isAllocated());
            ASSERT_TRUE(m.rows() == r);
            ASSERT_TRUE(m.cols() == c);
        }
    }

    ASSERT_TRUE(m2.isAllocated());
    ASSERT_TRUE(m3.isAllocated());
    ASSERT_TRUE(m4.isAllocated());

    ASSERT_TRUE(m2.rows() == 2 && m2.cols() == 2);
    ASSERT_TRUE(m3.rows() == 3 && m3.cols() == 3);
    ASSERT_TRUE(m4.rows() == 4 && m4.cols() == 4);
}

TEST_F(MatrixTest, AssignmentOperator)
{
    double eps = 1e-7;

    const Matrixd m{{1, 2, 3}, {6, 2, 3}, {8, 3, 1}};
    Matrixd m_act(4, 7);

    m_act = m;

    for (size_t r = 0; r < m.rows(); r++)
    {
        for (size_t c = 0; c < m.cols(); c++)
        {
            ASSERT_NEAR(m(r, c), m_act(r, c), eps);
        }
    }
}

TEST_F(MatrixTest, CopyConstructor)
{
    double eps = 1e-7;

    const Matrixd m{{1, 2, 3}, {6, 2, 3}, {8, 3, 1}};
    const Matrixd m_act(m);

    for (size_t r = 0; r < m.rows(); r++)
    {
        for (size_t c = 0; c < m.cols(); c++)
        {
            ASSERT_NEAR(m(r, c), m_act(r, c), eps);
        }
    }
}

TEST_F(MatrixTest, MatrixFill)
{
    double eps = 1e-7;

    for (size_t r = 1; r < 10; r++)
    {
        for (size_t c = 1; c < 10; c++)
        {
            Matrixd m(r, c);
            const double f = (3 + r) * (5 + c) * 4;
            m.fill(f);
            for (size_t r_ = 0; r_ < r; r_++)
            {
                for (size_t c_ = 0; c_ < c; c_++)
                {
                    ASSERT_NEAR(m(r_, c_), f, eps);
                }
            }
        }
    }
}

TEST_F(MatrixTest, ExtractArbitraryMatrixTest)
{
    Matrixd m{{0.5507979026, 0.8962930889, 0.02987621088, 0.5908628174, 0.2835250818},
              {0.7081478226, 0.1255853105, 0.4568332244, 0.02398188238, 0.6931379183},
              {0.2909047389, 0.2072428781, 0.6491440476, 0.558854088, 0.4404537177},
              {0.5108276052, 0.0514672033, 0.2784872826, 0.2592524469, 0.1568677385},
              {0.8929469543, 0.4408098437, 0.676254902, 0.415101197, 0.544649018}};
    for (size_t row_idx = 0; row_idx < 3; row_idx++)
    {
        for (size_t col_idx = 0; col_idx < 3; col_idx++)
        {
            for (size_t num_rows = 1; num_rows < 3; num_rows++)
            {
                for (size_t num_cols = 1; num_cols < 3; num_cols++)
                {
                    // const size_t rows = std::min(m.lastRowIdx(), row_idx + num_rows);
                    // const size_t cols = std::min(m.lastColIdx(), col_idx + num_cols);
                    // Matrixd msub = m.extractArbitraryMatrix(row_idx, col_idx, rows, cols);
                }
            }
        }
    }
    // NOT FINISHED!
}

TEST_F(MatrixTest, GetDataPointer)
{
    double eps = 1e-6;
    Matrixd m(3, 3);
    m(2, 2) = 0.3;
    ASSERT_NEAR(m(2, 2), 0.3, eps);

    double* p = m.getDataPointer();

    ASSERT_NEAR(p[8], 0.3, eps);
    p[8] = 1.2;

    ASSERT_NEAR(p[8], 1.2, eps);
}

TEST_F(MatrixTest, GetColumnVector)
{
    double eps = 1e-6;
    Matrixd m{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
    Vectord v0_exp{0, 3, 6};
    Vectord v1_exp{1, 4, 7};
    Vectord v2_exp{2, 5, 8};
    Vectord v0_actual = m.getColumnAsVector(0);
    Vectord v1_actual = m.getColumnAsVector(1);
    Vectord v2_actual = m.getColumnAsVector(2);

    ASSERT_VECTOR_NEAR_VECTOR(v0_exp, v0_actual, eps);
}

TEST_F(MatrixTest, GetRowVector)
{
    double eps = 1e-6;
    Matrixd m{{0, 1, 2}, {3, 4, 5}, {6, 7, 8}};
    Vectord v0_exp{0, 1, 2};
    Vectord v1_exp{3, 4, 5};
    Vectord v2_exp{6, 7, 8};
    Vectord v0_actual = m.getRowAsVector(0);
    Vectord v1_actual = m.getRowAsVector(1);
    Vectord v2_actual = m.getRowAsVector(2);

    ASSERT_VECTOR_NEAR_VECTOR(v0_exp, v0_actual, eps);
}

TEST_F(MatrixTest, GetColumnRowVectorOutOfBounds)
{
    Matrixd m(3, 3);
    ASSERT_DEATH(Vectord v0_actual = m.getRowAsVector(-1), "");
    ASSERT_DEATH(Vectord v0_actual = m.getRowAsVector(3), "");
    ASSERT_DEATH(Vectord v0_actual = m.getColumnAsVector(-1), "");
    ASSERT_DEATH(Vectord v0_actual = m.getColumnAsVector(3), "");
}

TEST_F(MatrixTest, SwitchColumns)
{
    double eps = 1e-6;
    // clang-format off
    Matrixd m{{0, 1, 2},
              {3, 4, 5},
              {6, 7, 8}};
    Matrixd m_exp{{2, 1, 0},
                  {5, 4, 3},
                  {8, 7, 6}};
    // clang-format on
    m.switchColumns(0, 2);

    ASSERT_MATRIX_NEAR_MATRIX(m, m_exp, eps);
}

TEST_F(MatrixTest, SwitchRows)
{
    double eps = 1e-6;
    // clang-format off
    Matrixd m{{0, 1, 2},
              {3, 4, 5},
              {6, 7, 8}};
    Matrixd m_exp{{6, 7, 8},
                  {3, 4, 5},
                  {0, 1, 2}};
    // clang-format on
    m.switchRows(0, 2);

    ASSERT_MATRIX_NEAR_MATRIX(m, m_exp, eps);
}

TEST_F(MatrixTest, StdVectorInitialization)
{
    double eps = 1e-7;
    std::vector<std::vector<double>> mv = {{1, 2, 3}, {6, 2, 3}, {8, 3, 1}};

    Matrixd m(mv);

    for (size_t r = 0; r < mv.size(); r++)
    {
        for (size_t c = 0; c < mv[r].size(); c++)
        {
            ASSERT_NEAR(m(r, c), mv[r][c], eps);
        }
    }
}

TEST_F(MatrixTest, InitializerListInitialization)
{
    double eps = 1e-7;
    // clang-format off
    Matrixd m{{0.5507979026, 0.8962930889, 0.02987621088, 0.5908628174},
              {0.7081478226, 0.1255853105, 0.4568332244, 0.02398188238},
              {0.2909047389, 0.2072428781, 0.6491440476, 0.558854088},
              {0.5108276052, 0.0514672033, 0.2784872826, 0.2592524469},
              {0.8929469543, 0.4408098437, 0.676254902, 0.415101197}};
    std::vector<std::vector<double>> mv = {{0.5507979026, 0.8962930889, 0.02987621088, 0.5908628174},
                                           {0.7081478226, 0.1255853105, 0.4568332244, 0.02398188238},
                                           {0.2909047389, 0.2072428781, 0.6491440476, 0.558854088},
                                           {0.5108276052, 0.0514672033, 0.2784872826, 0.2592524469},
                                           {0.8929469543, 0.4408098437, 0.676254902, 0.415101197}};
    // clang-format on

    for (size_t r = 0; r < mv.size(); r++)
    {
        for (size_t c = 0; c < mv[r].size(); c++)
        {
            ASSERT_NEAR(m(r, c), mv[r][c], eps);
        }
    }
}

TEST_F(MatrixTest, MatrixScalarOperations)
{
    const double m_array[20] = {0.5507979026, 0.8962930889, 0.02987621088, 0.5908628174,
                                0.7081478226, 0.1255853105, 0.4568332244,  0.02398188238,
                                0.2909047389, 0.2072428781, 0.6491440476,  0.558854088,
                                0.5108276052, 0.0514672033, 0.2784872826,  0.2592524469,
                                0.8929469543, 0.4408098437, 0.676254902,   0.415101197};

    Matrixd m0(4, 5);
    Matrixd m1(5, 4);

    const double eps = 1e-8;

    fillMatrixWithArrayRowMajor(m0, m_array);
    fillMatrixWithArrayColMajor(m1, m_array);

    const double f = 1.14251;

    Matrixd m0_add_left = f + m0;
    Matrixd m0_add_right = m0 + f;

    Matrixd m0_sub_left = f - m0;
    Matrixd m0_sub_right = m0 - f;

    Matrixd m0_mul_left = f * m0;
    Matrixd m0_mul_right = m0 * f;

    Matrixd m0_div_left = f / m0;
    Matrixd m0_div_right = m0 / f;

    Matrixd m0_negated = -m0;

    Matrixd m1_add_left = f + m1;
    Matrixd m1_add_right = m1 + f;

    Matrixd m1_sub_left = f - m1;
    Matrixd m1_sub_right = m1 - f;

    Matrixd m1_mul_left = f * m1;
    Matrixd m1_mul_right = m1 * f;

    Matrixd m1_div_left = f / m1;
    Matrixd m1_div_right = m1 / f;

    Matrixd m1_negated = -m1;

    for (size_t r = 0; r < 4; r++)
    {
        for (size_t c = 0; c < 5; c++)
        {
            ASSERT_NEAR(f + m0(r, c), m0_add_left(r, c), eps);
            ASSERT_NEAR(f + m0(r, c), m0_add_right(r, c), eps);
            ASSERT_NEAR(f - m0(r, c), m0_sub_left(r, c), eps);
            ASSERT_NEAR(m0(r, c) - f, m0_sub_right(r, c), eps);
            ASSERT_NEAR(f * m0(r, c), m0_mul_left(r, c), eps);
            ASSERT_NEAR(f * m0(r, c), m0_mul_right(r, c), eps);
            ASSERT_NEAR(f / m0(r, c), m0_div_left(r, c), eps);
            ASSERT_NEAR(m0(r, c) / f, m0_div_right(r, c), eps);
        }
    }

    for (size_t r = 0; r < 5; r++)
    {
        for (size_t c = 0; c < 4; c++)
        {
            ASSERT_NEAR(f + m1(r, c), m1_add_left(r, c), eps);
            ASSERT_NEAR(f + m1(r, c), m1_add_right(r, c), eps);
            ASSERT_NEAR(f - m1(r, c), m1_sub_left(r, c), eps);
            ASSERT_NEAR(m1(r, c) - f, m1_sub_right(r, c), eps);
            ASSERT_NEAR(f * m1(r, c), m1_mul_left(r, c), eps);
            ASSERT_NEAR(f * m1(r, c), m1_mul_right(r, c), eps);
            ASSERT_NEAR(f / m1(r, c), m1_div_left(r, c), eps);
            ASSERT_NEAR(m1(r, c) / f, m1_div_right(r, c), eps);
        }
    }
}

TEST_F(MatrixTest, MatrixAdditionWithMatrix)
{
    const double m0_array[12] = {0.5507979026,
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
    const double m1_array[12] = {0.6491440476,
                                 0.5908628174,
                                 0.2592524469,
                                 0.6931379183,
                                 0.2784872826,
                                 0.02398188238,
                                 0.415101197,
                                 0.4404537177,
                                 0.676254902,
                                 0.558854088,
                                 0.2835250818,
                                 0.1568677385};
    const double mres_array_add[12] = {1.19994195,
                                       1.101690423,
                                       0.3848377574,
                                       1.133947762,
                                       0.9866351053,
                                       0.9169288367,
                                       0.6223440751,
                                       0.4703299285,
                                       0.9671596409,
                                       1.455147177,
                                       0.3349922851,
                                       0.6137009629};
    const double mres_array_sub[12] = {-0.09834614504,
                                       -0.08003521222,
                                       -0.1336671364,
                                       -0.2523280747,
                                       0.42966054,
                                       0.868965072,
                                       -0.2078583189,
                                       -0.4105775068,
                                       -0.3853501631,
                                       0.3374390009,
                                       -0.2320578785,
                                       0.2999654859};
    Matrixd m0(3, 4);
    Matrixd m1(3, 4);
    Matrixd mres_add_expected(3, 4);
    Matrixd mres_sub_expected(3, 4);

    const double eps = 1e-8;

    fillMatrixWithArrayRowMajor(m0, m0_array);
    fillMatrixWithArrayRowMajor(m1, m1_array);
    fillMatrixWithArrayRowMajor(mres_add_expected, mres_array_add);
    fillMatrixWithArrayRowMajor(mres_sub_expected, mres_array_sub);

    Matrixd mres_add_actual = m0 + m1;
    Matrixd mres_sub_actual = m0 - m1;

    ASSERT_MATRIX_NEAR_MATRIX(mres_add_expected, mres_add_actual, eps);
    ASSERT_MATRIX_NEAR_MATRIX(mres_sub_expected, mres_sub_actual, eps);
}

TEST_F(MatrixTest, MatrixMultiplicationWithMatrix)
{
    const double m0_array[12] = {0.5507979026,
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
    const double m1_array[20] = {0.6491440476, 0.02398188238, 0.2835250818, 0.544649018,
                                 0.3879712576, 0.2784872826,  0.558854088,  0.6931379183,
                                 0.7803147645, 0.9363836499,  0.676254902,  0.2592524469,
                                 0.4404537177, 0.3063635324,  0.9759954225, 0.5908628174,
                                 0.415101197,  0.1568677385,  0.2219578839, 0.6723836759};
    const double mres_array[15] = {0.8451919996,
                                   0.5142262587,
                                   0.6347023634,
                                   0.8349138387,
                                   1.110988404,
                                   0.8661660691,
                                   0.5821396477,
                                   0.9156805722,
                                   1.152594629,
                                   1.333238306,
                                   0.7431760211,
                                   0.7108485167,
                                   0.7980648316,
                                   0.974997121,
                                   1.309535829};

    Matrixd m0(3, 4);
    Matrixd m1(4, 5);
    Matrixd mres_expected(3, 5);

    const double eps = 1e-8;

    fillMatrixWithArrayRowMajor(m0, m0_array);
    fillMatrixWithArrayRowMajor(m1, m1_array);
    fillMatrixWithArrayRowMajor(mres_expected, mres_array);

    Matrixd mres_actual = m0 * m1;

    ASSERT_MATRIX_NEAR_MATRIX(mres_expected, mres_actual, eps);
}

TEST_F(MatrixTest, FilledUnitOnesZerosMatrices)
{
    const double eps = 1e-8;

    const size_t m_sizes_rows[5] = {1, 5, 2, 7, 4};
    const size_t m_sizes_cols[5] = {2, 3, 4, 3, 8};

    const double fill_val = 2.54234;

    for (size_t k = 0; k < 5; k++)
    {
        const size_t rows = m_sizes_rows[k];
        const size_t cols = m_sizes_cols[k];
        Matrixd m_filled = filledMatrix(rows, cols, fill_val);
        Matrixd m_zeros = zerosMatrix<double>(rows, cols);
        Matrixd m_ones = onesMatrix<double>(rows, cols);
        Matrixd m_unit = unitMatrix<double>(rows, cols);

        ASSERT_TRUE(m_filled.rows() == rows && m_filled.cols() == cols);
        ASSERT_TRUE(m_zeros.rows() == rows && m_zeros.cols() == cols);
        ASSERT_TRUE(m_ones.rows() == rows && m_ones.cols() == cols);
        ASSERT_TRUE(m_unit.rows() == rows && m_unit.cols() == cols);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(m_filled(r, c), fill_val, eps);
                ASSERT_NEAR(m_zeros(r, c), 0.0, eps);
                ASSERT_NEAR(m_ones(r, c), 1.0, eps);
                if (r == c)
                {
                    ASSERT_NEAR(m_unit(r, c), 1.0, eps);
                }
                else
                {
                    ASSERT_NEAR(m_unit(r, c), 0.0, eps);
                }
            }
        }
    }
}

TEST_F(MatrixTest, AccessOperator)
{
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

    const double eps = 1e-8;
    Matrixd m0(3, 4);

    fillMatrixWithArrayRowMajor(m0, m_array);

    for (size_t r = 0; r < 3; r++)
    {
        for (size_t c = 0; c < 4; c++)
        {
            ASSERT_NEAR(m0(r, c), m_array[r * 4 + c], eps);
        }
    }

    Matrixd m1(3, 4);
    for (size_t r = 0; r < 3; r++)
    {
        for (size_t c = 0; c < 4; c++)
        {
            m1(r, c) = (r + 2) * (c + 3);
        }
    }

    for (size_t r = 0; r < 3; r++)
    {
        for (size_t c = 0; c < 4; c++)
        {
            ASSERT_NEAR(m1(r, c), (r + 2) * (c + 3), eps);
        }
    }
}

TEST_F(MatrixTest, MatrixTranspose)
{
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

    const double eps = 1e-8;
    Matrixd m(3, 4);

    fillMatrixWithArrayRowMajor(m, m_array);

    Matrixd m_transpose = m.getTranspose();
    ASSERT_TRUE(m_transpose.rows() == 4 && m_transpose.cols() == 3);
    for (size_t r = 0; r < 4; r++)
    {
        for (size_t c = 0; c < 3; c++)
        {
            ASSERT_NEAR(m_transpose(r, c), m(c, r), eps);
        }
    }
}

TEST_F(MatrixTest, MatrixMatrixHorizontalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = k + 1;
        const size_t cols0 = (k + 1) * 2;
        const size_t cols1 = (k + 2) * 3;

        Matrixd m0(rows, cols0);
        Matrixd m1(rows, cols1);

        for (size_t r = 0; r < m0.rows(); r++)
        {
            for (size_t c = 0; c < m0.cols(); c++)
            {
                const double rd = static_cast<double>(r);
                const double cd = static_cast<double>(c);
                m0(r, c) = (rd + 2) * cd + 7;
            }
        }

        for (size_t r = 0; r < m1.rows(); r++)
        {
            for (size_t c = 0; c < m1.cols(); c++)
            {
                const double rd = static_cast<double>(r);
                const double cd = static_cast<double>(c);
                m1(r, c) = (rd + 3) * (cd - 1) + 4;
            }
        }

        const Matrixd mres = hCat(m0, m1);

        ASSERT_EQ(mres.rows(), rows);
        ASSERT_EQ(mres.cols(), cols0 + cols1);

        for (size_t r = 0; r < m0.rows(); r++)
        {
            for (size_t c = 0; c < m0.cols(); c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        for (size_t r = 0; r < mres.rows(); r++)
        {
            for (size_t c = m0.cols(); c < mres.cols(); c++)
            {
                ASSERT_NEAR(mres(r, c), m1(r, c - m0.cols()), eps);
            }
        }
    }
}

TEST_F(MatrixTest, MatrixMatrixVerticalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t cols = k + 1;
        const size_t rows0 = (k + 1) * 2;
        const size_t rows1 = (k + 2) * 3;

        Matrixd m0(rows0, cols);
        Matrixd m1(rows1, cols);

        for (size_t r = 0; r < m0.rows(); r++)
        {
            for (size_t c = 0; c < m0.cols(); c++)
            {
                const double rd = static_cast<double>(r);
                const double cd = static_cast<double>(c);
                m0(r, c) = (rd + 2) * cd + 7;
            }
        }

        for (size_t r = 0; r < m1.rows(); r++)
        {
            for (size_t c = 0; c < m1.cols(); c++)
            {
                const double rd = static_cast<double>(r);
                const double cd = static_cast<double>(c);
                m1(r, c) = (rd + 3) * (cd - 1) + 4;
            }
        }

        Matrixd mres = vCat(m0, m1);

        ASSERT_EQ(mres.rows(), rows0 + rows1);
        ASSERT_EQ(mres.cols(), cols);

        for (size_t r = 0; r < m0.rows(); r++)
        {
            for (size_t c = 0; c < m0.cols(); c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        for (size_t r = m0.rows(); r < mres.rows(); r++)
        {
            for (size_t c = 0; c < mres.cols(); c++)
            {
                ASSERT_NEAR(mres(r, c), m1(r - m0.rows(), c), eps);
            }
        }
    }
}

TEST_F(MatrixTest, MatrixVectorVerticalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 2 * k + 3;
        const size_t cols = 5 * k + 7;

        Matrixd m0(rows, cols);
        Vectord v(cols);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        for (size_t c = 0; c < cols; c++)
        {
            v(c) = (c + 1) * 4 + 15;
        }

        Matrixd mres = vCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        for (size_t c = 0; c < cols; c++)
        {
            ASSERT_NEAR(v(c), mres(mres.lastRowIdx(), c), eps);
        }
    }
}

TEST_F(MatrixTest, MatrixVectorHorizontalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 2 * k + 3;
        const size_t cols = 5 * k + 7;

        Matrixd m0(rows, cols);
        Vectord v(rows);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        for (size_t r = 0; r < rows; r++)
        {
            v(r) = (r + 1) * 4 + 15;
        }

        Matrixd mres = hCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        for (size_t r = 0; r < rows; r++)
        {
            ASSERT_NEAR(v(r), mres(r, mres.lastColIdx()), eps);
        }
    }
}

TEST_F(MatrixTest, MatrixVec4DHorizontalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 4;
        const size_t cols = 5 * k + 7;

        Matrixd m0(rows, cols);
        Vec4Dd v;

        v.x = 2.1523;
        v.y = 0.3453;
        v.z = -2.4312;
        v.w = 20.235;

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        Matrixd mres = hCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        ASSERT_NEAR(v.x, mres(0, mres.lastColIdx()), eps);
        ASSERT_NEAR(v.y, mres(1, mres.lastColIdx()), eps);
        ASSERT_NEAR(v.z, mres(2, mres.lastColIdx()), eps);
        ASSERT_NEAR(v.w, mres(3, mres.lastColIdx()), eps);
    }
}

TEST_F(MatrixTest, MatrixVec4DVerticalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 5 * k + 7;
        const size_t cols = 4;

        Matrixd m0(rows, cols);
        Vec4Dd v;

        v.x = 2.1523;
        v.y = 0.3453;
        v.z = -2.4312;
        v.w = 20.235;

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        Matrixd mres = vCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        ASSERT_NEAR(v.x, mres(mres.lastRowIdx(), 0), eps);
        ASSERT_NEAR(v.y, mres(mres.lastRowIdx(), 1), eps);
        ASSERT_NEAR(v.z, mres(mres.lastRowIdx(), 2), eps);
        ASSERT_NEAR(v.w, mres(mres.lastRowIdx(), 3), eps);
    }
}

TEST_F(MatrixTest, MatrixVec3DHorizontalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 3;
        const size_t cols = 5 * k + 7;

        Matrixd m0(rows, cols);
        Vec3Dd v;

        v.x = 2.1523;
        v.y = 0.3453;
        v.z = -2.4312;

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        Matrixd mres = hCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        ASSERT_NEAR(v.x, mres(0, mres.lastColIdx()), eps);
        ASSERT_NEAR(v.y, mres(1, mres.lastColIdx()), eps);
        ASSERT_NEAR(v.z, mres(2, mres.lastColIdx()), eps);
    }
}

TEST_F(MatrixTest, MatrixVec3DVerticalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 5 * k + 7;
        const size_t cols = 3;

        Matrixd m0(rows, cols);
        Vec3Dd v;

        v.x = 2.1523;
        v.y = 0.3453;
        v.z = -2.4312;

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        Matrixd mres = vCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        ASSERT_NEAR(v.x, mres(mres.lastRowIdx(), 0), eps);
        ASSERT_NEAR(v.y, mres(mres.lastRowIdx(), 1), eps);
        ASSERT_NEAR(v.z, mres(mres.lastRowIdx(), 2), eps);
    }
}

TEST_F(MatrixTest, MatrixVec2DHorizontalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 2;
        const size_t cols = 5 * k + 7;

        Matrixd m0(rows, cols);
        Vec2Dd v;

        v.x = 2.1523;
        v.y = 0.3453;

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        Matrixd mres = hCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        ASSERT_NEAR(v.x, mres(0, mres.lastColIdx()), eps);
        ASSERT_NEAR(v.y, mres(1, mres.lastColIdx()), eps);
    }
}

TEST_F(MatrixTest, MatrixVec2DVerticalCat)
{
    const double eps = 1e-8;

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = 5 * k + 7;
        const size_t cols = 2;

        Matrixd m0(rows, cols);
        Vec2Dd v;

        v.x = 2.1523;
        v.y = 0.3453;

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m0(r, c) = 2 * (r + 1) * (3 * (c + 1) + 2) + 18;
            }
        }

        Matrixd mres = vCat(m0, v);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                ASSERT_NEAR(mres(r, c), m0(r, c), eps);
            }
        }

        ASSERT_NEAR(v.x, mres(mres.lastRowIdx(), 0), eps);
        ASSERT_NEAR(v.y, mres(mres.lastRowIdx(), 1), eps);
    }
}

TEST_F(MatrixTest, MatrixInverse)
{
    const double eps = 1e-8;

    std::srand(0);

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = k + 2;
        const size_t cols = k + 2;

        Matrixd m(rows, cols);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m(r, c) = static_cast<double>(std::rand() % 1001) / 500.0;
            }
        }

        Matrixd minv = m.inverse();

        Matrixd mres = minv * m;
        Matrixd unit_mat = unitMatrix<double>(rows, cols);
        ASSERT_MATRIX_NEAR_MATRIX(mres, unit_mat, eps);
    }
}

TEST_F(MatrixTest, MatrixSelfInverse)
{
    const double eps = 1e-8;

    std::srand(0);

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = k + 2;
        const size_t cols = k + 2;

        Matrixd m(rows, cols);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m(r, c) = static_cast<double>(std::rand() % 1001) / 500.0;
            }
        }

        const Matrixd m_copy = m;
        m.invert();

        Matrixd mres = m * m_copy;
        Matrixd unit_mat = unitMatrix<double>(rows, cols);
        ASSERT_MATRIX_NEAR_MATRIX(mres, unit_mat, eps);
    }
}

TEST_F(MatrixTest, MatrixVectorSolve)
{
    const double eps = 1e-8;

    std::srand(0);

    for (size_t k = 0; k < 10; k++)
    {
        const size_t rows = k + 2;
        const size_t cols = k + 2;

        Matrixd m(rows, cols);
        Vectord x_expected(rows);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m(r, c) = static_cast<double>(std::rand() % 1001) / 500.0;
            }
            x_expected(r) = static_cast<double>(std::rand() % 1001) / 500.0;
        }
        Vectord b = m * x_expected;
        Vectord x_actual = m.solve(b);

        ASSERT_VECTOR_NEAR_VECTOR(x_expected, x_actual, eps);
    }
}

TEST_F(MatrixTest, MatrixQRDecomposition)
{
    /*const double eps = 1e-8;

    std::srand(0);

    for (size_t k = 0; k < 3; k++)
    {
        const size_t rows = k + 2;
        const size_t cols = k + 2;

        Matrixd m_expected(rows, cols);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m_expected(r, c) = static_cast<double>(std::rand() % 1001) / 500.0;
            }
        }

        QRMatrixPair<double> qr_pair = m_expected.qr();

        Matrixd m_actual = qr_pair.q * qr_pair.r;

        // std::cout << "m_actual: " << m_actual << std::endl;
        // std::cout << "m_expected: " << m_expected << std::endl;

        std::cout << "qr_pair.q: " << qr_pair.q << std::endl;
        std::cout << "qr_pair.r: " << qr_pair.r << std::endl;

        // ASSERT_VECTOR_NEAR_VECTOR(x_expected, x_actual, eps);
    }*/
}

TEST_F(MatrixTest, MatrixLUDecomposition)
{
    /*const double eps = 1e-8;

    std::srand(0);

    for (size_t k = 0; k < 4; k++)
    {
        const size_t rows = k + 2;
        const size_t cols = k + 2;

        Matrixd m_expected(rows, cols);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m_expected(r, c) = static_cast<double>(std::rand() % 1001) / 500.0;
            }
        }

        LUMatrixPair<double> lu_pair = m_expected.lu();

        Matrixd m_actual = lu_pair.l * lu_pair.u;

        // std::cout << "m_actual: " << std::endl << m_actual << std::endl;
        // std::cout << "m_expected: " << std::endl << m_expected << std::endl;

        // std::cout << "l: " << lu_pair.l << std::endl;
        // std::cout << "u: " << lu_pair.u << std::endl;

        // ASSERT_VECTOR_NEAR_VECTOR(x_expected, x_actual, eps);
    }*/
}

TEST_F(MatrixTest, MatrixSVDecomposition)
{
    const double eps = 1e-8;

    std::srand(0);

    for (size_t k = 0; k < 4; k++)
    {
        const size_t rows = k + 2;
        const size_t cols = k + 3;

        Matrixd m_expected(rows, cols);

        for (size_t r = 0; r < rows; r++)
        {
            for (size_t c = 0; c < cols; c++)
            {
                m_expected(r, c) = static_cast<double>(std::rand() % 1001) / 500.0;
            }
        }

        const SVDMatrixTriplet<double> svd_triplet = m_expected.svd();

        const Matrixd m_actual = svd_triplet.u * svd_triplet.s * svd_triplet.v.getTranspose();

        ASSERT_MATRIX_NEAR_MATRIX(m_expected, m_actual, eps);
    }
}

TEST_F(MatrixTest, MatrixHCat)
{
    const double eps = 1e-8;
    const Matrixd mref = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrixd m0 = mref;
    ASSERT_MATRIX_NEAR_MATRIX(m0, mref, eps);
    const Matrixd m1 = {{11, 12, 13, 64}, {14, 15, 16, 43}, {16, 17, 18, 32}};
    const Matrixd m_exp = {
        {1, 2, 3, 11, 12, 13, 64}, {4, 5, 6, 14, 15, 16, 43}, {7, 8, 9, 16, 17, 18, 32}};

    m0.hCat(m1);

    ASSERT_MATRIX_NEAR_MATRIX(m0, m_exp, eps);
}

TEST_F(MatrixTest, MatrixVCat)
{
    const double eps = 1e-8;
    const Matrixd mref = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    Matrixd m0 = mref;
    ASSERT_MATRIX_NEAR_MATRIX(m0, mref, eps);
    const Matrixd m1 = {{11, 12, 13}, {14, 15, 16}, {16, 17, 18}, {63, 43, 42}};
    const Matrixd m_exp = {
        {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {11, 12, 13}, {14, 15, 16}, {16, 17, 18}, {63, 43, 42}};
    m0.vCat(m1);

    ASSERT_MATRIX_NEAR_MATRIX(m0, m_exp, eps);
}

TEST_F(MatrixTest, MatrixHCatGeneral)
{
    const Vector<size_t> rows = {1, 2, 4, 7, 10, 15, 17, 25, 30};
    const Vector<size_t> cols0 = {1, 2, 4, 7, 10, 15, 17, 25, 30};
    const Vector<size_t> cols1 = {1, 2, 4, 7, 10, 15, 17, 25, 30};

    for (size_t r0 : rows)
    {
        for (size_t c0 : cols0)
        {
            for (size_t c1 : cols1)
            {
                Matrixd mref(r0, c0);
                Matrixd m0;
                Matrixd m1(r0, c1);

                for (size_t r = 0; r < r0; r++)
                {
                    for (size_t c = 0; c < c0; c++)
                    {
                        mref(r, c) = (r + 3) + (c * r + 1) * 3 + 112;
                    }
                }
                m0 = mref;
                ASSERT_MATRIX_EQ_MATRIX(m0, mref);
                for (size_t r = 0; r < r0; r++)
                {
                    for (size_t c = 0; c < c1; c++)
                    {
                        m1(r, c) = (r + 2) + (c * r - 1) * 2 + 11;
                    }
                }

                ASSERT_EQ(m0.rows(), r0);
                ASSERT_EQ(m0.cols(), c0);
                ASSERT_EQ(m1.rows(), r0);
                ASSERT_EQ(m1.cols(), c1);

                m0.hCat(m1);
                ASSERT_EQ(m0.rows(), r0);
                ASSERT_EQ(m0.cols(), c0 + m1.cols());
                for (size_t r = 0; r < r0; r++)
                {
                    for (size_t c = 0; c < c0; c++)
                    {
                        ASSERT_EQ(m0(r, c), mref(r, c));
                    }
                }
                for (size_t r = 0; r < r0; r++)
                {
                    for (size_t c = c0; c < (c0 + c1); c++)
                    {
                        ASSERT_EQ(m0(r, c), m1(r, c - c0));
                    }
                }
            }
        }
    }
}

TEST_F(MatrixTest, MatrixVCatGeneral)
{
    const Vector<size_t> cols = {1, 2, 4, 7, 10, 15, 17, 25, 30};
    const Vector<size_t> rows0 = {1, 2, 4, 7, 10, 15, 17, 25, 30};
    const Vector<size_t> rows1 = {1, 2, 4, 7, 10, 15, 17, 25, 30};

    for (size_t c0 : cols)
    {
        for (size_t r0 : rows0)
        {
            for (size_t r1 : rows1)
            {
                Matrixd mref(r0, c0);
                Matrixd m0;
                Matrixd m1(r1, c0);

                for (size_t r = 0; r < r0; r++)
                {
                    for (size_t c = 0; c < c0; c++)
                    {
                        mref(r, c) = (r + 3) + (c * r + 1) * 3 + 112;
                    }
                }
                m0 = mref;
                ASSERT_MATRIX_EQ_MATRIX(m0, mref);
                for (size_t r = 0; r < r1; r++)
                {
                    for (size_t c = 0; c < c0; c++)
                    {
                        m1(r, c) = (r + 2) + (c * r - 1) * 2 + 11;
                    }
                }

                ASSERT_EQ(m0.rows(), r0);
                ASSERT_EQ(m0.cols(), c0);
                ASSERT_EQ(m1.rows(), r1);
                ASSERT_EQ(m1.cols(), c0);

                m0.vCat(m1);
                ASSERT_EQ(m0.cols(), c0);
                ASSERT_EQ(m0.rows(), r0 + m1.rows());
                for (size_t r = 0; r < r0; r++)
                {
                    for (size_t c = 0; c < c0; c++)
                    {
                        ASSERT_EQ(m0(r, c), mref(r, c));
                    }
                }
                for (size_t r = r0; r < (r0 + r1); r++)
                {
                    for (size_t c = 0; c < c0; c++)
                    {
                        ASSERT_EQ(m0(r, c), m1(r - r0, c));
                    }
                }
            }
        }
    }
}

TEST_F(MatrixTest, MatrixSingleArgIndexing)
{
    /*const Matrix<int> m0 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 11, 12}};
    const Vector<int> v0 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    PRINT() << "rows: " << m0.rows();
    PRINT() << "cols: " << m0.cols();

    for (size_t k = 0; k < v0.size(); k++)
    {
        ASSERT_EQ(v0(k), m0(k));
    }*/
}

}  // namespace
}  // namespace arl
