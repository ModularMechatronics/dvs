#include <algorithm>

#include "dvs.h"

using namespace dvs;

void sleepMS(const int ms)
{
    // usleep(ms * 1000);
}

void plotSomething(int i);

int main( void )
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
    setCurrentElement("view_10");
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
