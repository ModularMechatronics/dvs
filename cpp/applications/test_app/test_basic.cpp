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
    setCurrentElement("view_00");

    const size_t num_rows = 4, num_cols = 5;
    Matrix<double> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);

    for(size_t r = 0; r < num_rows; r++)
    {
        for(size_t c = 0; c < num_cols; c++)
        {
            x(r, c) = c;
            y(r, c) = r;
            z(r, c) = 2.0 * std::sin(x(r * c));
        }
    }

    surf(x, y, z, properties::Alpha(137),
                  properties::Name("SimpleName"),
                  properties::LineStyle("-*"));
}

void testScatter()
{
    Vector<double> x(20), y(20);

    for(size_t k = 0; k < 20; k++)
    {
        x(k) = k;
        y(k) = 2 * std::sin(x(k)) + k;
    }

    setCurrentElement("view_00");
    sleepMS(20);
    view(22.4f, 0.14f);
    sleepMS(20);
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    sleepMS(20);
    scatter(x, y, properties::Color(12, 14, 55));
}

void testScatter3()
{
    const size_t num_elements = 50;
    Vector<float> x(num_elements), y(num_elements), z(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        x(k) = 3.0 * cos(t);
        y(k) = 3.0 * sin(t);
        z(k) = t;
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    sleepMS(20);
    view(22.4f, 0.14f);
    sleepMS(20);
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    sleepMS(20);
    scatter3(x, y, z, properties::Color(12, 14, 55), properties::PointSize(3));
}

void testPlot3()
{
    const size_t num_elements = 50;
    Vector<uint32_t> x(num_elements), y(num_elements), z(num_elements);

    double t = 0.0;

    for(size_t k = 0; k < num_elements; k++)
    {
        x(k) = std::abs(10.0 * cos(t));
        y(k) = std::abs(10.0 * sin(t));
        z(k) = k;
        t = t + 0.3;
    }

    setCurrentElement("view_00");
    sleepMS(20);
    view(22.4f, 0.14f);
    sleepMS(20);
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    sleepMS(20);
    plot3(x, y, z, properties::Color(12, 14, 55), properties::LineWidth(1));
}

void testImShow()
{
    setCurrentElement("view_00");

    const uint32_t num_rows = 8, num_cols = 10;
    Matrix<uint32_t> img(num_rows, num_cols);
    const uint32_t max_val = num_rows * num_cols;

    for(uint32_t r = 0; r < num_rows; r++)
    {
        for(uint32_t c = 0; c < num_cols; c++)
        {
            img(r, c) = (r + c) * 300;
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
