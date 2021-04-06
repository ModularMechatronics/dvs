#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "dvs.h"

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

class TestProperties : public testing::Test
{
protected:

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestProperties, TestBasic)
{
    static_assert(std::is_base_of<PropertyBase, LineWidth>::value);
    static_assert(std::is_base_of<PropertyBase, Alpha>::value);
    static_assert(std::is_base_of<PropertyBase, Name>::value);
    static_assert(std::is_base_of<PropertyBase, LineStyle>::value);
    static_assert(std::is_base_of<PropertyBase, Color>::value);
    static_assert(std::is_base_of<PropertyBase, EdgeColor>::value);
    static_assert(std::is_base_of<PropertyBase, FaceColor>::value);
    static_assert(std::is_base_of<PropertyBase, ColorMap>::value);
    static_assert(std::is_base_of<PropertyBase, PointSize>::value);
}

TEST_F(TestProperties, TestPropertyBase)
{
    const PropertyBase pty0;
    ASSERT_EQ(pty0.getPropertyType(), PropertyType::UNKNOWN);

    for(size_t k = 0; k < static_cast<int>(PropertyType::PERSISTENT); k++)
    {
        const PropertyType at_type = static_cast<PropertyType>(k);
        const PropertyBase ab1(at_type);
        ASSERT_EQ(ab1.getPropertyType(), at_type);
    }
}

TEST_F(TestProperties, TestLinewidth)
{
    const uint8_t lw_val = 243;
    const LineWidth lw0, lw1(lw_val);
    ASSERT_EQ(lw0.getPropertyType(), PropertyType::LINE_WIDTH);
    ASSERT_EQ(lw1.getPropertyType(), PropertyType::LINE_WIDTH);
    ASSERT_EQ(lw1.data, lw_val);
}

TEST_F(TestProperties, TestAlpha)
{
    const uint8_t alpha_val = 243;
    const Alpha a0, a1(alpha_val);
    ASSERT_EQ(a0.getPropertyType(), PropertyType::ALPHA);
    ASSERT_EQ(a1.getPropertyType(), PropertyType::ALPHA);
    ASSERT_EQ(a1.data, alpha_val);
}

TEST_F(TestProperties, TestNameBasic)
{
    static_assert(Name::max_length == 10);  // Can be changed, just as long at it's a conscious decision

    const char* name_str = "a name";
    const Name n0, n1(name_str);
    ASSERT_EQ(n0.getPropertyType(), PropertyType::NAME);
    ASSERT_EQ(n1.getPropertyType(), PropertyType::NAME);

    for(size_t k = 0; k < (Name::max_length + 1); k++)
    {
        ASSERT_EQ(static_cast<char>(0), n0.data[k]);
    }
    ASSERT_EQ(0, std::strcmp(n1.data, name_str));
}

TEST_F(TestProperties, TestNameEqualityOperator)
{
    const char* a_name = "a name";
    const char* another_name = "anoth name";
    const Name n0(a_name), n1(a_name), n2(another_name);

    ASSERT_TRUE(n0 == n1);
    ASSERT_FALSE(n1 == n2);
    ASSERT_FALSE(n0 == n2);
}

TEST_F(TestProperties, TestSafeStringLenCheck)
{
    const char* a_name = "a name";
    ASSERT_EQ(std::strlen(a_name), safeStringLenCheck(a_name, 20));
    ASSERT_EQ(static_cast<size_t>(5), safeStringLenCheck(a_name, 5));
}

TEST_F(TestProperties, TestLineStyle)
{
    const char* ls_str = "o+";
    const LineStyle ls0, ls1(ls_str);
    ASSERT_EQ(ls0.getPropertyType(), PropertyType::LINE_STYLE);
    ASSERT_EQ(ls1.getPropertyType(), PropertyType::LINE_STYLE);

    for(size_t k = 0; k < (LineStyle::max_length + 1); k++)
    {
        ASSERT_EQ(static_cast<char>(0), ls0.data[k]);
    }
    ASSERT_EQ(0, std::strcmp(ls1.data, ls_str));
}

TEST_F(TestProperties, TestColorBasic)
{
    const Color c0, c1(1, 2, 3), c2 = Color::RED();
    ASSERT_EQ(c0.getPropertyType(), PropertyType::COLOR);
    ASSERT_EQ(c1.getPropertyType(), PropertyType::COLOR);
    ASSERT_EQ(c2.getPropertyType(), PropertyType::COLOR);

    ASSERT_EQ(c1.red, 1);
    ASSERT_EQ(c1.green, 2);
    ASSERT_EQ(c1.blue, 3);
}

