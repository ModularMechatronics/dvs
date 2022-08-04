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
class StructuresTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(StructuresTest, Interval1DMapping)
{
    const size_t num_test_points = 1000;

    const Interval1D<double> target_interval(10.3, 14.8);
    const Interval1D<double> value_interval(0.7, 5.6);

    const double inc = value_interval.length() / static_cast<double>(num_test_points - 1);

    for (double val = value_interval.from; val <= value_interval.to; val = val + inc)
    {
        const double mapped_val = mapValueToInterval(val, value_interval, target_interval);
        ASSERT_TRUE(target_interval.isInClosed(mapped_val));
    }
}

TEST_F(StructuresTest, TestOutsideInterval)
{
    const std::vector<double> test_points = {-1.0, 0.0, 10.3 - 0.000001, 14.8 + 0.000001, 15.0};
    const Interval1D<double> target_interval(10.3, 14.8);
    const Interval1D<double> value_interval(0.7, 5.6);

    for (const double val : test_points)
    {
        const double mapped_val = mapValueToInterval(val, value_interval, target_interval);
        ASSERT_FALSE(target_interval.isInClosed(mapped_val));
    }
}

TEST_F(StructuresTest, TestOnLimits)
{
    const double lower_bound = 10.3, upper_bound = 14.8;

    const Interval1D<double> interval(lower_bound, upper_bound);

    ASSERT_TRUE(interval.isInClosed(lower_bound));
    ASSERT_TRUE(interval.isInClosed(upper_bound));

    ASSERT_FALSE(interval.isInOpen(lower_bound));
    ASSERT_FALSE(interval.isInOpen(upper_bound));

    ASSERT_TRUE(interval.isInRightOpen(lower_bound));
    ASSERT_FALSE(interval.isInRightOpen(upper_bound));

    ASSERT_FALSE(interval.isInLeftOpen(lower_bound));
    ASSERT_TRUE(interval.isInLeftOpen(upper_bound));
}

TEST_F(StructuresTest, TestOperators)
{
    std::vector<double> test_values = {-20.0, -14.0, -0.1, 0.0, 2.0, 15.0, 23.0};
    const double lower_bound = 10.3, upper_bound = 14.8;

    const Interval1D<double> interval(lower_bound, upper_bound);

    for (size_t k = 0; k < test_values.size(); k++)
    {
        const double val = test_values[k];

        const Interval1D<double> interval_sub0 = interval - val;
        const Interval1D<double> interval_sub1 = val - interval;

        const Interval1D<double> interval_add0 = interval + val;
        const Interval1D<double> interval_add1 = val + interval;

        const Interval1D<double> interval_mul0 = interval * val;
        const Interval1D<double> interval_mul1 = val * interval;

        const Interval1D<double> interval_div = interval / val;

        ASSERT_EQ(interval_sub0.from, interval.from - val);
        ASSERT_EQ(interval_sub0.to, interval.to - val);

        ASSERT_EQ(interval_sub1.from, val - interval.from);
        ASSERT_EQ(interval_sub1.to, val - interval.to);

        ASSERT_EQ(interval_add0.from, interval.from + val);
        ASSERT_EQ(interval_add0.to, interval.to + val);

        ASSERT_EQ(interval_add1.from, val + interval.from);
        ASSERT_EQ(interval_add1.to, val + interval.to);

        ASSERT_EQ(interval_mul0.from, interval.from * val);
        ASSERT_EQ(interval_mul0.to, interval.to * val);

        ASSERT_EQ(interval_mul1.from, val * interval.from);
        ASSERT_EQ(interval_mul1.to, val * interval.to);

        ASSERT_EQ(interval_div.from, interval.from / val);
        ASSERT_EQ(interval_div.to, interval.to / val);
    }
}

}  // namespace
}  // namespace arl
