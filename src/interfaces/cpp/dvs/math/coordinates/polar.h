#ifndef DVS_MATH_COORDINATES_POLAR_H_
#define DVS_MATH_COORDINATES_POLAR_H_

#include <cmath>
#include <vector>

#include "dvs/math/coordinates/class_defs/polar.h"
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

template <typename T> Vec2<T> PolarCoord<T>::toVec2() const
{
    return Vec2<T>(r * std::cos(phi), r * std::sin(phi));
}

}  // namespace dvs

#endif // DVS_MATH_COORDINATES_POLAR_H_
