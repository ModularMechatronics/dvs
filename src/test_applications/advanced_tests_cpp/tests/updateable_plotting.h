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

void testSurfBasic()
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

        surf(x_mat, y_mat, z_mat, properties::ColorMap::Jet(), properties::UPDATABLE, properties::SLOT0);
        t += 0.1;

        azimuth = azimuth > 180.0f ? -180.0f : azimuth + 1.0f;
        axis({-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0});
        view(azimuth, 10);

        usleep(10000);
    }
}

}  // namespace updateable_plotting

#endif  // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_SCROLLING_PLOT_H_
