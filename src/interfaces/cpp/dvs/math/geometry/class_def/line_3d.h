#ifndef DVS_MATH_GEOMETRY_CLASS_DEF_LINE_3D_H_
#define DVS_MATH_GEOMETRY_CLASS_DEF_LINE_3D_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Line3D
{
    // Point on line
    Point3<T> p;

    // Vector in line direction
    Vec3<T> v;

    Line3D(const Point3<T>& p_, const Vec3<T>& v_);
    template <typename Y> Line3D(const Line3D<Y>& l);
    Line3D();

    Point3<T> eval(const T t) const;
};
}  // namespace dvs

#endif // DVS_MATH_GEOMETRY_CLASS_DEF_LINE_3D_H_
