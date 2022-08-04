#ifndef MODULES_CORE_MATH_CYLINDRICAL_CLASS_H_
#define MODULES_CORE_MATH_CYLINDRICAL_CLASS_H_

#include "core/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct CylindricalCoord
{
    T r;
    T z;
    T phi;

    CylindricalCoord(const T r_, const T z_, const T phi_);
    CylindricalCoord();

    SphericalCoord<T> toSphericalCoord() const;
    Vec3D<T> toVec3D() const;
};

}  // namespace dvs

#endif
