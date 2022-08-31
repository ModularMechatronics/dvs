#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCATTER2_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCATTER2_H_

#include "dvs/dvs.h"

using namespace dvs;

namespace scatter2
{

void testWithVectorView()
{
    const size_t num_elements = 100;
    const Vector<double> x = linspaceFromBoundariesAndCount<float>(0.0f, 5.0f, num_elements);
    const Vector<double> y = dvs::sin(x);

    const VectorView<double> xv = x.view();
    const VectorView<double> yv = y.view();

    setCurrentElement("view_00");
    clearView();
    plot(x, y, properties::Color::Blue());
    scatter(xv, yv, properties::Color::Red());
}

}

#endif // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCATTER2_H_
