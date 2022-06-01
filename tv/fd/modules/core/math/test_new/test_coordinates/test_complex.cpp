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
class ComplexTest : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(ComplexTest, Initialization)
{
    const double eps = 1e-7;

    const double real = 0.235253;
    const double imag = -2.252363;

    const ComplexCoordd c(real, imag);

    ASSERT_NEAR(real, c.real, eps);
    ASSERT_NEAR(imag, c.imag, eps);

    const ComplexCoordd c_no_constr;

    const double real_assigned = c_no_constr.real;
    const double imag_assigned = c_no_constr.imag;

    ASSERT_NEAR(real_assigned, c_no_constr.real, eps);
    ASSERT_NEAR(imag_assigned, c_no_constr.imag, eps);
}

TEST_F(ComplexTest, Multiply)
{
    /*const double eps = 1e-7;

    const ComplexCoordd c0(0.235253, -2.252363);
    const ComplexCoordd c1(-9.345223, 4.42312);

    ComplexCoordd cres = c0 * c1;
    // ASSERT_NEAR(real, c.real, eps);
    // ASSERT_NEAR(imag, c.imag, eps);*/
}

TEST_F(ComplexTest, Abs)
{
    const double eps = 1e-7;

    const ComplexCoordd c_unit(1.0 / std::sqrt(2.0), 1.0 / std::sqrt(2.0));

    for (size_t k = 0; k < 10; k++)
    {
        const double kd = static_cast<double>(k);
        const double f = 0.234235 * kd / (kd + 2.0);

        ComplexCoordd c = f * c_unit;

        const double c_abs = c.abs();

        ASSERT_NEAR(c_abs, f, eps);
    }
}

TEST_F(ComplexTest, ComplexToPolar)
{
    const double eps = 1e-7;

    const size_t num_test_cases = 10;
    const double num_test_cases_d = static_cast<double>(num_test_cases);

    const double inc = 2.0 * M_PI / (num_test_cases_d + 1.0);

    double phi = 0.0;

    for (size_t k = 0; k < num_test_cases; k++)
    {
        const double r = static_cast<double>(k) * 3.0 + 10.0;

        const ComplexCoordd c(r * std::cos(phi), r * std::sin(phi));

        const PolarCoordd p = c.toPolar();

        ASSERT_NEAR(r, p.r, eps);
        ASSERT_NEAR(std::cos(phi), std::cos(p.phi), eps);
        ASSERT_NEAR(std::sin(phi), std::sin(p.phi), eps);

        phi = phi + inc;
    }
}

TEST_F(ComplexTest, PolarToComplex)
{
    const double eps = 1e-7;

    const size_t num_test_cases = 10;
    const double num_test_cases_d = static_cast<double>(num_test_cases);

    const double inc = 2.0 * M_PI / (num_test_cases_d + 1.0);

    double phi = 0.0;

    for (size_t k = 0; k < num_test_cases; k++)
    {
        const double r = static_cast<double>(k) * 3.0 + 10.0;

        const double real = r * std::cos(phi);
        const double imag = r * std::sin(phi);

        const PolarCoordd pv(r, phi);
        const ComplexCoordd c = pv.toComplexCoord();

        ASSERT_NEAR(c.real, real, eps);
        ASSERT_NEAR(c.imag, imag, eps);

        phi = phi + inc;
    }
}

TEST_F(ComplexTest, PolarToComplexBackAndForth)
{
    const double eps = 1e-7;

    const size_t num_test_cases = 10;
    const double num_test_cases_d = static_cast<double>(num_test_cases);

    const double inc = 2.0 * M_PI / (num_test_cases_d + 1.0);

    double phi = 0.0;

    for (size_t k = 0; k < num_test_cases; k++)
    {
        const double r = static_cast<double>(k) * 3.0 + 10.0;

        const PolarCoordd pv_expected(r, phi);
        const ComplexCoordd c = pv_expected.toComplexCoord();

        const PolarCoordd pv_actual = c.toPolar();

        ASSERT_NEAR(pv_expected.r, pv_actual.r, eps);
        ASSERT_NEAR(std::cos(pv_expected.phi), std::cos(pv_actual.phi), eps);
        ASSERT_NEAR(std::sin(pv_expected.phi), std::sin(pv_actual.phi), eps);

        phi = phi + inc;
    }
}

}  // namespace
}  // namespace arl
