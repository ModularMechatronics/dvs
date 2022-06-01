#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "arl/math.h"
#include "arl/math/lin_alg/matrix_fixed/matrix_fixed.h"
#include "arl/math/lin_alg/matrix_fixed/matrix_fixed_functions.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class MatrixFixedTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(MatrixFixedTest, HorizontalConcatenationMatrix)
{
    MatrixFixed<3, 3, int> m0 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    MatrixFixed<3, 4, int> m1 = {{10, 20, 30, 40}, {40, 50, 60, 70}, {70, 80, 90, 10}};
    MatrixFixed<3, 5, int> m2 = {
        {100, 200, 300, 400, 500}, {400, 500, 600, 700, 800}, {700, 800, 900, 100, 200}};
    MatrixFixed<3, 6, int> m3 = {{1000, 2000, 3000, 4000, 5000, 6000},
                                 {4000, 5000, 6000, 7000, 8000, 9000},
                                 {7000, 8000, 9000, 1000, 2000, 3000}};

    auto m_res = hCatFixedMatrices(m0, m1, m2, m3);
    PRINT() << m_res;
}

TEST_F(MatrixFixedTest, VerticalConcatenationMatrix)
{
    MatrixFixed<3, 3, int> m0 = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
    MatrixFixed<4, 3, int> m1 = {{10, 20, 30}, {40, 50, 60}, {70, 80, 90}, {40, 30, 70}};
    MatrixFixed<5, 3, int> m2 = {
        {100, 200, 300}, {400, 500, 600}, {700, 800, 900}, {400, 100, 600}, {900, 500, 200}};
    MatrixFixed<6, 3, int> m3 = {{1000, 2000, 3000},
                                 {4000, 5000, 6000},
                                 {7000, 8000, 9000},
                                 {2000, 5000, 1000},
                                 {7000, 4000, 2000},
                                 {9000, 3000, 4000}};

    auto m_res = vCatFixedMatrices(m0, m1, m2, m3);
    PRINT() << m_res;
}

}  // namespace
}  // namespace arl
