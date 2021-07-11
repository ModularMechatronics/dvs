#ifndef DVS_ANGLE_AXIS_CLASS_H_
#define DVS_ANGLE_AXIS_CLASS_H_

#include "math/misc/forward_decl.h"

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
    AxisAngle(const Vec3D<T>& v);
    AxisAngle();

    AxisAngle<T> normalized() const;

    Matrix<T> toRotationMatrix() const;
    Quaternion<T> toQuaternion() const;
    RollPitchYaw<T> toRollPitchYaw() const;
};

}  // namespace dvs

#endif
