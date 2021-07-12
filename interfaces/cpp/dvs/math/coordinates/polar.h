#ifndef DVS_POLAR_H_
#define DVS_POLAR_H_

#include <cmath>
#include <vector>

#include "dvs/math/coordinates/class_defs/polar_class_def.h"
#include "dvs/math/lin_alg.h"

namespace dvs
{
// PolarCoord
template <typename T> PolarCoord<T>::PolarCoord(const T r_, const T phi_)
{
    r = r_;
    phi = phi_;
}

template <typename T> PolarCoord<T>::PolarCoord() {}

template <typename T> ComplexCoord<T> PolarCoord<T>::toComplex() const
{
    return ComplexCoord<T>(r * std::cos(phi), r * std::sin(phi));
}

template <typename T> Vec2D<T> PolarCoord<T>::toVec2D() const
{
    return Vec2D<T>(r * std::cos(phi), r * std::sin(phi));
}

}  // namespace dvs

#endif
