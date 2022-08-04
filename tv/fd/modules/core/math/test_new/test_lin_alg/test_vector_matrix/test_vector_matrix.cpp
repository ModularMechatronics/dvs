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
class VectorMatrixTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(VectorMatrixTest, Initialization)
{
    /*// Create uninitialized
    const Vectord v_uninitialized;
    ASSERT_FALSE(v_uninitialized.isAllocated());

    const Vectord v_initialized(5);
    ASSERT_TRUE(v_initialized.isAllocated());
    ASSERT_TRUE(v_initialized.size() == 5);

    const Vectord v_copy = v_initialized;
    ASSERT_TRUE(v_copy.isAllocated());
    ASSERT_TRUE(v_copy.size() == 5);

    Vectord v_to_be_copied;
    ASSERT_FALSE(v_to_be_copied.isAllocated());
    v_to_be_copied = v_initialized;
    ASSERT_TRUE(v_to_be_copied.isAllocated());*/
}

}  // namespace
}  // namespace arl
