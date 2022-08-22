#ifndef DVS_MATH_TRANSFORMATIONS_CLASS_DEF_AXIS_ANGLE_H_
#define DVS_MATH_TRANSFORMATIONS_CLASS_DEF_AXIS_ANGLE_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct AxisAngle
{
    T phi;
    T x;
    T y;
    T z;

    AxisAngle(const T phi_, const T x_, const T y_, const T z_);
    AxisAngle(const T x_, const T y_, const T z_);
    AxisAngle(const Vec3<T>& v);
    AxisAngle();

    AxisAngle<T> normalized() const;

    Matrix<T> toRotationMatrix() const;
    Quaternion<T> toQuaternion() const;
    RollPitchYaw<T> toRollPitchYaw() const;
};

}  // namespace dvs

#endif // DVS_MATH_TRANSFORMATIONS_CLASS_DEF_AXIS_ANGLE_H_
