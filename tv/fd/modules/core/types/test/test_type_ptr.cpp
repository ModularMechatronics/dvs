#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "core/types.h"

using namespace internal;

TEST(TestFloatPtr, BasicInitialization)
{
    F32Ptr ptr = F32Ptr::create(3);
    ptr[0] = 0.1f;
    ptr[1] = 0.2f;
    ptr[2] = 0.3f;

    ASSERT_EQ(ptr[0], 0.1f);
    ASSERT_EQ(ptr[1], 0.2f);
    ASSERT_EQ(ptr[2], 0.3f);

    const float* const fq = new float[2];

    free(ptr);

    delete fq;

}

TEST(TestFloatPtr, BasicInitialization1)
{
    F32Ptr ptr = F32Ptr::create(3);
    *ptr = 0.1f;
    /**(ptr + 1) = 0.2f;
    *(ptr + 2) = 0.3f;

    std::cout << ptr[0] << std::endl;
    std::cout << ptr[1] << std::endl;
    std::cout << ptr[1] << std::endl;*/

    delete ptr;
}

TEST(TestFloat, TestArraySerialization)
{
    TypeArray<float, 10> arr, arr_out;

    for(size_t k = 0; k < 10; k++)
    {
        arr[k] = static_cast<float>(k) / 10.0f;
    }

    const size_t num_bytes = arr.serializedSize();

    uint8_t* serialized_data = new uint8_t[num_bytes];
    arr.serialize(serialized_data);

    arr_out.deserialize(serialized_data);

    for(size_t k = 0; k < 10; k++)
    {
        ASSERT_EQ(arr[k], arr_out[k]);
    }

    delete[] serialized_data;
}

