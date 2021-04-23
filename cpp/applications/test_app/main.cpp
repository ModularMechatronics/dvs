#include <algorithm>

#include "dvs.h"

using namespace dvs;

int main( void )
{
    Vector<double> x(20), y(20);

    for(size_t k = 0; k < 20; k++)
    {
        x(k) = k;
        y(k) = 2 * x(k);
    }

    setCurrentElement("view_small00");
    usleep(20000);
    view(22.4f, 0.14f);
    usleep(20000);
    axis({-1.1, -2.2, -3.3}, {4.4, 5.5, 6.6});
    usleep(20000);
    plot(x, y, properties::Color(12, 14, 55),
               properties::Alpha(137),
               properties::Name("SimpleName"),
               properties::LineStyle("-*"),
               properties::LineWidth(22),
               properties::EdgeColor(55, 21, 7),
               properties::FaceColor(52, 26, 3),
               properties::ColorMap::MAGMA(),
               properties::PointSize(137));
}
