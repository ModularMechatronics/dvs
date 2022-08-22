#ifndef DVS_MATH_COORDINATES_SPHERICAL_H_
#define DVS_MATH_COORDINATES_SPHERICAL_H_

#include <cmath>
#include <vector>

#include "dvs/math/coordinates/class_defs/spherical_class_def.h"
#include "dvs/math/lin_alg.h"

namespace dvs
{
template <typename T> SphericalCoord<T>::SphericalCoord(const T r_, const T phi_, const T theta_)
{
    r = r_;
    phi = phi_;
    theta = theta_;
}

template <typename T> SphericalCoord<T>::SphericalCoord() {}

template <typename T> CylindricalCoord<T> SphericalCoord<T>::toCylindricalVec() const
{
    return CylindricalCoord<T>(r * std::sin(theta), r * std::cos(theta), phi);
}

template <typename T> Vec3<T> SphericalCoord<T>::toVec3() const
{
    return r * Vec3<T>(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
}
}  // namespace dvs

#endif // DVS_MATH_COORDINATES_SPHERICAL_H_
