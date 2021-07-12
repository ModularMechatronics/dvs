#ifndef DVS_TRIANGLE_CLASS_H_
#define DVS_TRIANGLE_CLASS_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Triangle2D
{
    Point2D<T> p0;
    Point2D<T> p1;
    Point2D<T> p2;
    Triangle2D();
    Triangle2D(const Point2D<T>& p0_, const Point2D<T>& p1_, const Point2D<T>& p2_);
    template <typename Y> Triangle2D(const Triangle2D<Y>& t_other);
};

template <typename T> struct Triangle3D
{
    Point3D<T> p0;
    Point3D<T> p1;
    Point3D<T> p2;
    Triangle3D();
    Triangle3D(const Point3D<T>& p0_, const Point3D<T>& p1_, const Point3D<T>& p2_);
    template <typename Y> Triangle3D(const Triangle3D<Y>& t_other);
};

}  // namespace dvs

#endif
