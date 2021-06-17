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

    plot(xf, yf, properties::Color(212, 14, 55));
    scatter(x, y, properties::Color(12, 14, 55), properties::PointSize(3));
}

void testScatter3()
{
    const size_t num_elements = 14600;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);

    double t0 = 0.0f, t1 = 0.0f, t2 = 0.0f, y0 = 0.0f, z0 = 0.0f;

    for(size_t k = 0; k < num_elements; k++)
    {
        const float xf = 7.0 * cos(t0) + 20.0f;
        const float yf = 7.0 * sin(t0) - 110.0f;
        const float zf = 0.0f;

        const Vec3Df v0 = rotationMatrixY<float>(t1) * Vec3Df(xf, yf, zf);
        const Vec3Df v1 = rotationMatrixZ<float>(t2) * (v0 + Vec3Df(50.0f, y0, 0.0f));

        x(k) = v1.x;
        y(k) = v1.y;
        z(k) = v1.z + z0 - 70;
        t0 = t0 + 0.3;
        t1 = t1 - 0.01;
        t2 = t2 + 0.002;
        y0 = y0 + 0.05;
        z0 = z0 + 0.02;
    }

    setCurrentElement("view_00");
    hardClearFigure();

    axis({-128.0, -128.0, -128.0}, {128.0, 128.0, 128.0});
    scatter3(x, y, z, properties::Color(212, 14, 55), properties::PointSize(3));
    plot3(x, y, z, properties::Color(21, 14, 55), properties::LineWidth(1));
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

    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    plot3(x, y, z, properties::Color(212, 14, 55), properties::LineWidth(1));
    plot3(xf, yf, zf, properties::Color(21, 14, 55), properties::LineWidth(1));
}

void testImShow()
{
    setCurrentElement("view_00");

    const uint32_t num_rows = 800, num_cols = 800;
    ImageC3<float> img(num_rows, num_cols);
    const double max_val = num_rows * num_cols;

    for(uint32_t r = 0; r < num_rows; r++)
    {
        for(uint32_t c = 0; c < num_cols; c++)
        {
            const float xr = 3.0f * (static_cast<float>(c) - 300.5f) / 800.0f;
            const float yr = 3.0f * (static_cast<float>(r) - 400.5f) / 800.0f;
            const float rr = std::sqrt(xr * xr + yr * yr);

            const float xg = 2.0f * (static_cast<float>(c) - 500.5f) / 800.0f;
            const float yg = 2.0f * (static_cast<float>(r) - 350.5f) / 800.0f;
            const float rg = std::sqrt(xg * xg + yg * yg);

            const float xb = 4.0f * (static_cast<float>(c) - 200.5f) / 800.0f;
            const float yb = 4.0f * (static_cast<float>(r) - 600.5f) / 800.0f;
            const float rb = std::sqrt(xb * xb + yb * yb);

            img(r, c, 0) = (std::sin(rb) / rr + 1.0f) * 0.5f;
            img(r, c, 1) = (std::sin(rb) / rg + 1.0f) * 0.5f;
            img(r, c, 2) = (std::sin(rb) / rb + 1.0f) * 0.5f;

            // img(r, c, 0) = (r * c) / max_val;
            // img(r, c, 1) = 1.0f - (r * c) / max_val;
            // img(r, c, 2) = (r * (num_cols - 1 - c)) / max_val;
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
    VecXZ<float> p0_xz(0.0, 0.0),
                 p1_xz(1.0, 1.0);
    VecYZ<int16_t> p0_yz(0.0, 0.0),
                   p1_yz(1.0, 1.0);
    setCurrentElement("view_00");
    hardClearFigure();
    drawPlaneXY(p0_xy, p1_xy, Plane<float>(0.1, 0.1, 0.1, 0.4), 
        properties::FaceColor(126, 225, 12), properties::EdgeColor(0, 0, 0));

    setCurrentElement("view_01");
    hardClearFigure();
    drawPlaneXZ(p0_xz, p1_xz, Plane<double>(0.1, 0.1, 0.1, 0.4), 
        properties::FaceColor(126, 225, 12), properties::EdgeColor(0, 0, 0));
    
    setCurrentElement("view_02");
    hardClearFigure();
    drawPlaneYZ(p0_yz, p1_yz, Plane<double>(0.1, 0.1, 0.1, 0.4), 
        properties::FaceColor(126, 225, 12), properties::EdgeColor(0, 0, 0));
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

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawTriangle(triangle, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
}

void testDrawLine3D()
{
    using tp = double;

    Line3D<tp> line(Point3D<tp>(0.0, 0.0, 0.0), Vec3D<tp>(1.0, 1.0, 1.0));
    tp t0 = 0;
    tp t1 = 1.0;

    Vector<double> x = {0.0, 1.0};
    Vector<double> y = {0.0, 1.0};
    Vector<double> z = {0.0, 1.0};

    setCurrentElement("view_00");
    hardClearFigure();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLine(line, t0, t1, properties::Color(12, 244, 244));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}

void testDrawLine2D()
{
    using tp = double;

    ParametricLine2D<tp> line(Point2D<tp>(0.0, 0.0), Vec2D<tp>(1.0, 1.0));
    HomogeneousLine2D<tp> line_h(0.1, 0.2, 0.3);
    tp t0 = 0;
    tp t1 = 1.0;

    Vector<double> x = {0.0, 1.0};
    Vector<double> y = {0.0, 1.0};
    Vector<double> z = {0.0, 0.0};

    setCurrentElement("view_00");
    hardClearFigure();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLine2D(line, t0, t1, properties::Color(12, 244, 244));
    drawLine2DBetweenXValues(line_h, -3.0, 3.0, properties::Color(12, 15, 244));
    drawLine2DBetweenYValues(line_h, -2.0, 2.0, properties::Color(12, 15, 244));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}

void testDrawLineBetweenPoints3D()
{
    using tp = double;

    Point3D<tp> p0(0.1, 0.2, 0.2);
    Point3D<tp> p1(2.0, 1.0, 1.0);

    Vector<double> x = {p0.x, p1.x};
    Vector<double> y = {p0.y, p1.y};
    Vector<double> z = {p0.z, p1.z};
    
    setCurrentElement("view_00");
    hardClearFigure();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawLineBetweenPoints(p0, p1, properties::Color(12, 244, 0));
    scatter3(x, y, z, properties::Color(0, 0, 0), properties::PointSize(3));
}
