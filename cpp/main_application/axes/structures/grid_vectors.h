#ifndef GRID_VECTORS_H_
#define GRID_VECTORS_H_

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

struct GridVectors
{
    Vectord x;
    Vectord y;
    Vectord z;
    GridVectors(const size_t num_lines)
    {
        x.resize(num_lines);
        y.resize(num_lines);
        z.resize(num_lines);
    }
    GridVectors() = default;
    void resizeX(const size_t num_lines)
    {
        x.resize(num_lines);
    }
    void resizeY(const size_t num_lines)
    {
        y.resize(num_lines);
    }
    void resizeZ(const size_t num_lines)
    {
        z.resize(num_lines);
    }
};

#endif
