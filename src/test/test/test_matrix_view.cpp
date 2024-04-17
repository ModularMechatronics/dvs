#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "duoplot/duoplot.h"

using namespace duoplot;

using Mat = Matrix<int>;

const Mat m_base = {{211, 212, 213, 214, 215, 216},
                    {221, 222, 223, 224, 225, 226},
                    {231, 232, 233, 234, 235, 236},
                    {241, 242, 243, 244, 245, 246},
                    {251, 252, 253, 254, 255, 256},
                    {261, 262, 263, 264, 265, 266},
                    {271, 272, 273, 274, 275, 276}};

class TestMatrixView : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestMatrixView, TestBasicAssignment)
{
    Mat m_actual_assigned = {{411, 412, 413}, {421, 422, 423}, {431, 432, 433}};
    const size_t nr = 3, nc = 2;

    for (size_t r = 0; r < 5; r += 2)
    {
        for (size_t c = 0; c < 5; c += 2)
        {
            const Mat m_actual_new = m_base({r, r + nr}, {c, c + nc});
            m_actual_assigned = m_base({r, r + nr}, {c, c + nc});

            for (size_t _r = 0; _r < nr; _r += 2)
            {
                for (size_t _c = 0; _c < nc; _c += 2)
                {
                    ASSERT_EQ(m_base(r + _r, c + _c), m_actual_new(_r, _c));
                    ASSERT_EQ(m_base(r + _r, c + _c), m_actual_assigned(_r, _c));
                }
            }
        }
    }
}

TEST_F(TestMatrixView, TestIndexedAssignment)
{
    Mat m_actual(10, 10);
    m_actual.fill(0);

    const IndexSpan row_span(2, 5);
    const IndexSpan col_span(3, 6);

    const size_t nr = 3, nc = 3;

    for (size_t r = 0; r < 5; r += 2)
    {
        for (size_t c = 0; c < 5; c += 2)
        {
            m_actual.fill(0);
            const IndexSpan inner_row_span(r, r + nr);
            const IndexSpan inner_col_span(c, c + nc);
            m_actual(inner_row_span, inner_col_span) = m_base(row_span, col_span);

            for (size_t _r = 0; _r < m_actual.rows(); _r++)
            {
                for (size_t _c = 0; _c < m_actual.cols(); _c++)
                {
                    if (inner_row_span.isIn(_r) && inner_col_span.isIn(_c))
                    {
                        const size_t row_idx = _r - inner_row_span.from + row_span.from;
                        const size_t col_idx = _c - inner_col_span.from + col_span.from;
                        const int expected_value = m_base(row_idx, col_idx);
                        ASSERT_EQ(expected_value, m_actual(_r, _c));
                    }
                    else
                    {
                        ASSERT_EQ(m_actual(_r, _c), 0);
                    }
                }
            }
        }
    }
}

TEST_F(TestMatrixView, TestEnd)
{
    for (size_t r = 0; r < m_base.rows(); r++)
    {
        ASSERT_EQ(m_base(r, End), m_base(r, m_base.cols() - 1));
    }
    for (size_t c = 0; c < m_base.cols(); c++)
    {
        ASSERT_EQ(m_base(End, c), m_base(m_base.rows() - 1, c));
    }
}

TEST_F(TestMatrixView, TestEndWithSpan)
{
    Mat m_actual_assign;

    for (size_t r = 0; r < 5; r++)
    {
        for (int o = 0; o < 6; o++)
        {
            const Mat m_actual_new = m_base(IndexSpan(r, r + 2), End - o);
            m_actual_assign = m_base(IndexSpan(r, r + 2), End - o);

            const int col_idx = m_base.cols() + (End - o).offset - 1;

            ASSERT_EQ(m_base(r, col_idx), m_actual_new(0, 0));
            ASSERT_EQ(m_base(r + 1, col_idx), m_actual_new(1, 0));

            ASSERT_EQ(m_base(r, col_idx), m_actual_assign(0, 0));
            ASSERT_EQ(m_base(r + 1, col_idx), m_actual_assign(1, 0));
        }
    }
}

TEST_F(TestMatrixView, TestAllWithIndexSpan)
{
    for (size_t r = 0; r < 6; r++)
    {
        const Mat m_actual_new = m_base(IndexSpan(r, r + 2), All);
        for (size_t c = 0; c < m_base.cols(); c++)
        {
            ASSERT_EQ(m_actual_new(0, c), m_base(r, c));
            ASSERT_EQ(m_actual_new(1, c), m_base(r + 1, c));
        }
    }

    for (size_t c = 0; c < 5; c++)
    {
        const Mat m_actual_new = m_base(All, IndexSpan(c, c + 2));
        for (size_t r = 0; r < m_base.rows(); r++)
        {
            ASSERT_EQ(m_actual_new(r, 0), m_base(r, c));
            ASSERT_EQ(m_actual_new(r, 1), m_base(r, c + 1));
        }
    }
}

TEST_F(TestMatrixView, TestAllWithScalar)
{
    Mat m_actual_assign;

    for (size_t r = 0; r < m_base.rows(); r++)
    {
        m_actual_assign = m_base(r, All);
        const Mat m_actual_new = m_base(r, All);
        for (size_t c = 0; c < m_base.cols(); c++)
        {
            ASSERT_EQ(m_base(r, c), m_actual_assign(0, c));
            ASSERT_EQ(m_base(r, c), m_actual_new(0, c));
        }
    }

    for (size_t c = 0; c < m_base.cols(); c++)
    {
        m_actual_assign = m_base(All, c);
        const Mat m_actual_new = m_base(All, c);
        ;
        for (size_t r = 0; r < m_base.rows(); r++)
        {
            ASSERT_EQ(m_base(r, c), m_actual_assign(r, 0));
            ASSERT_EQ(m_base(r, c), m_actual_new(r, 0));
        }
    }
}
