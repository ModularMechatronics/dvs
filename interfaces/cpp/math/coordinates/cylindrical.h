#ifndef DVS_CYLINDRICAL_H_
#define DVS_CYLINDRICAL_H_

#include <cmath>
#include <vector>

#include "logging.h"
#include "math/coordinates/class_defs/cylindrical_class_def.h"
#include "math/lin_alg.h"

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

template <typename T> Vec3D<T> CylindricalCoord<T>::toVec3D() const
{
    return Vec3D<T>(r * std::cos(phi), r * std::sin(phi), z);
}
}  // namespace dvs

#endif
