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
class MatrixInitTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(MatrixInitTest, MatrixMove)
{
    using Mat = Matrix<int>;
    const Mat mat_gt = {{1, 3, 6}, {3, 4, 1}, {0, 3, 1}, {1, 1, -3}};
    Mat mat0 = mat_gt, mat1 = mat_gt;

    const int* const ptr0 = mat0.data();
    const int* const ptr1 = mat1.data();

    const Mat mat_moved0 = std::move(mat0);
    Mat mat_moved1 = std::move(mat1);
    ASSERT_FALSE(mat0.isAllocated());
    ASSERT_FALSE(mat1.isAllocated());

    ASSERT_TRUE(mat_moved0.isAllocated());
    ASSERT_TRUE(mat_moved1.isAllocated());

    ASSERT_EQ(mat0.rows(), 0UL);
    ASSERT_EQ(mat1.rows(), 0UL);

    ASSERT_EQ(mat0.cols(), 0UL);
    ASSERT_EQ(mat1.cols(), 0UL);

    ASSERT_EQ(mat0.data(), nullptr);
    ASSERT_EQ(mat1.data(), nullptr);

    ASSERT_EQ(mat_moved0.rows(), mat_gt.rows());
    ASSERT_EQ(mat_moved1.rows(), mat_gt.rows());

    ASSERT_EQ(mat_moved0.cols(), mat_gt.cols());
    ASSERT_EQ(mat_moved1.cols(), mat_gt.cols());

    ASSERT_EQ(mat_moved0.data(), ptr0);
    ASSERT_EQ(mat_moved1.data(), ptr1);

    ASSERT_MATRIX_EQ_MATRIX(mat_moved0, mat_gt);
    ASSERT_MATRIX_EQ_MATRIX(mat_moved1, mat_gt);
}

TEST_F(MatrixInitTest, MatrixMoveInternal)
{
    using Mat = Matrix<int>;
    const Mat mat_gt = {{1, 3, 6}, {3, 4, 1}, {0, 3, 1}, {1, 1, -3}};

    Mat m0 = mat_gt, m1 = mat_gt, m2 = mat_gt;

    Mat m_res0 = m0.move();
    const Mat m_res2 = m2.move();
    Mat m_res1;

    m_res1 = m1.move();

    ASSERT_EQ(m0.rows(), 0UL);
    ASSERT_EQ(m0.cols(), 0UL);
    ASSERT_EQ(m0.data(), nullptr);

    ASSERT_EQ(m1.rows(), 0UL);
    ASSERT_EQ(m1.cols(), 0UL);
    ASSERT_EQ(m1.data(), nullptr);

    ASSERT_EQ(m2.rows(), 0UL);
    ASSERT_EQ(m2.cols(), 0UL);
    ASSERT_EQ(m2.data(), nullptr);

    ASSERT_MATRIX_EQ_MATRIX(m_res0, mat_gt);
    ASSERT_MATRIX_EQ_MATRIX(m_res1, mat_gt);
    ASSERT_MATRIX_EQ_MATRIX(m_res2, mat_gt);
}

}  // namespace
}  // namespace arl
