#include <gtest/gtest.h>

#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "dvs/math/pre_defs.h"

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
    Matrix<double> m(3, 3), m2(3, 3);
    m = m2;

    Vec2<double> v2;
    Vec3<double> v3;
    Vec4<double> v4;

    VecXY<double> vXY;
    VecXZ<double> vXZ;
    VecYZ<double> vYZ;

    ComplexCoord<double> cc;
    PolarCoord<double> pc;
    CylindricalCoord<double> cyl_coord;
    SphericalCoord<double> sc;

    Circle3D<double> c3d;
    Circle2D<double> c2d;
    Line3D<double> l3d;
    ParametricLine2D<double> pl;
    HomogeneousLine2D<double> hl;
    Plane<double> plane;
    Sphere<double> sphere;
    Triangle2D<double> tri2d;
    Triangle3D<double> tri3d;

    RollPitchYaw<double> rpy;
    AxisAngle<double> aa;
    PoseSE3<double> pse3;

    Point2<double> pt2D;
    Point3<double> pt3D;
    Point4<double> pt4D;
    Point<double> ptnt;
    PointXY<double> ptXY;
    PointXZ<double> ptXZ;
    PointYZ<double> ptYZ;

    Vectord vd;
}
