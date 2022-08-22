#ifndef DVS_MATH_GEOMETRY_CLASS_DEFS_CIRCLE_CLASS_DEF_H_
#define DVS_MATH_GEOMETRY_CLASS_DEFS_CIRCLE_CLASS_DEF_H_

#include <utility>

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Circle2D
{
    T radius;           // Radius
    Point2<T> center;  // Center point

    Circle2D(const T radius_, const T center_x_, const T center_y_);
    Circle2D(const T radius_, const Vec2<T>& center_);
    Circle2D();

    HomogeneousLine2D<T> tangentLine(const T angle) const;
    bool doesLineIntersect(const HomogeneousLine2D<T>& line) const;
    Point2<T> closestPointOnPerimeterFromPoint(const Point2<T>& p) const;
    std::pair<Point2<T>, Point2<T>> lineIntersectionPoints(const HomogeneousLine2D<T>& line) const;
    bool isPointInCircle(const Point2<T>& p) const;
    bool doesCircleIntersect(const Circle2D<T>& circle) const;
    std::pair<Point2<T>, Point2<T>> circleIntersection(const Circle2D<T>& circle) const;
};

template <typename T> class Circle3D
{
private:
    Vec3<T> v_vec;
    Vec3<T> a_vec;
    Vec3<T> b_vec;
    Point3<T> center;
    void calculateOrthogonalVectors();

public:
    Circle3D(const Vec3<T>& v_, const Vec3<T>& c_);
    Circle3D();
};

}  // namespace dvs

#endif // DVS_MATH_GEOMETRY_CLASS_DEFS_CIRCLE_CLASS_DEF_H_
