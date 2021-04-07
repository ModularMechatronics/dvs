#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "math/math.h"

using namespace dvs;

class TestMathFunctions : public testing::Test
{
protected:

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestMathFunctions, TestBasic0)
{
    Vector<double> v;
    Matrix<double> m;

    Vec2D<double> v2;
    Vec3D<double> v3;
    Vec4D<double> v4;

/*VecXY;
VecXZ;
VecYZ;


ComplexCoord;
PolarCoord;
CylindricalCoord;
SphericalCoord;

Circle3D;
Circle2D;
Line3D;
ParametricLine2D;
HomogeneousLine2D;
Plane;
Sphere;

Quaternion;
RollPitchYaw;
AxisAngle;
PoseSE3;
Rect2D;
PolygonNVertices2D;
PolygonNVertices3D;
OrderedSet3D;
OrderedSet2D;
Triangle2D;
Triangle3D;
Interval1D;
IndexSpan;
EndIndex;
Point2D;
Point3D;
Point4D;
Point;
PointXY;
PointXZ;
PointYZ;*/
}
