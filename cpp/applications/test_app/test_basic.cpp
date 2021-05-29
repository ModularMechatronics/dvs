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

    const size_t num_rows = 3, num_cols = 3;
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
        y(k) = 2 * std::sin(x(k));
    }

    setCurrentElement("view_00");
    sleepMS(20);
    view(22.4f, 0.14f);
    sleepMS(20);
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    sleepMS(20);
    scatter(x, y, properties::Color(12, 14, 55));
}