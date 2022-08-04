#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "core/serializer/global_file.h"

class TestGlobalSerializer : public testing::Test
{
protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestGlobalSerializer, TestBasic)
{
    serializer::global::simple::createNewFile("a_file.bin");

    const float f = 0.2423f;
    const uint8_t u8 = 13;
    const uint16_t u16 = 17;
    const uint32_t u32 = 141;
    const uint64_t u64 = 247;

    const int8_t i8 = 13;
    const int16_t i16 = 17;
    const int32_t i32 = 111;
    const int64_t i64 = -247;

    const std::string s = "hesanvarmivoare";
    const std::vector<double> double_vec = {0.1, 0.2, 0.3};
    const std::vector<std::string> str_vec = {"owi5t", "miofa", "5q4"};


    serializer::global::simple::append("a_file.bin", f);
    
}
