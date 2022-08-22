#ifndef DVS_MATH_GEOMETRY_CLASS_DEF_PLANE_H_
#define DVS_MATH_GEOMETRY_CLASS_DEF_PLANE_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
template <typename T> struct Plane
{
    T a;
    T b;
    T c;
    T d;

    Plane(const T a_, const T b_, const T c_, const T d_);
    Plane(const Point3<T>& point, const Vec3<T>& normal_vector);
    template <typename Y> Plane(const Plane<Y>& p);
    Plane();

    Plane<T> normalized() const;
    Vec3<T> normal() const;
    Vec3<T> normalizedNormal() const;
    Point3<T> lineIntersection(const Line3D<T>& line) const;
    Point3<T> closestPointOnPlaneFromPoint(const Point3<T>& p) const;
    T pointDistanceFromPlane(const Point3<T>& p) const;
    T eval(const Point3<T>& p) const;
    T evalXY(const T x, const T y) const;
    T evalXZ(const T x, const T z) const;
    T evalYZ(const T y, const T z) const;

    Plane<T> rotatePlaneAroundLine(const Line3D<T>& line) const;
    Point3<T> mirroredPoint(const Point3<T>& point) const;
    Line3D<T> mirroredLine(const Line3D<T>& line) const;
    Plane<T> mirroredPlane(const Plane<T>& plane) const;
    std::pair<Point3<T>, Point3<T>> circleIntersection(const Circle3D<T> circle) const;
    Circle3D<T> sphereIntersection(const Sphere<T>& sphere) const;
    std::pair<Point3<T>, Vec3<T>> projectPointAndVectorOntoPlane(const Point3<T>& point, const Vec3<T>& vec) const;
    Line3D<T> projectLineOntoPlane(const Line3D<T>& line) const;
    Plane<T> planeIntersection(const Plane<T>& plane) const;
    Plane<T> translatePlane(const Vec3<T>& vec) const;
};
}  // namespace dvs

#endif // DVS_MATH_GEOMETRY_CLASS_DEF_PLANE_H_
