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
class CircleTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(CircleTest, Initialization)
{
    double r = 0.2523;
    double x = -2.1039;
    double y = 3.4235;
    Circle2Dd c0(r, x, y);
}

}  // namespace
}  // namespace arl
