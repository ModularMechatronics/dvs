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
class ParametricLine2DTest : public testing::Test
{
protected:
    std::vector<double> px_vec;
    std::vector<double> py_vec;
    std::vector<double> vx_vec;
    std::vector<double> vy_vec;
    std::vector<double> t_vec;

    const double test_a[8] = {0.2342, -1.381, -0.982, 2.523, 0.2342, -1.381, -0.982, 2.523};
    const double test_b[8] = {1.231, -0.903, 2.412, -0.124, 1.231, -0.903, 2.412, -0.124};
    const double test_c[8] = {0.251, 0.912, 3.212, 0.553, -0.251, -0.912, -3.212, -0.553};

    const double x_vec[10] = {
        -2.124, -1.141, -0.101, 0.0, 0.141, 1.624, 1.971, 2.012, 3.512, 9.124};
    const double y_vec[10] = {
        -1.224, -1.113, -0.112, 0.0, 0.122, 1.723, 1.992, 2.113, 4.612, 10.324};

    void SetUp() override
    {
        const size_t num_iterations = 10;
        px_vec.clear();
        py_vec.clear();
        vx_vec.clear();
        vy_vec.clear();
        t_vec.clear();
        double phi = 0.0;

        for (size_t k = 0; k < num_iterations; k++)
        {
            phi = 2.0 * M_PI * static_cast<double>(k) / static_cast<double>(num_iterations - 1) -
                  DEG2RAD(2.0);

            px_vec.push_back(std::cos(static_cast<double>(k)));
            py_vec.push_back(std::sin(static_cast<double>(k)));
            t_vec.push_back(std::sin(3.0 * static_cast<double>(k)));
            vx_vec.push_back(std::cos(phi));
            vy_vec.push_back(std::sin(phi));
        }
    }

    void TearDown() override {}
};

TEST_F(ParametricLine2DTest, Initialization)
{
    std::cout << "Hello" << std::endl;
}

}  // namespace
}  // namespace arl
