#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCATTER3_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCATTER3_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace scatter3_ns
{

void testWithVectorView()
{
    const size_t num_elements = 100;
    const Vector<double> x = linspaceFromBoundariesAndCount<float>(0.0f, 5.0f, num_elements);
    const Vector<double> y = dvs::sin(x);
    const Vector<double> z = dvs::cos(x);

    const VectorView<double> xv = x.view();
    const VectorView<double> yv = y.view();
    const VectorView<double> zv = z.view();

    setCurrentElement("view_00");
    clearView();
    scatter3(xv, yv, zv, properties::Color::Green());
    plot3(x, y, z, properties::Color::Magenta());
}

}

#endif // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCATTER3_H_
