#ifndef DVS_MATH_PRE_DEFS_H_
#define DVS_MATH_PRE_DEFS_H_

#include "dvs/math/misc/forward_decl.h"

namespace dvs
{
typedef AxisAngle<double> AxisAngled;
typedef HomogeneousLine2D<double> HomogeneousLine2Dd;
typedef ParametricLine2D<double> ParametricLine2Dd;
typedef Line3D<double> Line3Dd;
typedef Plane<double> Planed;
typedef Triangle2D<double> Triangle2Dd;
typedef Triangle3D<double> Triangle3Dd;
typedef Matrix<double> Matrixd;
typedef Vector<double> Vectord;
typedef Point<double> Pointd;
typedef Vec2<double> Vec2d;
typedef Vec3<double> Vec3d;
typedef Vec4<double> Vec4d;
typedef Point2<double> Point2d;
typedef Point3<double> Point3d;
typedef Point4<double> Point4d;

typedef AxisAngle<float> AxisAnglef;
typedef HomogeneousLine2D<float> HomogeneousLine2Df;
typedef ParametricLine2D<float> ParametricLine2Df;
typedef Line3D<float> Line3Df;
typedef Plane<float> Planef;
typedef Triangle2D<float> Triangle2Df;
typedef Triangle3D<float> Triangle3Df;
typedef Matrix<float> Matrixf;
typedef Vector<float> Vectorf;
typedef Point<float> Pointf;
typedef Vec2<float> Vec2f;
typedef Vec3<float> Vec3f;
typedef Vec4<float> Vec4f;
typedef Point2<float> Point2f;
typedef Point3<float> Point3f;
typedef Point4<float> Point4f;

typedef Vec2<int> Vec2i;

}  // namespace dvs

#endif // DVS_MATH_PRE_DEFS_H_
