#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

// #include "test_classes.h"
#include "core/serializer/file.h"

class TestSimpleFile : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};


struct TrivialVector3Df : Serializable
{
    TrivialVector3Df() = default;
    TrivialVector3Df(const float x_, const float y_, const float z_) : x(x_), y(y_), z(z_) {}

    float x;
    float y;
    float z;

    uint64_t serializedSize() const override
    {
        return sizeof(TrivialVector3Df);
    }

    void serialize(uint8_t* serialized_data) const override
    {
        Serializer serializer(serialized_data);
        serializer.serializeStaticType(*this);
    }

    void deserialize(const uint8_t* serialized_data) override
    {
        Deserializer deserializer(serialized_data);
        deserializer.deserializeToStaticType(*this);
    }

};

TEST_F(TestSimpleFile, TestBasic)
{
    const float f = 0.2423f;
    const double d = 0.5264;
    const uint8_t u8 = 13;
    const uint16_t u16 = 17;
    const uint32_t u32 = 141;
    const uint64_t u64 = 247;

    const int8_t i8 = 13;
    const int16_t i16 = 17;
    const int32_t i32 = 111;
    const int64_t i64 = -247;

    const std::string s = "hesanvarmivoare";
    const char* c = "somechars";

    const TrivialVector3Df v3(0.7, 0.5, 0.3);

    /*
    Test types:
    const std::vector<double> double_vec = {0.1, 0.2, 0.3};
    const std::vector<std::string> str_vec = {"owi5t", "miofa", "5q4"};
    const std::vector<Point2Df>

    Point2Df* p_ptr;
    float* f_ptr;
    */

    SimpleFileWriter v_file("file.bin");

    v_file.append(f);
    v_file.append(d);

    v_file.append(u8);
    v_file.append(u16);
    v_file.append(u32);
    v_file.append(u64);

    v_file.append(i8);
    v_file.append(i16);
    v_file.append(i32);
    v_file.append(i64);

    v_file.append(s);
    v_file.append(c);

    v_file.append(v3);

    v_file.close();

    SimpleFileReader v_file_read("file.bin");

    float f_read;
    double d_read;
    uint8_t u8_read;
    uint16_t u16_read;
    uint32_t u32_read;
    uint64_t u64_read;
    int8_t i8_read;
    int16_t i16_read;
    int32_t i32_read;
    int64_t i64_read;

    std::string s_read;
    std::string c_read;

    TrivialVector3Df v3_read;

    v_file_read.readNext(f_read);
    v_file_read.readNext(d_read);
    v_file_read.readNext(u8_read);
    v_file_read.readNext(u16_read);
    v_file_read.readNext(u32_read);
    v_file_read.readNext(u64_read);
    v_file_read.readNext(i8_read);
    v_file_read.readNext(i16_read);
    v_file_read.readNext(i32_read);
    v_file_read.readNext(i64_read);

    v_file_read.readNext(s_read);
    v_file_read.readNext(c_read);

    v_file_read.readNext(v3_read);

    ASSERT_EQ(f, f_read);
    ASSERT_EQ(d, d_read);
    ASSERT_EQ(u8, u8_read);
    ASSERT_EQ(u16, u16_read);
    ASSERT_EQ(u32, u32_read);
    ASSERT_EQ(u64, u64_read);
    ASSERT_EQ(i8, i8_read);
    ASSERT_EQ(i16, i16_read);
    ASSERT_EQ(i32, i32_read);
    ASSERT_EQ(i64, i64_read);

    ASSERT_EQ(s, s_read);
    ASSERT_EQ(std::string(c), c_read);

    ASSERT_EQ(v3.x, v3_read.x);
    ASSERT_EQ(v3.y, v3_read.y);
    ASSERT_EQ(v3.z, v3_read.z);

}

TEST_F(TestSimpleFile, TestPointer)
{
    const std::vector<double> double_vec = {0.1, 0.2, 0.3};
    const std::vector<float> float_vec = {0.1, 0.2, 0.3};
    const std::vector<uint8_t> uint8_vec = {1, 2, 3};

    SimpleFileWriter v_file("file.bin");
    v_file.append(double_vec.data(), double_vec.size());
    v_file.append(float_vec.data(), float_vec.size());
    v_file.append(uint8_vec.data(), uint8_vec.size());

    v_file.close();

    SimpleFileReader v_file_read("file.bin");

    std::vector<double> double_vec_read;
    std::vector<float> float_vec_read;
    std::vector<uint8_t> uint8_vec_read;

    v_file_read.readNext(double_vec_read);
    v_file_read.readNext(float_vec_read);
    v_file_read.readNext(uint8_vec_read);

    for(size_t k = 0; k < 3; k++)
    {
        ASSERT_EQ(double_vec_read[k], double_vec[k]);
        ASSERT_EQ(float_vec_read[k], float_vec[k]);
        ASSERT_EQ(uint8_vec_read[k], uint8_vec[k]);
    }
}

