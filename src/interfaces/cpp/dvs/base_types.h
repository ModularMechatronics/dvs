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

}  // namespace dvs

#endif // DVS_BASE_TYPES_H_
