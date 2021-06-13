#include "test_utils.h"
#include "dvs.h"

#include <algorithm>

using namespace dvs;

void plotSomething(int i)
{
    Vector<double> x(10), y(10), z(10);

    for(size_t k = 0; k < 10; k++)
    {
        x(k) = k;
        y(k) = 2 * x(k);
        z(k) = 2 * std::sin(x(k) + static_cast<double>(i));
    }
    plot3(x, y, z, properties::Color(12, 14, 55),
                       properties::LineWidth(1),
                       properties::PointSize(137));
}

void testBasic()
{
    Vector<double> x(20), y(20), z(20);

    for(size_t k = 0; k < 20; k++)
    {
        x(k) = k;
        y(k) = 2 * x(k);
        z(k) = 2 * std::sin(x(k));
    }

    setCurrentElement("view_00");
    sleepMS(20);
    view(22.4f, 0.14f);
    sleepMS(20);
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    sleepMS(20);
    plot(x, y, properties::Color(12, 14, 55),
               properties::Alpha(137),
               properties::Name("SimpleName"),
               properties::LineStyle("-*"),
               properties::LineWidth(1),
               properties::EdgeColor(55, 21, 7),
               properties::FaceColor(52, 26, 3),
               properties::ColorMap::MAGMA(),
               properties::PointSize(137));
    sleepMS(20);
    setCurrentElement("view_01");
    sleepMS(20);
    plot3(x, y, z, properties::Color(12, 14, 55),
                   properties::Alpha(137),
                   properties::Name("SimpleName"),
                   properties::LineStyle("-*"),
                   properties::LineWidth(1),
                   properties::EdgeColor(55, 21, 7),
                   properties::FaceColor(52, 26, 3),
                   properties::ColorMap::MAGMA(),
                   properties::PointSize(137));
    sleepMS(20);
    setCurrentElement("view_02");
    sleepMS(20);
    plot3(x, y, z, properties::Color(12, 14, 55),
                   properties::Alpha(137),
                   properties::Name("SimpleName"),
                   properties::LineStyle("-*"),
                   properties::LineWidth(1),
                   properties::EdgeColor(55, 21, 7),
                   properties::FaceColor(52, 26, 3),
                   properties::ColorMap::MAGMA(),
                   properties::PointSize(137));
    sleepMS(20);
    setCurrentElement("view_00");

    for(int i = 0; i < 100; i++)
    {
        sleepMS(20);
        plotSomething(i);
    }
}

void testSurf()
{
    const size_t num_rows = 20, num_cols = 20;
    Matrix<int8_t> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);

    double inc = 0.4;
    for(int r = 0; r < num_rows; r++)
    {
        for(int c = 0; c < num_cols; c++)
        {
            const double rd = static_cast<double>(r - 5) * inc;
            const double cd = static_cast<double>(c - 5) * inc * 2;
            x(r, c) = c;
            y(r, c) = r;
            z(r, c) = 50.0 * std::sin(std::sqrt(rd * rd + cd * cd));
        }
    }

    setCurrentElement("view_00");
    hardClearFigure();
    surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::FaceColor(150, 244, 244));
}

void testScatter()
{
    Vector<double> x(20), y(20);
    Vector<float> xf(20), yf(20);

    double t = 0.0;

    for(size_t k = 0; k < 20; k++)
    {
        xf(k) = k;
        yf(k) = 5.0 * std::sin(3.0 * t);

        x(k) = xf(k);
        y(k) = yf(k);

        t = t + 0.1;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    holdOn();
    plot(xf, yf, properties::Color(212, 14, 55));
    scatter(x, y, properties::Color(12, 14, 55), properties::PointSize(3));
}

void testScatter3()
{
    const size_t num_elements = 30;
    Vector<int16_t> x(num_elements), y(num_elements), z(num_elements);
    Vector<float> xf(num_elements), yf(num_elements), zf(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        xf(k) = 10.0 * cos(t) + 20.0;
        yf(k) = 10.0 * sin(t) + 20.0;
        zf(k) = k;

        x(k) = xf(k);
        y(k) = yf(k);
        z(k) = k;
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    scatter3(x, y, z, properties::Color(212, 14, 55), properties::PointSize(3));
    plot3(xf, yf, zf, properties::Color(21, 14, 55), properties::LineWidth(1));
}

void testPlot2()
{
    const size_t num_elements = 30;
    Vector<int64_t> x(num_elements), y(num_elements);
    Vector<float> xf(num_elements), yf(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        xf(k) = 10.0 * cos(t) + 20.0;
        yf(k) = 10.0 * sin(t) + 20.0 + k;

        x(k) = xf(k);
        y(k) = yf(k);
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    plot(x, y, properties::Color(212, 14, 55), properties::LineWidth(1));
    plot(xf, yf, properties::Color(21, 14, 55), properties::LineWidth(1));
}

void testPlot3()
{
    const size_t num_elements = 30;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);
    Vector<float> xf(num_elements), yf(num_elements), zf(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        xf(k) = 10.0 * cos(t) + 20.0;
        yf(k) = 10.0 * sin(t) + 20.0;
        zf(k) = k;

        x(k) = xf(k);
        y(k) = yf(k);
        z(k) = k;
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    plot3(x, y, z, properties::Color(212, 14, 55), properties::LineWidth(1));
    plot3(xf, yf, zf, properties::Color(21, 14, 55), properties::LineWidth(1));
}

void testImShow()
{
    setCurrentElement("view_00");

    const uint32_t num_rows = 8, num_cols = 10;
    Matrix<double> img(num_rows, num_cols);
    const double max_val = num_rows * num_cols;

    for(uint32_t r = 0; r < num_rows; r++)
    {
        for(uint32_t c = 0; c < num_cols; c++)
        {
            img(r, c) = (r * c) / max_val;
        }
    }

    imShow(img, properties::Alpha(137));
}

void testPolygonFrom4Points()
{
    setCurrentElement("view_00");

    Point3D<double> p0(0.0, 0.0, 0.0),
                    p1(1.0, 0.0, 1.0),
                    p2(1.0, 1.0, 2.0),
                    p3(0.0, 1.0, 3.0);
    drawPolygonFrom4Points(p0, p1, p2, p3);
}


void testdrawXYPlane()
{
    VecXY<double> p0_xy(0.0, 0.0),
                  p1_xy(1.0, 1.0);
    VecXZ<double> p0_xz(0.0, 0.0),
                  p1_xz(1.0, 1.0);
    VecYZ<double> p0_yz(0.0, 0.0),
                  p1_yz(1.0, 1.0);
    setCurrentElement("view_00");
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});
    drawPlaneXY(p0_xy, p1_xy, Plane<double>(0.1, 0.1, 0.1, 0.4));
    
    setCurrentElement("view_01");
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});
    drawPlaneXZ(p0_xz, p1_xz, Plane<double>(0.1, 0.1, 0.1, 0.4));
    
    setCurrentElement("view_02");
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});
    drawPlaneYZ(p0_yz, p1_yz, Plane<double>(0.1, 0.1, 0.1, 0.4));
}

