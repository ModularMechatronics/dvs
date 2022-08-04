#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "serializer.h"

class TestDeserializer : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestDeserializer, TestBasic)
{
    const float f = 0.523f;
    const char c = 'q';
    const int i = 10412;
    const size_t u = 526434;

    const size_t num_bytes = sizeof(f) + sizeof(c) + sizeof(i) + sizeof(u);
    uint8_t* data = new uint8_t[num_bytes];

    Serializer serializer(data);

    serializer.serializeStaticType(f);
    serializer.serializeStaticType(c);
    serializer.serializeStaticType(i);
    serializer.serializeStaticType(u);

    Deserializer deserializer(data);

    float r_f;
    char r_c;
    int r_i;
    size_t r_u;

    deserializer.deserializeToStaticType(r_f);
    deserializer.deserializeToStaticType(r_c);
    deserializer.deserializeToStaticType(r_i);
    deserializer.deserializeToStaticType(r_u);

    ASSERT_EQ(r_f, f);
    ASSERT_EQ(r_c, c);
    ASSERT_EQ(r_i, i);
    ASSERT_EQ(r_u, u);

}

TEST_F(TestDeserializer, TestBig)
{
    const size_t num_elements = 12;

    ASSERT_EQ(num_elements % 4, 0U);

    const size_t num_bytes = num_elements * (sizeof(float) * sizeof(char) * sizeof(int) * sizeof(size_t));
    uint8_t* data = new uint8_t[num_bytes];

    Serializer serializer(data);

    for(size_t k = 0; k < num_elements; k++)
    {
        if((k % 4) == 0)
        {
            const float f = k;
            serializer.serializeStaticType(f);
        }
        else if((k % 4) == 1)
        {
            const char c = k;
            serializer.serializeStaticType(c);
        }
        else if((k % 4) == 2)
        {
            const int i = k;
            serializer.serializeStaticType(i);
        }
        else if((k % 4) == 3)
        {
            const size_t u = k;
            serializer.serializeStaticType(u);
        }
        else
        {
            ASSERT_TRUE(false) << "Shouldn't come here!";
        }
    }

    Deserializer deserializer(data);

    for(size_t k = 0; k < num_elements; k++)
    {
        if((k % 4) == 0)
        {
            const float f = k;
            float r_f;
            deserializer.deserializeToStaticType(r_f);
            ASSERT_EQ(f, r_f);
        }
        else if((k % 4) == 1)
        {
            const char c = k;
            char r_c;
            deserializer.deserializeToStaticType(r_c);
            ASSERT_EQ(c, r_c);
        }
        else if((k % 4) == 2)
        {
            const int i = k;
            int r_i;
            deserializer.deserializeToStaticType(r_i);
            ASSERT_EQ(i, r_i);
        }
        else if((k % 4) == 3)
        {
            const size_t u = k;
            size_t r_u;
            deserializer.deserializeToStaticType(r_u);
            ASSERT_EQ(u, r_u);
        }
        else
        {
            ASSERT_TRUE(false) << "Shouldn't come here!";
        }
    }

}
