#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_DYNAMIC_PLOTTING_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_DYNAMIC_PLOTTING_H_

#include "dvs/dvs.h"

#include <unistd.h>

using namespace dvs;

namespace dynamic_plotting
{

void test2DFunctionExpandinAxes()
{
    const size_t num_elements = 100;

    setCurrentElement("view_00");
    clearView();

    double t = 0.0;

    for(size_t k = 0; k < 100; k++)
    {
        const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0 + t, 5.0 + t, num_elements);
        const Vector<double> y = dvs::sin(x) + t;

        t += 0.1;

        plot(x, y, properties::Color::Blue());
        usleep(1000);
    }

}

void test3DFunctionExpandinAxes()
{
    const size_t num_elements = 100;

    setCurrentElement("view_00");
    clearView();

    double t = 0.0;

    for(size_t k = 0; k < 100; k++)
    {
        const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0 + t, 5.0 + t, num_elements);
        const Vector<double> y = dvs::sin(x) + t;
        const Vector<double> z = dvs::cos(x) + t * 2.0;

        t += 0.1;

        plot3(x, y, z, properties::Color::Blue());
        usleep(1000);
    }

}

}

#endif // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_DYNAMIC_PLOTTING_H_