TEST_F(TestProperties, TestColorTypes)
{
    const std::vector<Color> expected_colors = {{255, 0, 0},
                                                {0, 255, 0},
                                                {0, 0, 255},
                                                {0, 255, 255},
                                                {255, 0, 255},
                                                {255, 255, 0},
                                                {0, 0, 0},
                                                {255, 255, 255},
                                                {127, 127, 127}};

    const std::vector<std::function<Color(void)>> colors = {Color::RED, Color::GREEN, Color::BLUE,
                                                            Color::CYAN, Color::MAGENTA, Color::YELLOW,
                                                            Color::BLACK, Color::WHITE, Color::GRAY};
    const std::vector<std::function<FaceColor(void)>> face_colors = {FaceColor::RED, FaceColor::GREEN, FaceColor::BLUE,
                                                                     FaceColor::CYAN, FaceColor::MAGENTA, FaceColor::YELLOW,
                                                                     FaceColor::BLACK, FaceColor::WHITE, FaceColor::GRAY};
    const std::vector<std::function<EdgeColor(void)>> edge_colors = {EdgeColor::RED, EdgeColor::GREEN, EdgeColor::BLUE,
                                                                     EdgeColor::CYAN, EdgeColor::MAGENTA, EdgeColor::YELLOW,
                                                                     EdgeColor::BLACK, EdgeColor::WHITE, EdgeColor::GRAY};
    for(size_t k = 0; k < expected_colors.size(); k++)
    {
        const Color c = colors[k]();
        const EdgeColor ec = edge_colors[k]();
        const FaceColor fc = face_colors[k]();

        ASSERT_EQ(c.getPropertyType(), PropertyType::COLOR);
        ASSERT_EQ(ec.getPropertyType(), PropertyType::EDGE_COLOR);
        ASSERT_EQ(fc.getPropertyType(), PropertyType::FACE_COLOR);

        ASSERT_EQ(c.red, expected_colors[k].red);
        ASSERT_EQ(c.green, expected_colors[k].green);
        ASSERT_EQ(c.blue, expected_colors[k].blue);

        ASSERT_EQ(ec.red, expected_colors[k].red);
        ASSERT_EQ(ec.green, expected_colors[k].green);
        ASSERT_EQ(ec.blue, expected_colors[k].blue);

        ASSERT_EQ(fc.red, expected_colors[k].red);
        ASSERT_EQ(fc.green, expected_colors[k].green);
        ASSERT_EQ(fc.blue, expected_colors[k].blue);
    }
}

TEST_F(TestProperties, TestEdgeColor)
{
    const EdgeColor ec0, ec1(1, 2, 3), ec2 = EdgeColor::RED();
    ASSERT_EQ(ec0.getPropertyType(), PropertyType::EDGE_COLOR);
    ASSERT_EQ(ec1.getPropertyType(), PropertyType::EDGE_COLOR);
    ASSERT_EQ(ec2.getPropertyType(), PropertyType::EDGE_COLOR);

    ASSERT_EQ(ec1.red, 1);
    ASSERT_EQ(ec1.green, 2);
    ASSERT_EQ(ec1.blue, 3);
}

TEST_F(TestProperties, TestFaceColor)
{
    const FaceColor fc0, fc1(1, 2, 3), fc2 = FaceColor::RED();
    ASSERT_EQ(fc0.getPropertyType(), PropertyType::FACE_COLOR);
    ASSERT_EQ(fc1.getPropertyType(), PropertyType::FACE_COLOR);
    ASSERT_EQ(fc2.getPropertyType(), PropertyType::FACE_COLOR);

    ASSERT_EQ(fc1.red, 1);
    ASSERT_EQ(fc1.green, 2);
    ASSERT_EQ(fc1.blue, 3);
}

TEST_F(TestProperties, TestColorMap)
{
    const internal::ColorMapType cm_type = ColorMapType::MAGMA;
    const ColorMap cm0, cm1(cm_type), cm2 = ColorMap::MAGMA();
    ASSERT_EQ(cm0.getPropertyType(), PropertyType::COLOR_MAP);
    ASSERT_EQ(cm1.getPropertyType(), PropertyType::COLOR_MAP);

    ASSERT_EQ(cm1.data, cm_type);
    ASSERT_EQ(cm2.data, cm_type);
}

TEST_F(TestProperties, TestPointSize)
{
    const uint8_t point_size = 172;
    const PointSize ps0, ps1(point_size);
    ASSERT_EQ(ps0.getPropertyType(), PropertyType::POINT_SIZE);
    ASSERT_EQ(ps1.getPropertyType(), PropertyType::POINT_SIZE);

    ASSERT_EQ(ps1.data, point_size);
}
