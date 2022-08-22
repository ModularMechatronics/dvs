#ifndef DVS_MATH_TRANSFORMATIONS_CLASS_DEFS_QUATERNION_CLASS_DEF_H_
#define DVS_MATH_TRANSFORMATIONS_CLASS_DEFS_QUATERNION_CLASS_DEF_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Quaternion
{
    // w is real component, (x, y, z) are imaginary components
    T w;
    T x;
    T y;
    T z;

    Quaternion(const T w_, const T x_, const T y_, const T z_);
    Quaternion();

    Matrix<T> toRotationMatrix() const;
    AxisAngle<T> toAxisAngle() const;
    RollPitchYaw<T> toRollPitchYaw() const;

    T norm() const;
    T squaredNorm() const;
    Quaternion<T> normalized() const;
};
}  // namespace dvs

#endif // DVS_MATH_TRANSFORMATIONS_CLASS_DEFS_QUATERNION_CLASS_DEF_H_
