#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "arl/math.h"
#include "arl/math/structures/lookup_grid.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class LookupGridTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(LookupGridTest, Test0)
{
    LookupGrid<double, Cell0> grid(0.1, 2.0, 3.0, 4.0, 0.3, 0.2);

    const Cell0& c0 = grid(0.754, 3.01);
}

}  // namespace
}  // namespace arl
