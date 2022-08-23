#ifndef DVS_MATH_TRANSFORMATIONS_QUATERNION_H_
#define DVS_MATH_TRANSFORMATIONS_QUATERNION_H_

#include <cmath>

#include "dvs/math/lin_alg.h"
#include "dvs/math/transformations/class_def/quaternion.h"

namespace dvs
{
template <typename T> Quaternion<T>::Quaternion(const T w_, const T x_, const T y_, const T z_)
{
    w = w_;
    x = x_;
    y = y_;
    z = z_;
}

template <typename T> Quaternion<T>::Quaternion() {}

template <typename T> AxisAngle<T> Quaternion<T>::toAxisAngle() const
{
    AxisAngle<T> axis_angle;
    axis_angle.phi = 2.0 * std::acos(w);
    axis_angle.x = x / std::sqrt(1 - w * w);
    axis_angle.y = y / std::sqrt(1 - w * w);
    axis_angle.z = z / std::sqrt(1 - w * w);

    return axis_angle;
}

template <typename T> Matrix<T> Quaternion<T>::toRotationMatrix() const
{
    Matrix<T> m(3, 3);

    const Quaternion<T> qn = this->normalized();
    const T qr = qn.w;
    const T qi = qn.x;
    const T qj = qn.y;
    const T qk = qn.z;

    m(0, 0) = 1.0 - 2.0 * (qj * qj + qk * qk);
    m(0, 1) = 2.0 * (qi * qj - qk * qr);
    m(0, 2) = 2.0 * (qi * qk + qj * qr);
    m(1, 0) = 2.0 * (qi * qj + qk * qr);
    m(1, 1) = 1.0 - 2.0 * (qi * qi + qk * qk);
    m(1, 2) = 2.0 * (qj * qk - qi * qr);
    m(2, 0) = 2.0 * (qi * qk - qj * qr);
    m(2, 1) = 2.0 * (qj * qk + qi * qr);
    m(2, 2) = 1.0 - 2.0 * (qi * qi + qj * qj);

    return m;
}

template <typename T> T Quaternion<T>::norm() const
{
    return std::sqrt(w * w + x * x + y * y + z * z);
}

template <typename T> T Quaternion<T>::squaredNorm() const
{
    return w * w + x * x + y * y + z * z;
}

template <typename T> Quaternion<T> Quaternion<T>::normalized() const
{
    const T d = this->norm();
    return Quaternion<T>(w / d, x / d, y / d, z / d);
}

}  // namespace dvs

#endif // DVS_MATH_TRANSFORMATIONS_QUATERNION_H_
