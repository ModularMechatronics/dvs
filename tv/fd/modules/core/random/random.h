#ifndef MODULES_CORE_RANDOM_RANDOM_H_
#define MODULES_CORE_RANDOM_RANDOM_H_

#include <time.h>
#include <stdlib.h>

#include <algorithm>
#include <random>
#include <vector>
#include <functional>

#include "core/types.h"



namespace internal
{

using RandFunctionType = std::function<int(void)>;

RandFunctionType& getRandFunction()
{
    static RandFunctionType rand_func = rand;
    return rand_func;
}

}

// #ifdef DEBUG

#define __standardLibraryRandFunction (internal::getRandFunction())

// #else

// #define __standardLibraryRandFunction rand

// #endif

template <typename T>
T randomNumber()
{
    return __standardLibraryRandFunction();
}

template <typename T>
T randomNumber(const int64_t max_val)
{
    return __standardLibraryRandFunction() % (max_val + 1);
}

// Returns a uniform random number in the interval [0.0, 1.0]
F64 uniformRandomF64()
{
    return static_cast<F64>(__standardLibraryRandFunction() % 10001) / 10000.0;
}

// Returns a uniform random number in the interval [lower, upper]
F64 uniformRandomF64(const F64 lower, const F64 upper)
{
    const F64 scale = upper - lower;
    return uniformRandomF64() * scale + lower;
}

// Returns a uniform random number in the interval [0.0f, 1.0f]
F32 uniformRandomF32()
{
    return static_cast<F32>(__standardLibraryRandFunction() % 10001) / 10000.0f;
}

// Returns a uniform random number in the interval [lower, upper]
F32 uniformRandomF32(const F32 lower, const F32 upper)
{
    const F32 scale = upper - lower;
    return uniformRandomF32() * scale + lower;
}

// Returns a uniform random number in the interval [lower, upper]
S64 uniformRandomS64(const S64 lower, const S64 upper)
{
    const S64 scale = upper - lower;
    return (__standardLibraryRandFunction() % (scale + 1)) + lower;
}

// Returns a uniform random number in the interval [0, RAND_MAX]
S64 uniformRandomS64()
{
    return __standardLibraryRandFunction();
}

// Returns a uniform random number in the interval [lower, upper]
S64 uniformRandomU64(const S64 lower, const S64 upper)
{
    const U64 scale = upper - lower;
    return __standardLibraryRandFunction() % (scale + 1) + lower;
}

// Returns a uniform random number in the interval [0, RAND_MAX]
S64 uniformRandomU64()
{
    return __standardLibraryRandFunction();
}

template <typename T>
T normalDistributionRandomNumber(const T mean, const T stddev)
{
    std::random_device rd;

    std::mt19937 gen(rd());

    std::normal_distribution<T> d(mean, stddev);

    return d(gen);
}

template <typename T>
void randomPermutation(const std::vector<T>& in, std::vector<T>& out)
{
    out = in;
    std::random_shuffle(out.begin(), out.end());
}

template <typename T>
void randomPermutation(std::vector<T>& out)
{
    std::random_shuffle(out.begin(), out.end());
}

inline void setRandomSeed(const U32 seed)
{
    srand(seed);
}

inline void setRandomSeedToTimeNow()
{
    srand(time(NULL));
}

#endif
