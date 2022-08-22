#ifndef DVS_MATH_COORDINATES_CLASS_DEF_POLAR_H_
#define DVS_MATH_COORDINATES_CLASS_DEF_POLAR_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct PolarCoord
{
    T r;
    T phi;

    PolarCoord(const T r_, const T phi_);
    PolarCoord();

    ComplexCoord<T> toComplex() const;
    Vec2<T> toVec2() const;
};

}  // namespace dvs

#endif // DVS_MATH_COORDINATES_CLASS_DEF_POLAR_H_
