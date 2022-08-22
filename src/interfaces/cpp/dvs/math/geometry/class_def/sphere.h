#ifndef DVS_MATH_GEOMETRY_CLASS_DEFS_SPHERE_CLASS_DEF_H_
#define DVS_MATH_GEOMETRY_CLASS_DEFS_SPHERE_CLASS_DEF_H_

#include <utility>

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Sphere
{
    T radius;
    Point3<T> center;

    Sphere(const T radius_, const Point3<T>& center_);
    Sphere();

    bool doesLineIntersect(const Line3D<T>& line) const;
    // std::vector<Point3<T>> lineIntersectionPoint(const Line3D<T>& line) const;
    std::pair<Point3<T>, Point3<T>> lineIntersectionPoint(const Line3D<T>& line) const;
};
}  // namespace dvs

#endif // DVS_MATH_GEOMETRY_CLASS_DEFS_SPHERE_CLASS_DEF_H_
