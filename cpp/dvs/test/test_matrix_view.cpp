#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "dvs.h"

using namespace dvs;

using Mat = Matrix<int>;

class TestMatrixView : public testing::Test
{
protected:

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestMatrixView, TestBasicAssignment)
{
    const Mat m0 = {{211, 212, 213, 214, 215, 216},
                    {221, 222, 223, 224, 225, 226},
                    {231, 232, 233, 234, 235, 236},
                    {241, 242, 243, 244, 245, 246},
                    {251, 252, 253, 254, 255, 256},
                    {261, 262, 263, 264, 265, 266},
                    {271, 272, 273, 274, 275, 276}};
    Mat m_actual_assigned = {{411, 412, 413},
                             {421, 422, 423},
                             {431, 432, 433}};
    const size_t nr = 3, nc = 2;

    for(size_t r = 0; r < 5; r += 2)
    {
        for(size_t c = 0; c < 5; c += 2)
        {
            const Mat m_actual_new = m0({r, r + nr}, {c, c + nc});
            m_actual_assigned = m0({r, r + nr}, {c, c + nc});

            for(size_t _r = 0; _r < nr; _r += 2)
            {
                for(size_t _c = 0; _c < nc; _c += 2)
                {
                    ASSERT_EQ(m0(r + _r, c + _c), m_actual_new(_r, _c));
                    ASSERT_EQ(m0(r + _r, c + _c), m_actual_assigned(_r, _c));
                }
            }
        }
    }
}


TEST_F(TestMatrixView, TestBasic2)
{
    Mat m0 = {{211, 212, 213, 214, 215, 216},
              {221, 222, 223, 224, 225, 226},
              {231, 232, 233, 234, 235, 236},
              {241, 242, 243, 244, 245, 246},
              {251, 252, 253, 254, 255, 256},
              {261, 262, 263, 264, 265, 266},
              {271, 272, 273, 274, 275, 276}};

    Mat m_actual(10, 10);
    m_actual.fill(0);

    // m_actual({1, 4}, {1, 4}) = m0({2, 5}, {3, 6});

    const IndexSpan row_span(2, 5);
    const IndexSpan col_span(3, 6);

    const size_t nr = 3, nc = 3;

    for(size_t r = 0; r < 5; r += 2)
    {
        for(size_t c = 0; c < 5; c += 2)
        {
            m_actual.fill(0);
            const IndexSpan inner_row_span(r, r + nr);
            const IndexSpan inner_col_span(c, c + nc);
            m_actual(inner_row_span, inner_col_span) = m0(row_span, col_span);

            std::cout << m_actual << std::endl;

            size_t r0 = 0, c0 = 0;

            for(size_t _r = 0; _r < m_actual.rows(); _r++)
            {
                for(size_t _c = 0; _c < m_actual.cols(); _c++)
                {
                    if(inner_row_span.isIn(_r) && inner_col_span.isIn(_c))
                    {
                        ASSERT_EQ(m0(r0, c0), m_actual(_r, _c));
                        c0++;
                    }
                    else
                    {
                        ASSERT_EQ(m_actual(_r, _c), 0);
                    }
                }
                if(inner_row_span.isIn(_r))
                {
                    c0 = 0;
                    r0++;
                }
            }
        }
    }

}
