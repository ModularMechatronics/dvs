#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_DYNAMIC_PLOTTING_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_DYNAMIC_PLOTTING_H_

#include <unistd.h>

#include "dvs/dvs.h"

using namespace dvs;

namespace dynamic_plotting
{

void test2DFunctionExpandingPlotObject()
{
    const size_t num_elements = 100;

    setCurrentElement("p_view_0");
    clearView();

    double t = 0.0;

    for (size_t k = 0; k < 100; k++)
    {
        const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0 + t, 5.0 + t, num_elements);
        const Vector<double> y = dvs::sin(x) + t;

        t += 0.1;

        plot(x, y, properties::Color::Blue());
        usleep(1000);
    }
}

void test3DFunctionExpandingPlotObject()
{
    const size_t num_elements = 100;

    setCurrentElement("p_view_0");
    clearView();

    double t = 0.0;

    for (size_t k = 0; k < 100; k++)
    {
        const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0 + t, 5.0 + t, num_elements);
        const Vector<double> y = dvs::sin(x) + t;
        const Vector<double> z = dvs::cos(x) + t * 2.0;

        t += 0.1;

        plot3(x, y, z, properties::Color::Blue());
        usleep(1000);
    }
}

void test2DFunctionRotatingView()
{
    const size_t num_elements = 100;

    setCurrentElement("p_view_0");
    clearView();

    const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0, 5.0, num_elements);
    const Vector<double> y = dvs::sin(x);
    plot(x, y, properties::Color::Blue(), properties::LineWidth(14));

    for (size_t k = 0; k < 100; k++)
    {
        view(30 + k, 180);
        usleep(10000);
    }
}

void test2DFunctionChangingAxes()
{
    const size_t num_elements = 100;

    setCurrentElement("p_view_0");
    clearView();

    const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0, 5.0, num_elements);
    const Vector<double> y = dvs::sin(x);
    plot(x, y, properties::Color::Blue(), properties::LineWidth(14));

    const Vec2d min_vec(dvs::min(x), dvs::min(y));
    const Vec2d max_vec(dvs::max(x), dvs::max(y));

    for (size_t k = 0; k < 100; k++)
    {
        axis({min_vec.x, min_vec.y}, {max_vec.x + static_cast<double>(k) / 10.0, max_vec.y});
        usleep(10000);
    }
}

void test2DFunctionNewDataAndClear()
{
    const size_t num_elements = 100;

    setCurrentElement("p_view_0");
    clearView();

    double t = 0.0;
    const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0, 5.0, num_elements);
    const double min_x = dvs::min(x);
    const double max_x = dvs::max(x);

    for (size_t k = 0; k < 100; k++)
    {
        const Vector<double> y = dvs::sin(x + t);

        t += 0.1;
        const Vec2d min_vec(min_x, dvs::min(y));
        const Vec2d max_vec(max_x, dvs::max(y));

        plot(x, y, properties::Color::Blue(), properties::LineWidth(14));

        axis({min_vec.x, -1.0}, {max_vec.x, 1.0});
        softClearView();

        usleep(10000);
    }
}

void test3DFunctionNewDataAndClear()
{
    const size_t num_elements = 100;

    const auto mat_xy = dvs::meshGrid<double>(-1.0, 1.0, -1.0, 1.0, 100, 100);
    const Matrix<double>& x_mat = mat_xy.first;
    const Matrix<double>& y_mat = mat_xy.second;
    Matrix<double> z_mat{100, 100};

    setCurrentElement("p_view_0");
    clearView();

    double t = 0.0;
    const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0, 5.0, num_elements);
    view(10, 10);
    float azimuth = 10.0;

    for (size_t k = 0; k < 1000; k++)
    {
        const Vector<double> y = dvs::sin(x + t);
        const Vector<double> z = dvs::cos(x + t);

        for (size_t r = 0; r < x_mat.numRows(); r++)
        {
            for (size_t c = 0; c < x_mat.numCols(); c++)
            {
                const double x_val = x_mat(r, c);
                const double y_val = y_mat(r, c);
                const double r_val = std::sqrt(x_val * x_val + y_val * y_val);

                const double phi = std::sin(t * 0.2) * 50.0;
                z_mat(r, c) = std::sin(r_val * phi) / (r_val * phi);
            }
        }

        surf(x_mat, y_mat, z_mat, properties::ColorMap::Jet());
        t += 0.1;

        azimuth = azimuth > 180.0f ? -180.0f : azimuth + 1.0f;
        axis({-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0});
        view(azimuth, 10);

        usleep(10000);
        softClearView();
    }
}

void test3DFunctionManualInteraction()
{
    const size_t num_elements = 100;

    const auto mat_xy = dvs::meshGrid<double>(-1.0, 1.0, -1.0, 1.0, 100, 100);
    const Matrix<double>& x_mat = mat_xy.first;
    const Matrix<double>& y_mat = mat_xy.second;
    Matrix<double> z_mat{100, 100};

    setCurrentElement("p_view_0");
    clearView();
    disableAutomaticAxesSetting();

    double t = 0.0;
    const Vector<double> x = linspaceFromBoundariesAndCount<double>(0.0, 5.0, num_elements);
    view(10, 10);
    float azimuth = 10.0;

    for (size_t k = 0; k < 1000; k++)
    {
        const Vector<double> y = dvs::sin(x + t);
        const Vector<double> z = dvs::cos(x + t);

        for (size_t r = 0; r < x_mat.numRows(); r++)
        {
            for (size_t c = 0; c < x_mat.numCols(); c++)
            {
                const double x_val = x_mat(r, c);
                const double y_val = y_mat(r, c);
                const double r_val = std::sqrt(x_val * x_val + y_val * y_val);

                const double phi = std::sin(t * 0.2) * 50.0;
                z_mat(r, c) = std::sin(r_val * phi) / (r_val * phi);
            }
        }

        surf(x_mat, y_mat, z_mat, properties::ColorMap::Jet());
        t += 0.1;

        azimuth = azimuth > 180.0f ? -180.0f : azimuth + 1.0f;

        usleep(10000);
        softClearView();
    }
}

}  // namespace dynamic_plotting

#endif  // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_DYNAMIC_PLOTTING_H_
