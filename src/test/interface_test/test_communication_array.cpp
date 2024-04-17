#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "duoplot/duoplot.h"

using namespace duoplot;
using namespace duoplot::internal;
using namespace duoplot::properties;

TEST(TestCommunicationArray, TestBasic)
{
    constexpr int16_t n_elems = 10;
    CommunicationHeader::Array<n_elems> a0;

    ASSERT_EQ(a0.size(), static_cast<size_t>(n_elems));
    ASSERT_EQ(a0.usedSize(), 0U);

    a0.pushBack(CommunicationHeaderObject{});

    ASSERT_EQ(a0.usedSize(), 1U);
    ASSERT_EQ(a0[0].type, CommunicationHeaderObjectType::UNKNOWN);

    a0.pushBack(CommunicationHeaderObject{CommunicationHeaderObjectType::AZIMUTH});

    ASSERT_EQ(a0.usedSize(), 2U);
    ASSERT_EQ(a0[1].type, CommunicationHeaderObjectType::AZIMUTH);

    a0.clear();
    ASSERT_EQ(a0.usedSize(), 0U);
}

TEST(TestCommunicationArray, TestLastElement)
{
    constexpr int16_t n_elems = 10;
    CommunicationHeader::Array<n_elems> a0;

    ASSERT_EQ(a0.size(), static_cast<size_t>(n_elems));
    ASSERT_EQ(a0.usedSize(), 0U);

    EXPECT_THROW(
        {
            try
            {
                const auto& q = a0.lastElement();
                static_cast<void>(q);
            }
            catch (const std::runtime_error& e)
            {
                EXPECT_STREQ("Array empty!", e.what());
                throw;
            }
        },
        std::runtime_error);

    a0.pushBack(CommunicationHeaderObject{});
    ASSERT_EQ(a0.lastElement().type, CommunicationHeaderObjectType::UNKNOWN);

    a0.pushBack(CommunicationHeaderObject{CommunicationHeaderObjectType::AZIMUTH});
    ASSERT_EQ(a0.lastElement().type, CommunicationHeaderObjectType::AZIMUTH);
}

TEST(TestCommunicationArray, TestSetUsedSize)
{
    constexpr int16_t n_elems = 5;
    CommunicationHeader::Array<n_elems> a0;

    ASSERT_EQ(a0.size(), static_cast<size_t>(n_elems));

    ASSERT_EQ(a0.usedSize(), 0U);

    a0.setUsedSize(1);
    ASSERT_EQ(a0.usedSize(), 1U);

    a0.setUsedSize(2);
    ASSERT_EQ(a0.usedSize(), 2U);

    a0.setUsedSize(3);
    ASSERT_EQ(a0.usedSize(), 3U);

    a0.setUsedSize(4);
    ASSERT_EQ(a0.usedSize(), 4U);

    a0.setUsedSize(5);
    ASSERT_EQ(a0.usedSize(), 5U);

    EXPECT_THROW(
        {
            try
            {
                a0.setUsedSize(6);
            }
            catch (const std::runtime_error& e)
            {
                EXPECT_STREQ("Trying set new size to larger than N!", e.what());
                throw;
            }
        },
        std::runtime_error);

    ASSERT_EQ(a0.usedSize(), 5U);
}

TEST(TestCommunicationArray, TestOutOfBounds)
{
    constexpr int16_t n_elems = 3;
    CommunicationHeader::Array<n_elems> a0;

    ASSERT_EQ(a0.usedSize(), 0U);

    a0.pushBack(CommunicationHeaderObject{});
    ASSERT_EQ(a0.usedSize(), 1U);

    a0.pushBack(CommunicationHeaderObject{});
    ASSERT_EQ(a0.usedSize(), 2U);

    a0.pushBack(CommunicationHeaderObject{});
    ASSERT_EQ(a0.usedSize(), 3U);

    EXPECT_THROW(
        {
            try
            {
                a0.pushBack(CommunicationHeaderObject{});
            }
            catch (const std::runtime_error& e)
            {
                EXPECT_STREQ("Trying to push new object out of bounds!", e.what());
                throw;
            }
        },
        std::runtime_error);

    ASSERT_EQ(a0.usedSize(), 3U);
}
