#ifndef MODULES_CORE_MATH_COMPLEX_CLASS_H_
#define MODULES_CORE_MATH_COMPLEX_CLASS_H_

#include "core/math/misc/forward_decl.h"

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
