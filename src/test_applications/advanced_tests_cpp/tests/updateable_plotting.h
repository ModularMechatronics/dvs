#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_UPDATEABLE_PLOT_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_UPDATEABLE_PLOT_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace updateable_plotting
{

void testRealTimePlotBasic()
{
    const size_t num_its = 1500;

    float t = 0.0f;
    float dt = 0.01f;

    setCurrentElement("p_view_0");
    clearView();
    view(180, -90);
    axis({-1.0, -1.5f, -1.0}, {20.0, 1.5f, 1.0f});

    setProperties(properties::SLOT0, properties::Color::Blue());

    for (size_t k = 0; k < num_its; k++)
    {
        const float y = std::sin(t) + 0.1f * std::sin(t * 10.0f);
        t += dt;

        if (k == 600)
        {
            setProperties(properties::SLOT0, properties::Color::Red());
            setProperties(properties::SLOT0, properties::BufferSize(1000U));
        }
        /*if (k < 120)
        {
            realTimePlot(dt, y, properties::SLOT0, properties::Color::Blue());
        }
        else if (k == 120)
        {
            setProperties(properties::SLOT0, properties::Color::Red());
            realTimePlot(dt, y, properties::SLOT0);
        }
        else
        {
            realTimePlot(dt, y, properties::SLOT0);
        }*/
        realTimePlot(dt, y, properties::SLOT0);
        usleep(10000);
    }
}

void testPlot2Basic()
{
    const size_t num_its = 100;
    const size_t num_elements = 100;

    float t = 0.0f;
    float dt = 0.01f;

    setCurrentElement("p_view_0");
    clearView();
    axis({-1.0, -1.5f, -1.0}, {6.0, 1.5f, 1.0f});

    setProperties(properties::SLOT0, properties::UPDATABLE);
    const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0, 5.0, num_elements);

    for (size_t k = 0; k < num_its; k++)
    {
        const Vector<double> xt = linspaceFromBoundariesAndCount<double>(0.0 + t, 5.0 + t, num_elements);
        const Vector<double> y = dvs::sin(xt);

        plot(x, y, properties::SLOT0);

        usleep(1000 * 50);

        t += dt;
    }
}

}  // namespace updateable_plotting

#endif  // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCROLLING_PLOT_H_
