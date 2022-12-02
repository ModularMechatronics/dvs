#ifndef DVS_BASE_TYPES_H_
#define DVS_BASE_TYPES_H_

#include <stdint.h>
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

struct RGB888
{
    uint8_t red, green, blue;

    RGB888() {}
    RGB888(const uint8_t red_, const uint8_t green_, const uint8_t blue_) : red{red_}, green{green_}, blue{blue_} {}
};

}  // namespace dvs

#endif  // DVS_BASE_TYPES_H_
