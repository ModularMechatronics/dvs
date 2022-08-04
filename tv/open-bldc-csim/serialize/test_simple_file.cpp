#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "test_classes.h"
#include "file.h"
#include "global_serializer.h"

class TestSimpleFile : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestSimpleFile, TestBasic)
{
    Vector3Df input_vec;
    input_vec.x = 2.1f;
    input_vec.y = 1.5f;
    input_vec.z = 7.3f;
    uint8_t* serialized_data = new uint8_t[input_vec.serializedSize()];

    input_vec.serialize(serialized_data);

    Vector3Df output_vec;
    output_vec.deserialize(serialized_data);

    ASSERT_EQ(input_vec.x, output_vec.x);
    ASSERT_EQ(input_vec.y, output_vec.y);
    ASSERT_EQ(input_vec.z, output_vec.z);
}

TEST_F(TestSimpleFile, TestGpsData)
{
    const size_t num_elements = 10;

    double t = 0.0, x;
    std::vector<double> t_vec, x_vec;

    SimpleFileWriter t_file("t.bin");
    SimpleFileWriter x_file("x.bin");

    for(size_t k = 0; k < num_elements; k++)
    {
        x = std::sin(t);
        t += 0.01;

        t_file.appendStaticType(t);
        x_file.appendStaticType(x);
        t_vec.push_back(t);
        x_vec.push_back(x);
    }
    t_file.close();
    x_file.close();

    SimpleFileReader t_read_file("t.bin");
    SimpleFileReader x_read_file("x.bin");
    for(size_t k = 0; k < num_elements; k++)
    {
        t_read_file.readNextStaticType(t);
        x_read_file.readNextStaticType(x);
        ASSERT_EQ(t, t_vec[k]);
        ASSERT_EQ(x, x_vec[k]);
    }
}

TEST_F(TestSimpleFile, TestFileWriter2)
{
    const size_t num_elements = 10;

    double t = 0.0;
    Vector3Df v;
    std::vector<Vector3Df> v_vec;

    SimpleFileWriter v_file("v.bin");

    for(size_t k = 0; k < num_elements; k++)
    {
        v.x = std::sin(t);
        v.y = std::sin(t * 2.0);
        v.z = std::cos(t * 3.0);
        t += 0.01;

        v_file.appendStaticType(v);
        v_vec.push_back(v);
    }
    v_file.close();


    SimpleFileReader v_read_file("v.bin");
    for(size_t k = 0; k < num_elements; k++)
    {
        v_read_file.readNextStaticType(v);
        ASSERT_EQ(v.x, v_vec[k].x);
        ASSERT_EQ(v.y, v_vec[k].y);
        ASSERT_EQ(v.z, v_vec[k].z);
    }

}

TEST_F(TestSimpleFile, TestOOB)
{
    const size_t num_elements = 10;

    uint8_t t8 = 0;
    uint32_t t32 = 0;

    SimpleFileWriter file8("8.bin");
    SimpleFileWriter file32("32.bin");

    for(size_t k = 0; k < num_elements; k++)
    {
        t8 = k;
        t32 = k;
        file8.appendStaticType(t8);
        file32.appendStaticType(t32);
    }
    file8.close();
    file32.close();

    SimpleFileReader read_file8("8.bin");
    SimpleFileReader read_file32("32.bin");
    for(size_t k = 0; k < num_elements; k++)
    {
        ASSERT_TRUE(read_file8.readNextStaticType(t8));
        ASSERT_EQ(read_file8.bytesLeftToRead(), num_elements - k - 1U);

        ASSERT_TRUE(read_file32.readNextStaticType(t32));
        ASSERT_EQ(read_file32.bytesLeftToRead(), (num_elements - k - 1U) * sizeof(uint32_t));
    }
    for(size_t k = num_elements; k < (num_elements + 3); k++)
    {
        ASSERT_FALSE(read_file8.readNextStaticType(t8));
        ASSERT_EQ(read_file8.bytesLeftToRead(), 0U);

        ASSERT_FALSE(read_file32.readNextStaticType(t32));
        ASSERT_EQ(read_file32.bytesLeftToRead(), 0U);
    }

}

TEST_F(TestSimpleFile, TestBig)
{
    const size_t num_elements = 1000000;

    SimpleFileWriter file_big("big.bin", num_elements * 4);

    std::vector<int> random_numbers;
    random_numbers.resize(num_elements);

    std::vector<float> numbers_float;
    std::vector<char> numbers_char;
    std::vector<int> numbers_int;
    std::vector<size_t> numbers_size_t;

    srand(412);

    for(size_t k = 0; k < num_elements; k++)
    {
        const int r = rand();
        random_numbers[k] = r;
        const float rn = static_cast<float>(r % 1001) / 1000.0f;

        if(rn < 0.25f)
        {
            const float f = static_cast<float>(r % 10001) / 1000.0f;
            numbers_float.push_back(f);
            file_big.appendStaticType(f);
        }
        else if((rn >= 0.25f) && (rn < 0.5f))
        {
            const char c = r % 256;
            numbers_char.push_back(c);
            file_big.appendStaticType(c);
        }
        else if((rn >= 0.5f) && (rn < 0.75f))
        {
            numbers_int.push_back(r);
            file_big.appendStaticType(r);
        }
        else
        {
            const size_t u = r * 100;
            numbers_size_t.push_back(u);
            file_big.appendStaticType(u);
        }
    }

    file_big.close();

    size_t idx_float = 0U;
    size_t idx_char = 0U;
    size_t idx_int = 0U;
    size_t idx_size_t = 0U;

    SimpleFileReader read_file_big("big.bin");

    ASSERT_EQ(read_file_big.numEntries(), num_elements);

    for(size_t k = 0; k < num_elements; k++)
    {
        const int r = random_numbers[k];
        const float rn = static_cast<float>(r % 1001) / 1000.0f;

        if(rn < 0.25f)
        {
            const float f = numbers_float[idx_float];
            float f_r;

            ASSERT_TRUE(read_file_big.readNextStaticType(f_r));
            ASSERT_EQ(f, f_r);
            idx_float += 1U;
        }
        else if((rn >= 0.25f) && (rn < 0.5f))
        {
            const char c = numbers_char[idx_char];
            char c_r;

            ASSERT_TRUE(read_file_big.readNextStaticType(c_r));
            ASSERT_EQ(c, c_r);
            idx_char += 1U;
        }
        else if((rn >= 0.5f) && (rn < 0.75f))
        {
            const int i = numbers_int[idx_int];
            int i_r;

            ASSERT_TRUE(read_file_big.readNextStaticType(i_r));
            ASSERT_EQ(i, i_r);
            idx_int += 1U;
        }
        else
        {
            const size_t u = numbers_size_t[idx_size_t];
            size_t u_r;

            ASSERT_TRUE(read_file_big.readNextStaticType(u_r));
            ASSERT_EQ(u, u_r);
            idx_size_t += 1U;
        }
    }

    float f_last;
    ASSERT_FALSE(read_file_big.readNextStaticType(f_last));
}