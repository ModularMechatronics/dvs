#ifndef SYSTEM_TEST_BASIC_C_TESTS_HPP
#define SYSTEM_TEST_BASIC_C_TESTS_HPP

#include "basic_c/tests.h"
#include "utils.h"

namespace basic_c
{
void addTests()
{
    addTest("c", "basic", "plot", testPlot);
    addTest("c", "basic", "plot3", testPlot3);
    addTest("c", "basic", "scatter", testScatter);
    addTest("c", "basic", "scatter3", testScatter3);
    addTest("c", "basic", "surf", testSurf);
    addTest("c", "basic", "imShow", testImShow);
}
}  // namespace basic_c

#endif  // SYSTEM_TEST_BASIC_C_TESTS_HPP
