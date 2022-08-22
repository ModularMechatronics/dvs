#ifndef DVS_MATH_COORDINATES_CLASS_DEFS_SPHERICAL_CLASS_DEF_H_
#define DVS_MATH_COORDINATES_CLASS_DEFS_SPHERICAL_CLASS_DEF_H_

#include "dvs/math/misc/forward_decl.h"

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
    Vec3<T> toVec3() const;
};

}  // namespace dvs

#endif // DVS_MATH_COORDINATES_CLASS_DEFS_SPHERICAL_CLASS_DEF_H_