void testMultipleStuff()
{
    const size_t num_elements = 50;
    Vector<float> x(num_elements), y(num_elements), z(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        x(k) = 10.0 * cos(t);
        y(k) = 10.0 * sin(t);
        z(k) = t;
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    view(-106.5f, 16.3f);
    axis({-5.0, -5.0, -5.0}, {5.0, 5.0, 5.0});
    plot3(x, y, z, properties::Color(12, 14, 55), properties::LineWidth(1));
    scatter3(x, y, z, properties::Color(255, 0, 0), properties::PointSize(3));
}

void testAxis2D()
{
    const size_t num_elements = 50;
    Vector<double> x(num_elements), y(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        x(k) = t;
        y(k) = 10.0 * cos(t);
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    plot(x, y, properties::Color(12, 14, 55), properties::LineWidth(1));
}

void testAxis3D()
{
    const size_t num_elements = 50;
    Vector<float> x(num_elements), y(num_elements), z(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        x(k) = 10.0 * cos(t);
        y(k) = 10.0 * sin(t);
        z(k) = t;
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    plot3(x, y, z, properties::Color(12, 14, 55), properties::LineWidth(1));
    scatter3(x, y, z, properties::Color(255, 0, 0), properties::PointSize(3));
}

void testDrawTriangles()
{
    const size_t num_elements = 10;
    using tp = double;
    Vector<Triangle3D<tp>> triangles(num_elements);

    double t = 0.0;
    Point3D<tp> p0(-1.0, -1.0, 0.0), p1(1.0, -1.0, 0.0), p2(0.0, 1.0, 0.0);

    for(size_t k = 0; k < num_elements; k++)
    {
        const Matrix<tp> r = rotationMatrixZ(t);

        const Point3D<tp> p0r = r * p0 + Point3D<tp>(0.0, 0.0, t);
        const Point3D<tp> p1r = r * p1 + Point3D<tp>(0.0, 0.0, t);
        const Point3D<tp> p2r = r * p2 + Point3D<tp>(0.0, 0.0, t);

        Triangle3D<tp> tri(p0r, p1r, p2r);
        triangles(k) = tri;

        t = t + 0.3;
    }

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();

    drawTriangles(triangles, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawMesh()
{
    using tp = double;

    Vector<Point3D<tp>> vertices(4);
    vertices(0) = Point3D<tp>(-1.0, -1.0, -1.0);
    vertices(1) = Point3D<tp>(1.0, -1.0, -1.0);
    vertices(2) = Point3D<tp>(1.0, 1.0, -1.0);
    vertices(3) = Point3D<tp>(0.0, 0.0, 1.0);

    Vector<IndexTriplet> indices(4);
    indices(0) = IndexTriplet(0, 1, 2);
    indices(1) = IndexTriplet(0, 1, 3);
    indices(2) = IndexTriplet(1, 2, 3);
    indices(3) = IndexTriplet(2, 0, 3);

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawMesh(vertices, indices, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawSingleTriangle()
{
    using tp = double;

    Triangle3D<tp> triangle;
    triangle.p0 = Point3D<tp>(-1.0, -1.0, -1.0);
    triangle.p1 = Point3D<tp>(1.0, -1.0, -1.0);
    triangle.p2 = Point3D<tp>(1.0, 1.0, 1.0);

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawTriangle(triangle, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawLine3D()
{
    using tp = double;

    Line3D<tp> line(Point3D<tp>(0.1, 0.2, 0.2), Vec3D<tp>(1.0, 1.0, 1.0));
    tp t0 = 0;
    tp t1 = 1.0;

    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLine(line, t0, t1, properties::Color(12, 244, 244));
}

void testDrawLineBetweenPoints3D()
{
    using tp = double;

    Point3D<tp> p0(0.1, 0.2, 0.2);
    Point3D<tp> p1(2.0, 1.0, 1.0);
    
    setCurrentElement("view_00");
    hardClearFigure();
    holdOn();
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLineBetweenPoints(p0, p1, properties::Color(12, 244, 0));
}
