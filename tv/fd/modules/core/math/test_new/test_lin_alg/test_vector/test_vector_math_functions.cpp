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
class GeneralMathFunctionsTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(GeneralMathFunctionsTest, MinMax)
{
    const Vectord v({1, 5, 2, -3, 2, 9, 6, 17, 4});
    const double min_exp = -3;
    const double max_exp = 17;

    ASSERT_EQ(min_exp, min(v));
    ASSERT_EQ(max_exp, max(v));
}

TEST_F(GeneralMathFunctionsTest, SinCos)
{
    const double eps = 1e-7;

    const size_t num_values = 27;
    const double x0 = 0.1;
    const double x1 = 1.2;
    double e = 1.312413;
    const Vectord v = linspaceFromPointsAndCount(x0, x1, num_values);
    const Vectord vsin = sin(v);
    const Vectord vcos = cos(v);
    const Vectord vlog = log(v);
    const Vectord vexp = exp(v);
    const Vectord vpow = pow(v, e);
    const Vectord vlog10 = log10(v);

    for (size_t k = 0; k < num_values; k++)
    {
        ASSERT_NEAR(vsin(k), std::sin(v(k)), eps);
        ASSERT_NEAR(vcos(k), std::cos(v(k)), eps);
        ASSERT_NEAR(vlog(k), std::log(v(k)), eps);
        ASSERT_NEAR(vexp(k), std::exp(v(k)), eps);
        ASSERT_NEAR(vpow(k), std::pow(v(k), e), eps);
        ASSERT_NEAR(vlog10(k), std::log10(v(k)), eps);
    }
}

TEST_F(GeneralMathFunctionsTest, linspaceFromPointsAndCount)
{
    const double eps = 1e-7;

    const size_t num_values = 7;
    const double x0 = 0.1;
    const double x1 = 1.2;
    const double dx = (x1 - x0) / static_cast<double>(num_values - 1);
    Vectord v_exp(num_values);

    v_exp(0) = x0;
    for (size_t k = 1; k < num_values; k++)
    {
        v_exp(k) = v_exp(k - 1) + dx;
    }

    const Vectord v_act = linspaceFromPointsAndCount(x0, x1, num_values);

    ASSERT_VECTOR_NEAR_VECTOR(v_exp, v_act, eps);
}

TEST_F(GeneralMathFunctionsTest, VecConcatenate)
{
    const double eps = 1e-7;

    const Vectord v0({4, 2, 1, 5, 3});
    const Vectord v1({3, 6});
    const Vectord v2({0, 3, 2, 5, 3, 5});
    const Vectord v3({1, 4, 2, 3});
    const Vectord v_exp({4, 2, 1, 5, 3, 3, 6, 0, 3, 2, 5, 3, 5, 1, 4, 2, 3});
    const Vectord v_act = concatenate({v0, v1, v2, v3});

    ASSERT_VECTOR_NEAR_VECTOR(v_exp, v_act, eps);
}

TEST_F(GeneralMathFunctionsTest, VecFind)
{
    const Vector<int> v0({0, 1, 0, 0, 2, 0, 1, -3, 0, 0, -1, 1, 0, 3, 0, 2});
    const Vector<size_t> v_exp({1, 4, 6, 7, 10, 11, 13, 15});

    const Vector<size_t> v_act = findIndicesOfNonZeroElements(v0);

    ASSERT_VECTOR_EQ_VECTOR(v_exp, v_act);
}

TEST_F(GeneralMathFunctionsTest, VecAll)
{
    const Vector<int> v0({0, 1, 2, 0, 1, 2, 0, 1, 0});
    const Vector<int> v1({1, 1, 2, 1, 4, 2, 3, 5, 3});

    ASSERT_FALSE(all(v0));
    ASSERT_TRUE(all(v1));
}

TEST_F(GeneralMathFunctionsTest, VecAny)
{
    const Vector<int> v0({0, 1, 2, 0, 1, 2, 0, 1, 0});
    const Vector<int> v1({1, 1, 2, 1, 4, 2, 3, 5, 3});
    const Vector<int> v2({0, 0, 0, 0, 0, 0});

    ASSERT_TRUE(any(v0));
    ASSERT_TRUE(any(v1));
    ASSERT_FALSE(any(v2));
}

}  // namespace
}  // namespace arl
