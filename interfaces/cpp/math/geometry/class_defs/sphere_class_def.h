#ifndef DVS_SPHERE_CLASS_H_
#define DVS_SPHERE_CLASS_H_

#include <utility>

#include "math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Sphere
{
    T radius;
    Point3D<T> center;

    Sphere(const T radius_, const Point3D<T>& center_);
    Sphere();

    bool doesLineIntersect(const Line3D<T>& line) const;
    // std::vector<Point3D<T>> lineIntersectionPoint(const Line3D<T>& line) const;
    std::pair<Point3D<T>, Point3D<T>> lineIntersectionPoint(const Line3D<T>& line) const;
};
}  // namespace dvs

#endif
