
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
template <typename T> class MatrixView;

template <typename T> struct ComplexCoord;
template <typename T> struct PolarCoord;
template <typename T> struct CylindricalCoord;
template <typename T> struct SphericalCoord;

template <typename T> class Circle3D;
template <typename T> struct Circle2D;
template <typename T> struct Line3D;
template <typename T> struct ParametricLine2D;
template <typename T> struct HomogeneousLine2D;
template <typename T> struct Plane;
template <typename T> struct Sphere;

template <typename T> struct Quaternion;
template <typename T> struct RollPitchYaw;
template <typename T> struct AxisAngle;
template <typename T> class PoseSE3;

template <typename T> struct Triangle2D;
template <typename T> struct Triangle3D;

template <typename T> struct Interval1D;
struct IndexSpan;
struct EndIndex;
struct AllIndices;

template <typename T> using Point2 = Vec2<T>;
template <typename T> using Point3 = Vec3<T>;
template <typename T> using Point4 = Vec4<T>;
template <typename T> using Point = Vector<T>;

template <typename T> using PointXY = VecXY<T>;
template <typename T> using PointXZ = VecXZ<T>;
template <typename T> using PointYZ = VecYZ<T>;

}  // namespace dvs

// clang-format off
/*#include "dvs/math/lin_alg/matrix_dynamic/class_def/matrix_dynamic.h"
#include "dvs/math/lin_alg/vector_dynamic/class_def/vector_dynamic.h"
#include "dvs/math/lin_alg/vector_low_dim/class_def/vec2.h"
#include "dvs/math/lin_alg/vector_low_dim/class_def/vec3.h"
#include "dvs/math/lin_alg/vector_low_dim/class_def/vec4.h"

#include "dvs/math/geometry/class_def/line_2d.h"
#include "dvs/math/geometry/class_def/line_3d.h"
#include "dvs/math/geometry/class_def/plane.h"

#include "dvs/math/transformations/class_def/axis_angle.h"
#include "dvs/math/transformations/class_def/pose.h"
#include "dvs/math/transformations/class_def/quaternion.h"
#include "dvs/math/transformations/class_def/roll_pitch_yaw.h"

#include "dvs/math/structures/end_index.h"
#include "dvs/math/structures/index_span.h"
#include "dvs/math/structures/interval_1d.h"*/
// clang-format on

#endif // DVS_MATH_MISC_FORWARD_DECL_H_
