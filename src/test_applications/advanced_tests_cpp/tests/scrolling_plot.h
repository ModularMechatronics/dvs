#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCROLLING_PLOT_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCROLLING_PLOT_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace scrolling_plot
{

void testBasic()
{
    const size_t num_its = 1000;

    float t = 0.0f;
    float dt = 0.01f;

    setCurrentElement("view_00");
    clearView();
    view(180, -90);
    axis({-1.0, -1.5f, -1.0}, {20.0, 1.5f, 1.0f});

    for (size_t k = 0; k < num_its; k++)
    {
        const float y = std::sin(t) + 0.1f * std::sin(t * 10.0f);
        t += dt;
        realTimePlot(dt, y, properties::SLOT0);
        usleep(10000);
    }
}

}  // namespace scrolling_plot

#endif  // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCROLLING_PLOT_H_
