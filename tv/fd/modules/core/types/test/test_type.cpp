#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "core/types.h"

using namespace internal;

TEST(TestFloat, BasicInitialization0)
{
    const float f = 0.512343f;
    F32 f0{f};
    const F32 f1{f};
    F32 f2 = f;
    const F32 f3 = f;
    F32 f4(f);
    const F32 f5(f);
    F32 f6 = {f};
    const F32 f7 = {f};

    ASSERT_EQ(f0, f);
    ASSERT_EQ(f1, f);
    ASSERT_EQ(f2, f);
    ASSERT_EQ(f3, f);
    ASSERT_EQ(f4, f);
    ASSERT_EQ(f5, f);
    ASSERT_EQ(f6, f);
    ASSERT_EQ(f7, f);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f1, f3);
}

TEST(TestFloat, BasicInitialization1)
{
    float f = 0.512343f;
    F32 f0{f};
    const F32 f1{f};
    F32 f2 = f;
    const F32 f3 = f;
    F32 f4(f);
    const F32 f5(f);
    F32 f6 = {f};
    const F32 f7 = {f};

    ASSERT_EQ(f0, f);
    ASSERT_EQ(f1, f);
    ASSERT_EQ(f2, f);
    ASSERT_EQ(f3, f);
    ASSERT_EQ(f4, f);
    ASSERT_EQ(f5, f);
    ASSERT_EQ(f6, f);
    ASSERT_EQ(f7, f);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f1, f3);

}

TEST(TestFloat, BasicInitialization2)
{
    F32 f0{0.512343f};
    const F32 f1{0.512343f};
    F32 f2 = 0.512343f;
    const F32 f3 = 0.512343f;
    F32 f4(0.512343f);
    const F32 f5(0.512343f);
    F32 f6 = {0.512343f};
    const F32 f7 = {0.512343f};

    ASSERT_EQ(f0, 0.512343f);
    ASSERT_EQ(f1, 0.512343f);
    ASSERT_EQ(f2, 0.512343f);
    ASSERT_EQ(f3, 0.512343f);
    ASSERT_EQ(f4, 0.512343f);
    ASSERT_EQ(f5, 0.512343f);
    ASSERT_EQ(f6, 0.512343f);
    ASSERT_EQ(f7, 0.512343f);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f1, f3);
}

TEST(TestFloat, BasicInitializationFromSameType0)
{
    const float f = 0.512343f;
    const F32 fq = f;
    F32 f0{fq};
    const F32 f1{fq};
    F32 f2 = fq;
    const F32 f3 = fq;
    F32 f4(fq);
    const F32 f5(fq);
    F32 f6 = {fq};
    const F32 f7 = {fq};

    ASSERT_EQ(f0, f);
    ASSERT_EQ(f1, f);
    ASSERT_EQ(f2, f);
    ASSERT_EQ(f3, f);
    ASSERT_EQ(f4, f);
    ASSERT_EQ(f5, f);
    ASSERT_EQ(f6, f);
    ASSERT_EQ(f7, f);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f1, f3);
}

TEST(TestFloat, BasicInitializationFromSameType1)
{
    float f = 0.512343f;
    F32 fq = f;
    F32 f0{fq};
    const F32 f1{fq};
    F32 f2 = fq;
    const F32 f3 = fq;
    F32 f4(fq);
    const F32 f5(fq);
    F32 f6 = {fq};
    const F32 f7 = {fq};

    ASSERT_EQ(f0, f);
    ASSERT_EQ(f1, f);
    ASSERT_EQ(f2, f);
    ASSERT_EQ(f3, f);
    ASSERT_EQ(f4, f);
    ASSERT_EQ(f5, f);
    ASSERT_EQ(f6, f);
    ASSERT_EQ(f7, f);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f1, f3);
}

TEST(TestFloat, BasicInitializationFromOtherType0)
{
    const std::uint64_t u = 1423;
    const float f = u;
    F32 f0{u};
    const F32 f1{u};
    F32 f2 = u;
    const F32 f3 = u;
    F32 f4(u);
    const F32 f5(u);
    F32 f6 = {u};
    const F32 f7 = {u};

    ASSERT_EQ(f0, f);
    ASSERT_EQ(f1, f);
    ASSERT_EQ(f2, f);
    ASSERT_EQ(f3, f);
    ASSERT_EQ(f4, f);
    ASSERT_EQ(f5, f);
    ASSERT_EQ(f6, f);
    ASSERT_EQ(f7, f);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f1, f3);
}

TEST(TestFloat, BasicInitializationFromOtherType1)
{
    std::uint64_t u = 1423;
    const float f = u;
    F32 f0{u};
    const F32 f1{u};
    F32 f2 = u;
    const F32 f3 = u;
    F32 f4(u);
    const F32 f5(u);
    F32 f6 = {u};
    const F32 f7 = {u};

    ASSERT_EQ(f0, f);
    ASSERT_EQ(f1, f);
    ASSERT_EQ(f2, f);
    ASSERT_EQ(f3, f);
    ASSERT_EQ(f4, f);
    ASSERT_EQ(f5, f);
    ASSERT_EQ(f6, f);
    ASSERT_EQ(f7, f);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f2);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f1, f3);
}

TEST(TestFloat, TestArithmeticOperators)
{
    const float f0 = 0.234f;
    const float f1 = 0.625f;

    const F32 fq0{f0};
    const F32 fq1{f1};

    const float f_mul_exp = f0 * f1;
    const float f_div_exp = f0 / f1;
    const float f_add_exp = f0 + f1;
    const float f_sub_exp = f0 - f1;

    const F32 f0_mul_res = fq0 * fq1;
    const F32 f1_mul_res = f0 * fq1;
    const F32 f2_mul_res = fq0 * f1;

    const F32 f0_div_res = fq0 / fq1;
    const F32 f1_div_res = f0 / fq1;
    const F32 f2_div_res = fq0 / f1;
    
    const F32 f0_add_res = fq0 + fq1;
    const F32 f1_add_res = f0 + fq1;
    const F32 f2_add_res = fq0 + f1;

    const F32 f0_sub_res = fq0 - fq1;
    const F32 f1_sub_res = f0 - fq1;
    const F32 f2_sub_res = fq0 - f1;

    ASSERT_EQ(f_mul_exp, f0_mul_res);
    ASSERT_EQ(f_mul_exp, f1_mul_res);
    ASSERT_EQ(f_mul_exp, f2_mul_res);

    ASSERT_EQ(f_div_exp, f0_div_res);
    ASSERT_EQ(f_div_exp, f1_div_res);
    ASSERT_EQ(f_div_exp, f2_div_res);

    ASSERT_EQ(f_add_exp, f0_add_res);
    ASSERT_EQ(f_add_exp, f1_add_res);
    ASSERT_EQ(f_add_exp, f2_add_res);

    ASSERT_EQ(f_sub_exp, f0_sub_res);
    ASSERT_EQ(f_sub_exp, f1_sub_res);
    ASSERT_EQ(f_sub_exp, f2_sub_res);
}

TEST(TestFloat, TestSerialization)
{
    F32 f = 0.124f;
    uint8_t* serialized_data = new uint8_t[f.serializedSize()];

    f.serialize(serialized_data);

    F32 f_out;
    f_out.deserialize(serialized_data);

    ASSERT_EQ(f_out, f);
}
