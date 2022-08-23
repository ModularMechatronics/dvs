
#ifndef DVS_MATH_GEOMETRY_LINE_3D_H_
#define DVS_MATH_GEOMETRY_LINE_3D_H_

#include <cmath>
#include <vector>

#include "dvs/math/geometry/class_def/line_3d.h"
#include "dvs/math/lin_alg.h"

namespace dvs
{
template <typename T> Line3D<T>::Line3D(const Point3<T>& p_, const Vec3<T>& v_) : p(p_), v(v_) {}

template <typename T> Line3D<T>::Line3D() {}

template <typename T> template <typename Y> Line3D<T>::Line3D(const Line3D<Y>& l)
{
    p = l.p;
    v = l.v;
}

template <typename T> Point3<T> Line3D<T>::eval(const T t) const
{
    return p + t * v;
}

}  // namespace dvs

#endif // DVS_MATH_GEOMETRY_LINE_3D_H_
