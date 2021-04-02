#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "dvs.h"

using namespace dvs::internal;

class TestAttributes : public testing::Test
{
protected:

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestAttributes, TestBasic)
{
    static_assert(std::is_base_of<AttributeBase, Linewidth>::value);
    static_assert(std::is_base_of<AttributeBase, Alpha>::value);
    static_assert(std::is_base_of<AttributeBase, Name>::value);
    static_assert(std::is_base_of<AttributeBase, LineStyle>::value);
    static_assert(std::is_base_of<AttributeBase, Color>::value);
    static_assert(std::is_base_of<AttributeBase, EdgeColor>::value);
    static_assert(std::is_base_of<AttributeBase, FaceColor>::value);
    static_assert(std::is_base_of<AttributeBase, ColorMap>::value);
    static_assert(std::is_base_of<AttributeBase, Persistent>::value);
    static_assert(std::is_base_of<AttributeBase, PointSize>::value);
}

TEST_F(TestAttributes, TestAttributeBase)
{
    const AttributeBase ab0;
    ASSERT_EQ(ab0.getAttributeType(), AttributeType::UNKNOWN);

    for(size_t k = 0; k < static_cast<int>(AttributeType::PERSISTENT); k++)
    {
        const AttributeType at_type = static_cast<AttributeType>(k);
        const AttributeBase ab1(at_type);
        ASSERT_EQ(ab1.getAttributeType(), at_type);
    }
}

TEST_F(TestAttributes, TestLinewidth)
{
    const float lw_val = 3.14f;
    const Linewidth lw0, lw1(lw_val);
    ASSERT_EQ(lw0.getAttributeType(), AttributeType::LINEWIDTH);
    ASSERT_EQ(lw1.getAttributeType(), AttributeType::LINEWIDTH);
    ASSERT_EQ(lw1.data, lw_val);
}

TEST_F(TestAttributes, TestAlpha)
{
    const float alpha_val = 3.14f;
    const Alpha a0, a1(alpha_val);
    ASSERT_EQ(a0.getAttributeType(), AttributeType::ALPHA);
    ASSERT_EQ(a1.getAttributeType(), AttributeType::ALPHA);
    ASSERT_EQ(a1.data, alpha_val);
}

TEST_F(TestAttributes, TestNameBasic)
{
    static_assert(Name::max_length == 10);  // Can be changed, just as long at it's a conscious decision

    const char* name_str = "a name";
    const Name n0, n1(name_str);
    ASSERT_EQ(n0.getAttributeType(), AttributeType::NAME);
    ASSERT_EQ(n1.getAttributeType(), AttributeType::NAME);

    for(size_t k = 0; k < (Name::max_length + 1); k++)
    {
        ASSERT_EQ(static_cast<char>(0), n0.data[k]);
    }
    ASSERT_EQ(0, std::strcmp(n1.data, name_str));
}

TEST_F(TestAttributes, TestNameEqualityOperator)
{
    const char* a_name = "a name";
    const char* another_name = "anoth name";
    const Name n0(a_name), n1(a_name), n2(another_name);

    ASSERT_TRUE(n0 == n1);
    ASSERT_FALSE(n1 == n2);
    ASSERT_FALSE(n0 == n2);
}

TEST_F(TestAttributes, TestSafeStringLenCheck)
{
    const char* a_name = "a name";
    ASSERT_EQ(std::strlen(a_name), safeStringLenCheck(a_name, 20));
    ASSERT_EQ(5, safeStringLenCheck(a_name, 5));
}

TEST_F(TestAttributes, TestLineStyle)
{
    const char* ls_str = "o+";
    const LineStyle ls0, ls1(ls_str);
    ASSERT_EQ(ls0.getAttributeType(), AttributeType::LINE_STYLE);
    ASSERT_EQ(ls1.getAttributeType(), AttributeType::LINE_STYLE);
    // ASSERT_EQ(a1.data, alpha_val);
}
