#ifndef DVS_COMPLEX_CLASS_H_
#define DVS_COMPLEX_CLASS_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct ComplexCoord
{
    T real;
    T imag;

    ComplexCoord(const T real_, const T imag_);
    ComplexCoord();

    PolarCoord<T> toPolar() const;
    ComplexCoord<T> conjugate() const;
    ComplexCoord<T> inv() const;
    Vec2D<T> toVec2D() const;
    T abs() const;
};
}  // namespace dvs

#endif
