
#ifndef DVS_MATH_MISC_FORWARD_DECL_H_
#define DVS_MATH_MISC_FORWARD_DECL_H_

#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include "dvs/math/misc/math_macros.h"

namespace dvs
{
template <typename T> struct Vec2;
template <typename T> struct Vec3;
template <typename T> struct Vec4;

template <typename T> struct VecXY;
template <typename T> struct VecXZ;
template <typename T> struct VecYZ;

template <typename T> class Vector;
template <typename T> class Matrix;

template <typename T> struct Line3D;
template <typename T> struct ParametricLine2D;
template <typename T> struct HomogeneousLine2D;
template <typename T> struct Plane;
template <typename T> struct Sphere;

template <typename T> struct Triangle2D;
template <typename T> struct Triangle3D;

template <typename T> using Point2 = Vec2<T>;
template <typename T> using Point3 = Vec3<T>;
template <typename T> using Point4 = Vec4<T>;
template <typename T> using Point = Vector<T>;

template <typename T> using PointXY = VecXY<T>;
template <typename T> using PointXZ = VecXZ<T>;
template <typename T> using PointYZ = VecYZ<T>;

}  // namespace dvs

#endif  // DVS_MATH_MISC_FORWARD_DECL_H_
