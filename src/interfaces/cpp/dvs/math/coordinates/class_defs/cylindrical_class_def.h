#ifndef DVS_MATH_COORDINATES_CLASS_DEFS_CYLINDRICAL_CLASS_DEF_H_
#define DVS_MATH_COORDINATES_CLASS_DEFS_CYLINDRICAL_CLASS_DEF_H_

#include "dvs/math/misc/forward_decl.h"

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
    Vec3<T> toVec3() const;
};

}  // namespace dvs

#endif // DVS_MATH_COORDINATES_CLASS_DEFS_CYLINDRICAL_CLASS_DEF_H_
