#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class VectorInitTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(VectorInitTest, VectorMove)
{
    using Vec = Vector<int>;
    const Vec vec_gt = {4, 1, 6, 3, 2, 5, 5, 11, 5, 1};

    Vec vec0 = vec_gt, vec1 = vec_gt;

    const int* const ptr0 = vec0.getDataPointer();
    const int* const ptr1 = vec1.getDataPointer();

    const Vec vec_moved0 = std::move(vec0);
    Vec vec_moved1;
    vec_moved1 = std::move(vec1);

    ASSERT_FALSE(vec0.isAllocated());
    ASSERT_FALSE(vec1.isAllocated());

    ASSERT_TRUE(vec_moved0.isAllocated());
    ASSERT_TRUE(vec_moved1.isAllocated());

    ASSERT_EQ(vec0.size(), 0UL);
    ASSERT_EQ(vec0.getDataPointer(), nullptr);

    ASSERT_EQ(vec1.size(), 0UL);
    ASSERT_EQ(vec1.getDataPointer(), nullptr);

    ASSERT_EQ(vec_moved0.size(), vec_gt.size());
    ASSERT_EQ(vec_moved0.getDataPointer(), ptr0);
    ASSERT_EQ(vec_moved1.size(), vec_gt.size());
    ASSERT_EQ(vec_moved1.getDataPointer(), ptr1);

    ASSERT_VECTOR_EQ_VECTOR(vec_moved0, vec_gt);
    ASSERT_VECTOR_EQ_VECTOR(vec_moved1, vec_gt);
}

TEST_F(VectorInitTest, VectorPushFrontBack)
{
    using Vec = Vector<int>;
    const Vec v = {6, 1, 4, 2, 7, 5, 3};
    const Vec v_gt = {6, 1, 4, 2, 7, 5, 3, 9};
    const Vec v_gt2 = {-1, 6, 1, 4, 2, 7, 5, 3, 9};
    Vec v0 = v;

    v0.pushBack(9);

    ASSERT_VECTOR_EQ_VECTOR(v0, v_gt);

    v0.pushFront(-1);

    ASSERT_VECTOR_EQ_VECTOR(v0, v_gt2);
}

TEST_F(VectorInitTest, VectorPushFrontBackEmpty)
{
    using Vec = Vector<int>;

    const Vec v_gt = {6, 1, 4, 2};
    Vec v0, v1;

    v0.pushBack(6);
    v0.pushBack(1);
    v0.pushBack(4);
    v0.pushBack(2);

    v1.pushFront(2);
    v1.pushFront(4);
    v1.pushFront(1);
    v1.pushFront(6);

    ASSERT_VECTOR_EQ_VECTOR(v0, v_gt);
    ASSERT_VECTOR_EQ_VECTOR(v1, v_gt);
}

TEST_F(VectorInitTest, VectorInsertAtIndex)
{
    using Vec = Vector<int>;

    const Vec v = {6, 1, 4, 2, 8, 2};
    const Vec v_gt0 = {6, 1, 4, 2, -4, 8, 2};
    const Vec v_gt1 = {6, 1, -5, 4, 2, -4, 8, 2};
    const size_t idx0 = 4, idx1 = 2;

    Vec v0 = v;
    v0.insertAtIndex(-4, idx0);
    ASSERT_VECTOR_EQ_VECTOR(v0, v_gt0);

    v0.insertAtIndex(-5, idx1);
    ASSERT_VECTOR_EQ_VECTOR(v0, v_gt1);
}

TEST_F(VectorInitTest, VectorMoveInternal)
{
    using Vec = Vector<int>;
    const Vec vec_gt = {1, 3, 6, 3, 4, 1, 0, 3, 1, 1, 1, -3};

    Vec v0 = vec_gt, v1 = vec_gt, v2 = vec_gt;

    Vec v_res0 = v0.move();
    const Vec v_res2 = v2.move();
    Vec v_res1;

    v_res1 = v1.move();

    ASSERT_EQ(v0.size(), 0UL);
    ASSERT_EQ(v0.getDataPointer(), nullptr);

    ASSERT_EQ(v1.size(), 0UL);
    ASSERT_EQ(v1.getDataPointer(), nullptr);

    ASSERT_EQ(v2.size(), 0UL);
    ASSERT_EQ(v2.getDataPointer(), nullptr);

    ASSERT_VECTOR_EQ_VECTOR(v_res0, vec_gt);
    ASSERT_VECTOR_EQ_VECTOR(v_res1, vec_gt);
    ASSERT_VECTOR_EQ_VECTOR(v_res2, vec_gt);
}

}  // namespace
}  // namespace arl
