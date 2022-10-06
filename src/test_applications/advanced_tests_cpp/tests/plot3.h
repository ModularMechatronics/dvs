#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_PLOT3_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_PLOT3_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace plot3_ns
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
    scatter3(x, y, z, properties::Color::Red());
    plot3(xv, yv, zv, properties::Color::Blue());
}

}  // namespace plot3_ns

#endif  // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_PLOT3_H_
