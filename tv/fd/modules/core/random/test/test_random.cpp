#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "core/random.h"

int newRandFunction()
{
    std::cout << "I was called!" << std::endl;
    return 11;
}

TEST(TestRandom, TestBasic)
{
    internal::getRandFunction() = newRandFunction;
    
    for(size_t k = 0; k < 10; k++)
    {
        std::cout << (randomNumber<S32>() % 101) << std::endl;
    }

    internal::getRandFunction() = rand;
}

TEST(TestRandom, TestNew)
{
    for(size_t k = 0; k < 10; k++)
    {
        std::cout << (randomNumber<S32>() % 101) << std::endl;
    }
}

