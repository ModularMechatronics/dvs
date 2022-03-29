#ifndef GRID_VECTORS_H_
#define GRID_VECTORS_H_

#include <cstdint>
#include <array>

#include "dvs/math/math.h"

using namespace dvs;

enum class InteractionType
{
    UNCHANGED,
    PAN,
    ROTATE,
    RESET,
    ZOOM,
};

struct GridVector
{
    static constexpr std::uint8_t kMaxNumGridNumbers = 30;
    std::array<double, kMaxNumGridNumbers> data;
    size_t num_valid_values;
};

struct GridVectors
{
    GridVector x;
    GridVector y;
    GridVector z;
};

#endif
