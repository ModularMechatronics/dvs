#ifndef SYSTEM_TEST_BASIC_C_TESTS_H
#define SYSTEM_TEST_BASIC_C_TESTS_H

#ifdef __cplusplus
extern "C" {
#endif

// Basic C
void testPlot();
void testPlot3();
void testScatter();
void testScatter3();
void testSurf();
void testImShow();

// GUI Test
void testGUIBasic();

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
namespace basic_c
{
inline void addTests()
{
    addTest("c", "basic", "plot", testPlot);
    addTest("c", "basic", "plot3", testPlot3);
    addTest("c", "basic", "scatter", testScatter);
    addTest("c", "basic", "scatter3", testScatter3);
    addTest("c", "basic", "surf", testSurf);
    addTest("c", "basic", "imShow", testImShow);

    addTest("c", "gui", "basic", testGUIBasic);
}
}  // namespace basic_c
#endif

#endif  // SYSTEM_TEST_BASIC_C_TESTS_H