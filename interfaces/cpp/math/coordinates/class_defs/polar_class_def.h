#ifndef DVS_POLAR_CLASS_H_
#define DVS_POLAR_CLASS_H_

#include "math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct PolarCoord
{
    T r;
    T phi;

    PolarCoord(const T r_, const T phi_);
    PolarCoord();

    ComplexCoord<T> toComplex() const;
    Vec2D<T> toVec2D() const;
};

}  // namespace dvs

#endif
