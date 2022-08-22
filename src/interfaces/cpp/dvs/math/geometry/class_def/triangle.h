#ifndef DVS_MATH_GEOMETRY_CLASS_DEFS_TRIANGLE_CLASS_DEF_H_
#define DVS_MATH_GEOMETRY_CLASS_DEFS_TRIANGLE_CLASS_DEF_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Triangle2D
{
    Point2<T> p0;
    Point2<T> p1;
    Point2<T> p2;
    Triangle2D();
    Triangle2D(const Point2<T>& p0_, const Point2<T>& p1_, const Point2<T>& p2_);
    template <typename Y> Triangle2D(const Triangle2D<Y>& t_other);
};

template <typename T> struct Triangle3D
{
    Point3<T> p0;
    Point3<T> p1;
    Point3<T> p2;
    Triangle3D();
    Triangle3D(const Point3<T>& p0_, const Point3<T>& p1_, const Point3<T>& p2_);
    template <typename Y> Triangle3D(const Triangle3D<Y>& t_other);
};

}  // namespace dvs

#endif // DVS_MATH_GEOMETRY_CLASS_DEFS_TRIANGLE_CLASS_DEF_H_
