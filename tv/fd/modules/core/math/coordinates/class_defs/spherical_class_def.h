#ifndef MODULES_CORE_MATH_SPHERICAL_CLASS_H_
#define MODULES_CORE_MATH_SPHERICAL_CLASS_H_

#include "core/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct SphericalCoord
{
    // phi for azimuth, theta for inclination
    T r;
    T phi;
    T theta;

    SphericalCoord(const T r_, const T phi_, const T theta_);
    SphericalCoord();

    CylindricalCoord<T> toCylindricalVec() const;
    Vec3D<T> toVec3D() const;
};

}  // namespace dvs

#endif
