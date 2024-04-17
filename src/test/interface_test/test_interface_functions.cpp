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

DataType sizeToSignedDataType(const size_t size)
{
    DataType int_data_type;
    switch (size)
    {
        case 1:
            int_data_type = DataType::INT8;
            break;
        case 2:
            int_data_type = DataType::INT16;
            break;
        case 4:
            int_data_type = DataType::INT32;
            break;
        case 8:
            int_data_type = DataType::INT64;
            break;
        default:
            int_data_type = DataType::UNKNOWN;
    }
    return int_data_type;
}

DataType sizeToUnsignedDataType(const size_t size)
{
    DataType int_data_type;
    switch (size)
    {
        case 1:
            int_data_type = DataType::UINT8;
            break;
        case 2:
            int_data_type = DataType::UINT16;
            break;
        case 4:
            int_data_type = DataType::UINT32;
            break;
        case 8:
            int_data_type = DataType::UINT64;
            break;
        default:
            int_data_type = DataType::UNKNOWN;
    }
    return int_data_type;
}

TEST(TestInterface, TestTypeToDataTypeEnum)
{
    ASSERT_EQ(DataType::FLOAT, typeToDataTypeEnum<float>());
    ASSERT_EQ(DataType::DOUBLE, typeToDataTypeEnum<double>());
    ASSERT_EQ(DataType::FLOAT, typeToDataTypeEnum<float_t>());
    ASSERT_EQ(DataType::DOUBLE, typeToDataTypeEnum<double_t>());
    ASSERT_EQ(DataType::INT8, typeToDataTypeEnum<int8_t>());
    ASSERT_EQ(DataType::INT16, typeToDataTypeEnum<int16_t>());
    ASSERT_EQ(DataType::INT32, typeToDataTypeEnum<int32_t>());
    ASSERT_EQ(DataType::INT64, typeToDataTypeEnum<int64_t>());
    ASSERT_EQ(DataType::UINT8, typeToDataTypeEnum<uint8_t>());
    ASSERT_EQ(DataType::UINT16, typeToDataTypeEnum<uint16_t>());
    ASSERT_EQ(DataType::UINT32, typeToDataTypeEnum<uint32_t>());
    ASSERT_EQ(DataType::UINT64, typeToDataTypeEnum<uint64_t>());

    ASSERT_EQ(DataType::INT8, typeToDataTypeEnum<char>());
    ASSERT_EQ(DataType::UINT8, typeToDataTypeEnum<unsigned char>());

    // int
    {
        const DataType dtype = sizeToSignedDataType(sizeof(int));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<int>());
    }

    {
        const DataType dtype = sizeToUnsignedDataType(sizeof(unsigned int));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<unsigned int>());
    }

    // long
    {
        const DataType dtype = sizeToSignedDataType(sizeof(long));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<long>());
    }

    {
        const DataType dtype = sizeToUnsignedDataType(sizeof(unsigned long));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<unsigned long>());
    }

    // long long
    {
        const DataType dtype = sizeToSignedDataType(sizeof(long long));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<long long>());
    }

    {
        const DataType dtype = sizeToUnsignedDataType(sizeof(unsigned long long));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<unsigned long long>());
    }

    // long int
    {
        const DataType dtype = sizeToSignedDataType(sizeof(long int));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<long int>());
    }

    {
        const DataType dtype = sizeToUnsignedDataType(sizeof(unsigned long int));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<unsigned long int>());
    }

    // size_t
    {
        const DataType dtype = sizeToUnsignedDataType(sizeof(size_t));
        ASSERT_NE(DataType::UNKNOWN, dtype);
        ASSERT_EQ(dtype, typeToDataTypeEnum<size_t>());
    }

}

TEST(TestInterface, TestToUInt32)
{
    const size_t number = 3414;
    const bool b = std::is_same<uint32_t, decltype(toUInt32(number))>::value;
    ASSERT_TRUE(b);
    static_assert(std::is_same<uint32_t, decltype(toUInt32(number))>::value, "Invalid type!");

    const size_t overflow_number = 4294967296U; // 2^32
    ASSERT_EQ(toUInt32(overflow_number), 0U);

    ASSERT_EQ(toUInt32(overflow_number - 1U), overflow_number - 1U);
}

TEST(TestInterface, TestToUInt8)
{
    const size_t number = 124;
    const bool b = std::is_same<uint8_t, decltype(toUInt8(number))>::value;
    ASSERT_TRUE(b);
    static_assert(std::is_same<uint8_t, decltype(toUInt8(number))>::value, "Invalid type!");

    const size_t overflow_number = 256U;
    ASSERT_EQ(toUInt8(overflow_number), 0U);

    ASSERT_EQ(toUInt8(overflow_number - 1U), overflow_number - 1U);
}