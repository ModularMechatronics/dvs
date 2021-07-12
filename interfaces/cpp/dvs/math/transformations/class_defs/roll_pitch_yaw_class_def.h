#ifndef DVS_ROLL_PITCH_YAW_CLASS_H_
#define DVS_ROLL_PITCH_YAW_CLASS_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct RollPitchYaw
{
    // Angles in radians
    T roll;
    T pitch;
    T yaw;

    // Coordinate system:
    // x forward, roll around x, positive rotation clockwise
    // y left, pitch around y, positive rotation down
    // z up, yaw around z, positive rotation to the left

    RollPitchYaw(const T roll_, const T pitch_, const T yaw_);
    RollPitchYaw();

    Matrix<T> toRotationMatrix() const;
    Quaternion<T> toQuaternion() const;
    AxisAngle<T> toAxisAngle() const;
};

}  // namespace dvs

#endif
