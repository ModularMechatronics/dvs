#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "serializer.h"

class TestSerializer : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestSerializer, TestBasic)
{
    const float f = 0.523f;
    const char c = 'q';
    const int i = 10412;
    const size_t u = 526434;

    const size_t num_bytes = sizeof(f) + sizeof(c) + sizeof(i) + sizeof(u);
    uint8_t* data = new uint8_t[num_bytes];

    Serializer serializer(data);
    ASSERT_EQ(serializer.getIdx(), 0U);
    ASSERT_EQ(serializer.getData(), data);

    serializer.serializeStaticType(f);
    ASSERT_EQ(serializer.getIdx(), sizeof(f));

    serializer.serializeStaticType(c);
    ASSERT_EQ(serializer.getIdx(), sizeof(f) + sizeof(c));

    serializer.serializeStaticType(i);
    ASSERT_EQ(serializer.getIdx(), sizeof(f) + sizeof(c) + sizeof(i));

    serializer.serializeStaticType(u);
    ASSERT_EQ(serializer.getIdx(), sizeof(f) + sizeof(c) + sizeof(i) + sizeof(u));

    uint64_t idx = 0;

    const uint8_t* const written_data = serializer.getData();
    const uint8_t* const f_ptr = reinterpret_cast<const uint8_t* const>(&f);
    const uint8_t* const c_ptr = reinterpret_cast<const uint8_t* const>(&c);
    const uint8_t* const i_ptr = reinterpret_cast<const uint8_t* const>(&i);
    const uint8_t* const u_ptr = reinterpret_cast<const uint8_t* const>(&u);

    for(size_t k = 0; k < sizeof(f); k++)
    {
        ASSERT_EQ(written_data[idx], f_ptr[k]);
        idx += 1;
    }

    for(size_t k = 0; k < sizeof(c); k++)
    {
        ASSERT_EQ(written_data[idx], c_ptr[k]);
        idx += 1;
    }

    for(size_t k = 0; k < sizeof(i); k++)
    {
        ASSERT_EQ(written_data[idx], i_ptr[k]);
        idx += 1;
    }

    for(size_t k = 0; k < sizeof(u); k++)
    {
        ASSERT_EQ(written_data[idx], u_ptr[k]);
        idx += 1;
    }

    delete[] data;
}

TEST_F(TestSerializer, TestString)
{
    // TODO: String size must be included
    const std::string s = "A nice string";
    const size_t num_bytes = sizeOfString(s);
    uint8_t* data = new uint8_t[num_bytes];

    for(size_t k = 0; k < num_bytes; k++)
    {
        data[k] = '#';
    }

    Serializer serializer(data);
    serializer.serializeString(s);

    for(size_t k = 0; k < (num_bytes - 1U); k++)
    {
        ASSERT_EQ(data[k], s.data()[k]);
    }

    ASSERT_EQ(data[num_bytes - 1U], '\0');
}

TEST_F(TestSerializer, TestMultipleStrings)
{
    const std::vector<std::string> v = {"A nice string", "Second data", "Hey hey"};
    const size_t num_bytes = sizeOfString(v[0]) + sizeOfString(v[1]) + sizeOfString(v[2]);
    uint8_t* data = new uint8_t[num_bytes];

    for(size_t k = 0; k < num_bytes; k++)
    {
        data[k] = '#';
    }

    Serializer serializer(data);
    // TODO
}
