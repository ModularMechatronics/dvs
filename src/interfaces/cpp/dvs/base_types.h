#ifndef DVS_BASE_TYPES_H_
#define DVS_BASE_TYPES_H_

#include <stdlib.h>

#include "dvs/enumerations.h"

namespace dvs
{
namespace internal
{
struct Dimension2D
{
    size_t rows;
    size_t cols;

    Dimension2D() : rows(0), cols(0) {}
    Dimension2D(const size_t r, const size_t c) : rows(r), cols(c) {}
};

}  // namespace internal

struct Bound2D
{
    double x;
    double y;

    Bound2D() : x(0), y(0) {}
    Bound2D(const double x_, const double y_) : x(x_), y(y_) {}
};

struct Bound3D
{
    double x;
    double y;
    double z;

    Bound3D() : x(0), y(0), z(0) {}
    Bound3D(const double x_, const double y_, const double z_) : x(x_), y(y_), z(z_) {}
};

}  // namespace dvs

#endif // DVS_BASE_TYPES_H_
