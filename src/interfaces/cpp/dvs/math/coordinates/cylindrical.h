#ifndef DVS_MATH_COORDINATES_CYLINDRICAL_H_
#define DVS_MATH_COORDINATES_CYLINDRICAL_H_

#include <cmath>
#include <vector>

#include "dvs/math/coordinates/class_defs/cylindrical.h"
#include "dvs/math/lin_alg.h"

namespace dvs
{
template <typename T> CylindricalCoord<T>::CylindricalCoord(const T r_, const T z_, const T phi_)
{
    r = r_;
    z = z_;
    phi = phi_;
}

template <typename T> CylindricalCoord<T>::CylindricalCoord() {}

template <typename T> SphericalCoord<T> CylindricalCoord<T>::toSphericalCoord() const
{
    SphericalCoord<T> sv;

    sv.r = std::sqrt(r * r + z * z);
    sv.theta = std::atan2(r, z);
    sv.phi = phi;

    return sv;
}

template <typename T> Vec3<T> CylindricalCoord<T>::toVec3() const
{
    return Vec3<T>(r * std::cos(phi), r * std::sin(phi), z);
}
}  // namespace dvs

#endif // DVS_MATH_COORDINATES_CYLINDRICAL_H_
