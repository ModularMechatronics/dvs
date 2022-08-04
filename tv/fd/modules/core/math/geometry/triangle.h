#ifndef MODULES_CORE_MATH_TRIANGLE_H_
#define MODULES_CORE_MATH_TRIANGLE_H_

#include <cmath>
#include <vector>

#include "core/math/geometry/class_defs/triangle_class_def.h"
#include "core/math/lin_alg.h"

namespace dvs
{
template <typename T> Triangle2D<T>::Triangle2D() {}
template <typename T> Triangle2D<T>::Triangle2D(const Point2D<T>& p0_, const Point2D<T>& p1_, const Point2D<T>& p2_)
{
    p0 = p0_;
    p1 = p1_;
    p2 = p2_;
}

template <typename T> template <typename Y> Triangle2D<T>::Triangle2D(const Triangle2D<Y>& t_other)
{
    p0 = t_other.p0;
    p1 = t_other.p1;
    p2 = t_other.p2;
}

template <typename T> Triangle3D<T>::Triangle3D() {}
template <typename T> Triangle3D<T>::Triangle3D(const Point3D<T>& p0_, const Point3D<T>& p1_, const Point3D<T>& p2_)
{
    p0 = p0_;
    p1 = p1_;
    p2 = p2_;
}

template <typename T> template <typename Y> Triangle3D<T>::Triangle3D(const Triangle3D<Y>& t_other)
{
    p0 = t_other.p0;
    p1 = t_other.p1;
    p2 = t_other.p2;
}

}  // namespace dvs

#endif
