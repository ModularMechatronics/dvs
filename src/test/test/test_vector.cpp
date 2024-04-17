#include <duoplot/duoplot.h>
#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

using namespace duoplot;

class TestVector : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestVector, TestZipUnzip3)
{
    const size_t num_elements = 143;
    const Vector<double> x = linspaceFromBoundariesAndCount(0.1, 1.2, num_elements);
    const Vector<double> y = duoplot::sin(x);
    const Vector<double> z = duoplot::cos(x);

    const Vector<Vec3<double>> zipped_vec = zip(x.constView(), y.constView(), z.constView());
    const auto [xu, yu, zu] = unzip(zipped_vec.constView());

    for (size_t k = 0; k < num_elements; k++)
    {
        ASSERT_EQ(zipped_vec(k).x, x(k));
        ASSERT_EQ(zipped_vec(k).y, y(k));
        ASSERT_EQ(zipped_vec(k).z, z(k));

        ASSERT_EQ(x(k), xu(k));
        ASSERT_EQ(y(k), yu(k));
        ASSERT_EQ(z(k), zu(k));
    }
}

TEST_F(TestVector, TestZipUnzip2)
{
    const size_t num_elements = 143;
    const Vector<double> x = linspaceFromBoundariesAndCount(0.1, 1.2, num_elements);
    const Vector<double> y = duoplot::sin(x);

    const Vector<Vec2<double>> zipped_vec = zip(x.constView(), y.constView());
    const auto [xu, yu] = unzip(zipped_vec.constView());

    for (size_t k = 0; k < num_elements; k++)
    {
        ASSERT_EQ(zipped_vec(k).x, x(k));
        ASSERT_EQ(zipped_vec(k).y, y(k));

        ASSERT_EQ(x(k), xu(k));
        ASSERT_EQ(y(k), yu(k));
    }
}