TEST_F(TestSimpleFile, TestVector)
{
    const std::vector<double> double_vec = {0.1, 0.2, 0.3};
    const std::vector<float> float_vec = {0.1, 0.2, 0.3};
    const std::vector<uint8_t> uint8_vec = {1, 2, 3};

    SimpleFileWriter v_file("file.bin");
    v_file.append(double_vec);
    v_file.append(float_vec);
    v_file.append(uint8_vec);

    v_file.close();

    SimpleFileReader v_file_read("file.bin");

    std::vector<double> double_vec_read;
    std::vector<float> float_vec_read;
    std::vector<uint8_t> uint8_vec_read;

    v_file_read.readNext(double_vec_read);
    v_file_read.readNext(float_vec_read);
    v_file_read.readNext(uint8_vec_read);

    for(size_t k = 0; k < 3; k++)
    {
        ASSERT_EQ(double_vec_read[k], double_vec[k]);
        ASSERT_EQ(float_vec_read[k], float_vec[k]);
        ASSERT_EQ(uint8_vec_read[k], uint8_vec[k]);
    }
}

TEST_F(TestSimpleFile, TestBasic2)
{
    TrivialVector3Df input_vec;
    input_vec.x = 2.1f;
    input_vec.y = 1.5f;
    input_vec.z = 7.3f;
    uint8_t* serialized_data = new uint8_t[input_vec.serializedSize()];

    input_vec.serialize(serialized_data);

    TrivialVector3Df output_vec;
    output_vec.deserialize(serialized_data);

    ASSERT_EQ(input_vec.x, output_vec.x);
    ASSERT_EQ(input_vec.y, output_vec.y);
    ASSERT_EQ(input_vec.z, output_vec.z);
}

/*TEST_F(TestSimpleFile, TestGpsData)
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

        t_file.append(t);
        x_file.append(x);
        t_vec.push_back(t);
        x_vec.push_back(x);
    }
    t_file.close();
    x_file.close();

    SimpleFileReader t_read_file("t.bin");
    SimpleFileReader x_read_file("x.bin");
    for(size_t k = 0; k < num_elements; k++)
    {
        t_read_file.readNext(t);
        x_read_file.readNext(x);
        ASSERT_EQ(t, t_vec[k]);
        ASSERT_EQ(x, x_vec[k]);
    }
}*/

TEST_F(TestSimpleFile, TestFileWriter2)
{
    const size_t num_elements = 10;

    double t = 0.0;
    TrivialVector3Df v;
    std::vector<TrivialVector3Df> v_vec;

    SimpleFileWriter v_file("v.bin");

    for(size_t k = 0; k < num_elements; k++)
    {
        v.x = std::sin(t);
        v.y = std::sin(t * 2.0);
        v.z = std::cos(t * 3.0);
        t += 0.01;

        v_file.append(v);
        v_vec.push_back(v);
    }
    v_file.close();


    SimpleFileReader v_read_file("v.bin");
    for(size_t k = 0; k < num_elements; k++)
    {
        v_read_file.readNext(v);
        ASSERT_EQ(v.x, v_vec[k].x);
        ASSERT_EQ(v.y, v_vec[k].y);
        ASSERT_EQ(v.z, v_vec[k].z);
    }

}

/*TEST_F(TestSimpleFile, TestOOB)
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
        file8.append(t8);
        file32.append(t32);
    }
    file8.close();
    file32.close();

    SimpleFileReader read_file8("8.bin");
    SimpleFileReader read_file32("32.bin");
    for(size_t k = 0; k < num_elements; k++)
    {
        ASSERT_TRUE(read_file8.readNext(t8));
        ASSERT_EQ(read_file8.bytesLeftToRead(), num_elements - k - 1U);

        ASSERT_TRUE(read_file32.readNext(t32));
        ASSERT_EQ(read_file32.bytesLeftToRead(), (num_elements - k - 1U) * sizeof(uint32_t));
    }
    for(size_t k = num_elements; k < (num_elements + 3); k++)
    {
        ASSERT_FALSE(read_file8.readNext(t8));
        ASSERT_EQ(read_file8.bytesLeftToRead(), 0U);

        ASSERT_FALSE(read_file32.readNext(t32));
        ASSERT_EQ(read_file32.bytesLeftToRead(), 0U);
    }

}*/

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
            file_big.append(f);
        }
        else if((rn >= 0.25f) && (rn < 0.5f))
        {
            const char c = r % 256;
            numbers_char.push_back(c);
            file_big.append(c);
        }
        else if((rn >= 0.5f) && (rn < 0.75f))
        {
            numbers_int.push_back(r);
            file_big.append(r);
        }
        else
        {
            const size_t u = r * 100;
            numbers_size_t.push_back(u);
            file_big.append(u);
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

            ASSERT_TRUE(read_file_big.readNext(f_r));
            ASSERT_EQ(f, f_r);
            idx_float += 1U;
        }
        else if((rn >= 0.25f) && (rn < 0.5f))
        {
            const char c = numbers_char[idx_char];
            char c_r;

            ASSERT_TRUE(read_file_big.readNext(c_r));
            ASSERT_EQ(c, c_r);
            idx_char += 1U;
        }
        else if((rn >= 0.5f) && (rn < 0.75f))
        {
            const int i = numbers_int[idx_int];
            int i_r;

            ASSERT_TRUE(read_file_big.readNext(i_r));
            ASSERT_EQ(i, i_r);
            idx_int += 1U;
        }
        else
        {
            const size_t u = numbers_size_t[idx_size_t];
            size_t u_r;

            ASSERT_TRUE(read_file_big.readNext(u_r));
            ASSERT_EQ(u, u_r);
            idx_size_t += 1U;
        }
    }

    float f_last;
    // ASSERT_FALSE(read_file_big.readNext(f_last)); // TODO: Fix
}
