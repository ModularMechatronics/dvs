#ifndef TEST_APPLICATIONS_BASIC_TESTS_CPP_TESTS_H_
#define TEST_APPLICATIONS_BASIC_TESTS_CPP_TESTS_H_

#include <algorithm>

#include "dvs/dvs.h"

using namespace dvs;

RGB888 colorMapJet(const float value)
{
    float _val = value;
    if (_val < 0.0)
    {
        _val = 0.0;
    }
    else if (_val >= 1.0)
    {
        _val = 0.99999;
    }

    const float full_range_value = _val * 6.0;
    const float integer_part = std::floor(full_range_value);
    const float fraction_part = full_range_value - integer_part;

    float r = 0.0, g = 0.0, b = 0.0;

    switch (static_cast<int>(integer_part))
    {
        case 0:
            r = 0.07450980392156863 + fraction_part * 0.11372549019607843;
            g = fraction_part * 0.047058823529411764;
            b = 0.51 + fraction_part * 0.48235294117647065;
            break;
        case 1:
            r = 0.18823529411764706 + fraction_part * 0.0784313725490196;
            g = 0.047058823529411764 + fraction_part * 0.8509803921568628;
            b = 0.9923529411764707 - fraction_part * 0.03137254901960784;
            break;
        case 2:
            r = 0.26666666666666666 + fraction_part * 0.3137254901960785;
            g = 0.8980392156862745 + fraction_part * 0.10196078431372546;
            b = 0.9609803921568628 - fraction_part * 0.5372549019607844;
            break;
        case 3:
            r = 0.5803921568627451 + fraction_part * 0.3999999999999999;
            g = 1.0 - fraction_part * 0.196078431372549;
            b = 0.4237254901960784 - fraction_part * 0.2901960784313725;
            break;
        case 4:
            r = 0.9803921568627451 - fraction_part * 0.0117647058823529;
            g = 0.803921568627451 - fraction_part * 0.611764705882353;
            b = 0.13352941176470587 - fraction_part * 0.05098039215686273;
            break;
        case 5:
            r = 0.9686274509803922 - fraction_part * 0.47058823529411764;
            g = 0.19215686274509805 - fraction_part * 0.19215686274509805;
            b = 0.08254901960784314 - fraction_part * 0.06274509803921569;
            break;
    }

    return RGB888(static_cast<uint8_t>(r * 255.0f), static_cast<uint8_t>(g * 255.0f), static_cast<uint8_t>(b * 255.0f));
}

void testOpenProjectFile()
{
    const std::string base_path = "/Users/danielpi/work/dvs/project_files/";
    const std::vector<std::string> project_files = {
        "ad_dataset.dvs", "car.dvs", "exp0.dvs", "exp1.dvs", "exp2.dvs", "imu.dvs", "legend.dvs", "particles.dvs"};

    for (const auto& pf : project_files)
    {
        openProjectFile(base_path + pf);
        usleep(1000 * 100);
    }

    openProjectFile(base_path + "exp0.dvs");
}

void testSurf()
{
    const int num_rows = 200, num_cols = 250;
    Matrix<double> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);
    Matrix<RGB888> color(num_rows, num_cols);

    const double inc = 0.4;
    const float nr = (num_rows - 100U);
    const float nc = (num_cols - 100U);
    const float mul = 1.0f / (std::sqrt(nr * nr + nc * nc));

    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++)
        {
            const double rd = static_cast<double>(r - 5) * inc;
            const double cd = static_cast<double>(c - 5) * inc * 2;
            x(r, c) = c;
            y(r, c) = r;
            const float r_val = std::sqrt(rd * rd + cd * cd);
            z(r, c) = 50.0 * std::sin(r_val * 0.05);

            color(r, c) = colorMapJet(r_val * mul);
        }
    }

    setCurrentElement("p_view_0");
    clearView();
    surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::FaceColor(255, 0, 0));

    setCurrentElement("p_view_1");
    clearView();
    surf(x, y, z, properties::EdgeColor::NONE, properties::ColorMap::JET);

    setCurrentElement("p_view_2");
    clearView();
    surf(x, y, z, properties::EdgeColor::NONE, properties::ColorMap::JET, properties::INTERPOLATE_COLORMAP);

    setCurrentElement("w1_p_view_0");
    clearView();
    surf(x, y, z, color, properties::EdgeColor::NONE, properties::ColorMap::JET);

    setCurrentElement("p1");
    clearView();
    surf(x, y, z);
    surf(x, y, z + 1.0);
    surf(x, y, z + 2.0);
    surf(x, y, z + 3.0);
    surf(x, y, z + 4.0);
}

void testScatter()
{
    Vector<double> x(20), y(20);
    Vector<float> xf(20), yf(20);

    double t = 0.0;

    for (size_t k = 0; k < 20; k++)
    {
        xf(k) = k;
        yf(k) = 5.0 * std::sin(3.0 * t);

        x(k) = xf(k);
        y(k) = yf(k);

        t = t + 0.1;
    }

    setCurrentElement("p_view_0");
    clearView();
    axis({0.0, -8.0, -3.3}, {32.0, 32.0, 6.6});

    plot(xf, yf, properties::Color(212, 14, 55));
    scatter(x, y, properties::Color(12, 14, 55));
    scatter(x, y + 1.0, properties::Color::BLACK, properties::PointSize(11));
    scatter(x, y + 2.0, properties::Color::CYAN, properties::PointSize(12), properties::ScatterStyle::SQUARE);
    scatter(x, y + 3.0, properties::Color::MAGENTA, properties::PointSize(13), properties::ScatterStyle::DISC);
    scatter(x, y + 4.0, properties::Color::BLUE, properties::PointSize(14), properties::ScatterStyle::PLUS);
    scatter(x, y + 5.0, properties::Color::RED, properties::PointSize(14), properties::ScatterStyle::CROSS);

    const size_t new_num_elements = 500;

    x.resize(new_num_elements);
    y.resize(new_num_elements);

    for (size_t k = 0; k < new_num_elements; k++)
    {
        const float r = static_cast<float>(rand() % 1001) / 1000.0f;
        const float phi = 2.0f * M_PI * static_cast<float>(rand() % 1001) / 1000.0f;

        const float xf = r * std::cos(phi);
        const float yf = r * std::sin(phi);

        // const float xf = 2.0f * static_cast<float>(rand() % 1001) / 1000.0f - 1.0f;
        // const float yf = 2.0f * static_cast<float>(rand() % 1001) / 1000.0f - 1.0f;

        x(k) = xf;
        y(k) = yf;
    }

    setCurrentElement("p_view_1");
    clearView();

    axis({-1.0, -1.0}, {1.0, 1.0});

    scatter(x,
            y,
            properties::DistanceFrom::xy({0.0, 0.0}, 0.0, 1.0),
            properties::ColorMap::RAINBOW,
            properties::PointSize(13),
            properties::ScatterStyle::CIRCLE);

    setCurrentElement("p1");
    clearView();
    axis({-4.0, -8.0, -3.3}, {8.0, 8.0, 6.6});
    scatter(x, y);
    scatter(x, y + 1.0, properties::PointSize(11));
    scatter(x, y + 2.0, properties::PointSize(12), properties::ScatterStyle::SQUARE);
    scatter(x, y + 3.0, properties::PointSize(13), properties::ScatterStyle::DISC);
    scatter(x, y + 4.0, properties::PointSize(14), properties::ScatterStyle::PLUS);
    scatter(x, y + 5.0, properties::PointSize(14), properties::ScatterStyle::CROSS);
}

void testScatter3()
{
    const size_t num_elements = 14600;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);

    double t0 = 0.0f, t1 = 0.0f, t2 = 0.0f, y0 = 0.0f, z0 = 0.0f;

    for (size_t k = 0; k < num_elements; k++)
    {
        const float xf = 7.0 * cos(t0) + 20.0f;
        const float yf = 7.0 * sin(t0) - 110.0f;
        const float zf = 0.0f;

        const Vec3f v0 = rotationMatrixY<float>(t1) * Vec3f(xf, yf, zf);
        const Vec3f v1 = rotationMatrixZ<float>(t2) * (v0 + Vec3f(50.0f, y0, 0.0f));

        x(k) = v1.x;
        y(k) = v1.y;
        z(k) = v1.z + z0 - 70;
        t0 = t0 + 0.3;
        t1 = t1 - 0.01;
        t2 = t2 + 0.002;
        y0 = y0 + 0.05;
        z0 = z0 + 0.02;
    }

    setCurrentElement("p_view_0");
    clearView();

    plot3(x, y, z, properties::Color(255, 14, 255), properties::LineWidth(1));
    scatter3(x, y, z, properties::Color(12, 14, 55));
    scatter3(x, y, z + 1.0, properties::Color::BLACK, properties::PointSize(11));
    scatter3(x, y, z + 2.0, properties::Color::CYAN, properties::PointSize(12), properties::ScatterStyle::SQUARE);
    scatter3(x, y, z + 3.0, properties::Color::MAGENTA, properties::PointSize(13), properties::ScatterStyle::DISC);
    scatter3(x, y, z + 4.0, properties::Color::BLUE, properties::PointSize(14), properties::ScatterStyle::PLUS);
    scatter3(x, y, z + 5.0, properties::Color::RED, properties::PointSize(14), properties::ScatterStyle::CROSS);

    const size_t new_num_elements = 500;

    x.resize(new_num_elements);
    y.resize(new_num_elements);
    z.resize(new_num_elements);

    for (size_t k = 0; k < new_num_elements; k++)
    {
        const float r = static_cast<float>(rand() % 1001) / 1000.0f;
        const float theta = M_PI * static_cast<float>(rand() % 1001) / 1000.0f;
        const float phi = 2.0f * M_PI * static_cast<float>(rand() % 1001) / 1000.0f;

        const float xf = r * std::sin(theta) * std::cos(phi);
        const float yf = r * std::sin(theta) * std::sin(phi);
        const float zf = r * std::cos(theta);

        // const float xf = 2.0f * static_cast<float>(rand() % 1001) / 1000.0f - 1.0f;
        // const float yf = 2.0f * static_cast<float>(rand() % 1001) / 1000.0f - 1.0f;
        // const float zf = 2.0f * static_cast<float>(rand() % 1001) / 1000.0f - 1.0f;

        x(k) = (xf < 0) ? -xf : xf;
        y(k) = yf;
        z(k) = zf;
    }

    setCurrentElement("p_view_1");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    scatter3(x,
             y,
             z,
             properties::DistanceFrom::x(0.0, 0.0, 1.0),
             properties::ColorMap::JET,
             properties::PointSize(13),
             properties::ScatterStyle::CIRCLE);
}

void testPlotCollection()
{
    const size_t num_elements = 40;
    Vector<double> x0(num_elements), y0(num_elements), x1(num_elements), y1(num_elements), x2(num_elements),
        y2(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        const double kd = k;
        x0(k) = 10.0 * cos(t);
        y0(k) = kd * 0.05 * 10.0 * sin(t) + 0.2;

        x1(k) = 14.0 * cos(t);
        y1(k) = kd * 0.07 * 11.0 * sin(t) + 0.3;

        x2(k) = 9.0 * cos(t);
        y2(k) = kd * 0.08 * 15.0 * sin(t) + 0.4;
        t = t + 0.1;
    }

    std::vector<VectorConstView<double>> pc_x = {x0.constView(), x1.constView(), x2.constView()};
    std::vector<VectorConstView<double>> pc_y = {y0.constView(), y1.constView(), y2.constView()};

    setCurrentElement("p_view_0");
    clearView();

    plotCollection(pc_x, pc_y, properties::Color(0, 0, 0));
    scatter(x0, y0, properties::Color(212, 14, 55), properties::PointSize(10));
    scatter(x1, y1, properties::Color(0, 14, 55), properties::PointSize(10));
    scatter(x2, y2, properties::Color(0, 255, 55), properties::PointSize(10));

    std::vector<Vector<double>> pcm_x;
    std::vector<Vector<double>> pcm_y;

    auto rand_num = [](void) -> double { return static_cast<double>(rand() % 1001) / 1000.0; };

    std::vector<size_t> np = {50, 50, 50, 50, 56};

    for (size_t k = 0; k < 30; k++)
    {
        const size_t num_elems = (rand() % 20) + 50;
        Vector<double> x(num_elems), y(num_elems);

        t = 0.0;

        const double t_inc = rand_num() / 100.0 + 0.05;
        const double x_a = rand_num() * 3.0;
        const double y_a = rand_num() * 3.0;
        const double x_o = rand_num() * 10.0;
        const double y_o = rand_num() * 10.0;

        for (size_t i = 0; i < num_elems; i++)
        {
            x(i) = x_a * cos(t) + x_o;
            y(i) = y_a * t * sin(t) + y_o;
            t += t_inc;
        }
        pcm_x.push_back(x);
        pcm_y.push_back(y);
    }

    setCurrentElement("p_view_1");
    clearView();

    for (size_t k = 0; k < pcm_x.size(); k++)
    {
        scatter(pcm_x[k], pcm_y[k], properties::Color(255, 0, 0), properties::PointSize(10));
    }

    plotCollection(pcm_x, pcm_y, properties::Color(0, 0, 0));
}

void testPlotCollection3()
{
    const size_t num_elements = 40;
    Vector<double> x0(num_elements), y0(num_elements), z0(num_elements), x1(num_elements), y1(num_elements),
        z1(num_elements), x2(num_elements), y2(num_elements), z2(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        const double kd = k;
        x0(k) = 10.0 * cos(t);
        y0(k) = kd * 0.05 * 10.0 * sin(t) + 0.2;
        z0(k) = kd * 0.05 * 10.0 * cos(t) * sin(t) + 0.3;

        x1(k) = 14.0 * cos(t);
        y1(k) = kd * 0.07 * 11.0 * sin(t) + 0.3;
        z1(k) = kd * 0.07 * 11.0 * sin(t) * cos(t) + 0.3;

        x2(k) = 9.0 * cos(t);
        y2(k) = kd * 0.08 * 15.0 * sin(t) + 0.4;
        z2(k) = kd * 0.08 * 15.0 * sin(t) * cos(t) + 0.4;
        t = t + 0.1;
    }

    std::vector<VectorConstView<double>> pc_x = {x0.constView(), x1.constView(), x2.constView()};
    std::vector<VectorConstView<double>> pc_y = {y0.constView(), y1.constView(), y2.constView()};
    std::vector<VectorConstView<double>> pc_z = {z0.constView(), z1.constView(), z2.constView()};

    setCurrentElement("p_view_0");
    clearView();

    plotCollection3(pc_x, pc_y, pc_z, properties::Color(0, 0, 0));
    scatter3(x0, y0, z0, properties::Color(212, 14, 55), properties::PointSize(10));
    scatter3(x1, y1, z1, properties::Color(0, 14, 55), properties::PointSize(10));
    scatter3(x2, y2, z2, properties::Color(0, 255, 55), properties::PointSize(10));

    std::vector<Vector<double>> pcm_x;
    std::vector<Vector<double>> pcm_y;
    std::vector<Vector<double>> pcm_z;

    auto rand_num = [](void) -> double { return static_cast<double>(rand() % 1001) / 1000.0; };

    std::vector<size_t> np = {50, 50, 50, 50, 56};

    for (size_t k = 0; k < 30; k++)
    {
        const size_t num_elems = (rand() % 20) + 50;
        Vector<double> x(num_elems), y(num_elems), z(num_elems);

        t = 0.0;

        const double t_inc = rand_num() / 100.0 + 0.05;
        const double x_a = rand_num() * 3.0;
        const double y_a = rand_num() * 3.0;
        const double z_a = rand_num() * 3.0;
        const double x_o = rand_num() * 10.0;
        const double y_o = rand_num() * 10.0;
        const double z_o = rand_num() * 10.0;

        for (size_t i = 0; i < num_elems; i++)
        {
            x(i) = x_a * cos(t) + x_o;
            y(i) = y_a * t * sin(t) + y_o;
            z(i) = z_a * t * sin(t) * cos(t) + z_o;
            t += t_inc;
        }
        pcm_x.push_back(std::move(x));
        pcm_y.push_back(std::move(y));
        pcm_z.push_back(std::move(z));
    }

    setCurrentElement("p_view_1");
    clearView();

    for (size_t k = 0; k < pcm_x.size(); k++)
    {
        scatter3(pcm_x[k], pcm_y[k], pcm_z[k], properties::Color(255, 0, 0), properties::PointSize(10));
    }

    plotCollection3(pcm_x, pcm_y, pcm_z, properties::Color(0, 0, 0));
}

void testPlot()
{
    const size_t num_elements = 30;
    Vector<float> x(num_elements), y(num_elements), z(num_elements);

    const size_t num_points = 7;
    Vector<float> xp(num_points), yp(num_points), zp(num_points);
    Vector<RGB888> colorp(num_points);

    xp(0) = 0.0;
    xp(1) = 1.0;
    xp(2) = 2.2;
    xp(3) = 3.0;
    xp(4) = 3.5;
    xp(5) = 3.5;
    xp(6) = 4.5;

    yp(0) = 0.0;
    yp(1) = 3.5;
    yp(2) = 3.5;
    yp(3) = 2.5;
    yp(4) = 4.0;
    yp(5) = 2.0;
    yp(6) = 2.0;

    colorp(0) = RGB888{255, 0, 0};
    colorp(1) = RGB888{255, 255, 0};
    colorp(2) = RGB888{255, 0, 255};
    colorp(3) = RGB888{0, 255, 0};
    colorp(4) = RGB888{0, 255, 255};
    colorp(5) = RGB888{0, 0, 0};
    colorp(6) = RGB888{0, 0, 255};

    zp.fill(0.01f);

    setCurrentElement("p_view_0");
    clearView();

    axis({-1.0, -1.0, -1.0}, {5.0, 5.0, 1.0});
    plot(xp, yp, colorp, properties::LineWidth(20), properties::Color(200, 200, 200));
    scatter3(xp, yp, zp, properties::PointSize(10), properties::Color(255, 0, 0));

    zp.fill(-0.01f);
    plot3(xp, yp, zp, properties::LineWidth(60), properties::Color(0, 255, 0));

    view(0, 90);

    setCurrentElement("p1");
    clearView();

    xp(0) = 0.0;
    xp(1) = 0.0;
    xp(2) = 1.0;
    xp(3) = 1.0;
    xp(4) = 3.5;
    xp(5) = 3.5;

    yp(0) = 0.0;
    yp(1) = 3.0;
    yp(2) = 3.0;
    yp(3) = 0.0;
    yp(4) = 4.0;
    yp(5) = 2.0;

    axis({-0.5, 2.5, -1.0}, {0.5, 3.5, 1.0});
    plot(xp, yp, properties::LineWidth(50), properties::Color(200, 200, 200));
    view(0, 90);

    setCurrentElement("p_view_1");
    float t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        x(k) = 10.0 * cos(t) + 20.0;
        y(k) = 10.0 * sin(t) + 20.0 + k;
        z(k) = 0.01f;

        t = t + 0.3;
    }

    axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    plot(x + 3.0f, y, properties::Color(0, 255, 255), properties::LineWidth(1));
    plot(x + 4.0f, y, properties::Color(212, 14, 55), properties::LineWidth(1));
    plot(x + 5.0f, y, properties::Color(212, 255, 55), properties::LineWidth(4));
    plot(x + 6.0f, y, properties::Color(212, 14, 255), properties::LineWidth(7));
    plot(x + 6.0f, y, properties::Color(212, 14, 255), properties::LineWidth(7), properties::PERSISTENT);
    scatter3(x + 3.0f, y, z, properties::Color::BLACK, properties::PointSize(14));

    setCurrentElement("p_view_2");
    clearView();
    t = 0.0;
    x.resize(num_elements * 10);
    y.resize(num_elements * 10);
    z.resize(num_elements * 10);
    Vector<RGB888> color(num_elements * 10);

    for (size_t k = 0; k < (num_elements * 10); k++)
    {
        x(k) = 10.0f * cos(t) + 20.0f;
        y(k) = 10.0f * sin(t) + 20.0f + static_cast<float>(k) / 10.0f;
        z(k) = 0.01f;

        color(k) = colorMapJet(static_cast<float>(k) / static_cast<float>(num_elements * 10U - 1U));

        t = t + 0.03;
    }

    axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    plot(x + 3.0f, y, properties::LineWidth(3));
    plot(x + 4.0f, y, properties::LineWidth(3));
    plot(x + 5.0f, y, properties::LineWidth(3));
    plot(x + 6.0f, y, color, properties::Color(212, 14, 255), properties::LineWidth(7));

    setCurrentElement("w1_p_view_0");
    clearView();

    axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    plot(x, y, properties::LineWidth(7));
    plot(x + 1.0f, y, properties::LineWidth(7));
    plot(x + 2.0f, y, properties::LineWidth(7));
    plot(x + 3.0f, y, properties::LineWidth(7));
    plot(x + 4.0f, y, properties::LineWidth(7));
    plot(x + 5.0f, y, properties::LineWidth(7));
    plot(x + 6.0f, y, properties::LineWidth(7));
}

void testFastPlot()
{
    const size_t num_elements = 30;
    Vector<float> x(num_elements), y(num_elements);

    setCurrentElement("p_view_0");
    clearView();

    float t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        x(k) = 10.0 * cos(t) + 20.0;
        y(k) = 10.0 * sin(t) + 20.0 + k;

        t = t + 0.3;
    }

    axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    fastPlot(x + 3.0f, y, properties::Color(0, 255, 255));
    fastPlot(x + 4.0f, y, properties::Color(212, 14, 55));
    fastPlot(x + 5.0f, y, properties::Color(212, 255, 55));
    fastPlot(x + 6.0f, y, properties::Color(212, 14, 255));

    setCurrentElement("p_view_2");
    clearView();
    t = 0.0;
    x.resize(num_elements * 10);
    y.resize(num_elements * 10);

    for (size_t k = 0; k < (num_elements * 10); k++)
    {
        x(k) = 10.0f * cos(t) + 20.0f;
        y(k) = 10.0f * sin(t) + 20.0f + static_cast<float>(k) / 10.0f;

        t = t + 0.03;
    }

    axis({0.0, 16.0, -1.0}, {50.0, 64.0, 1.0});
    fastPlot(x + 3.0f, y, properties::Color(0, 255, 255));
    fastPlot(x + 4.0f, y, properties::Color(212, 14, 55));
    fastPlot(x + 5.0f, y, properties::Color(212, 255, 55));
    fastPlot(x + 6.0f, y, properties::Color(212, 14, 255));
}

void testLineCollection()
{
    const size_t num_lines = 5;
    Vector<float> x(num_lines * 2), y(num_lines * 2);

    setCurrentElement("p_view_0");
    clearView();

    float t = 0.0f;

    size_t idx = 0;

    for (size_t k = 0; k < num_lines; k++)
    {
        x(idx) = t * 2.0f;
        y(idx) = t * 2.0f;

        x(idx + 1) = t * 2.0f + cos(t);
        y(idx + 1) = t * 2.0f + sin(t);

        idx += 2;

        t = t + 0.3;
    }

    axis({0.0, 0.0, -1.0}, {10.0, 10.0, 1.0});
    lineCollection(x, y, properties::Color(255, 0, 0));
}

void testLineCollection3()
{
    const size_t num_lines = 5;
    Vector<float> x(num_lines * 2), y(num_lines * 2), z(num_lines * 2);

    setCurrentElement("p_view_0");
    clearView();

    float t = 0.0f;

    size_t idx = 0;

    for (size_t k = 0; k < num_lines; k++)
    {
        x(idx) = t * 2.0f;
        y(idx) = t * 2.0f;
        z(idx) = t * 2.0f;

        x(idx + 1) = t * 2.0f + cos(t);
        y(idx + 1) = t * 2.0f + sin(t);
        z(idx + 1) = t * 2.0f + sin(t) + cos(t);

        idx += 2;

        t = t + 0.3;
    }

    axis({0.0, 0.0, -1.0}, {10.0, 10.0, 10.0});
    lineCollection3(x, y, z, properties::Color(255, 0, 0));
}

void testStem()
{
    const size_t num_elements = 30;

    const Vector<float> x = linspaceFromBoundariesAndCount<float>(0.0f, 5.0f, num_elements);
    const Vector<float> y = dvs::sin(x);

    setCurrentElement("p_view_0");
    clearView();

    axis({0.0, -1.0, -1.0}, {5.0, 5.0, 1.0});
    stem(x, y, properties::Color::RED);
}

void testStairs()
{
    const size_t num_elements = 30;
    Vector<float> xf(num_elements), yf(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        xf(k) = t;
        yf(k) = 10.0 * sin(t) + 20.0 + k;

        t = t + 0.3;
    }

    setCurrentElement("p_view_0");
    clearView();

    stairs(xf, yf, properties::Color(21, 14, 55), properties::LineWidth(2));
    plot(xf, yf, properties::Color(21, 14, 255), properties::LineWidth(2));
    scatter(xf, yf, properties::Color(255, 0, 0), properties::PointSize(10));
}

void testPlot3()
{
    setCurrentElement("p_view_0");
    clearView();

    const size_t num_points = 6;
    Vector<float> xp(num_points), yp(num_points), zp(num_points);

    xp(0) = 0.0;
    xp(1) = 1.0;
    xp(2) = 2.2;
    xp(3) = 3.0;
    xp(4) = 3.5;
    xp(5) = 3.5;

    yp(0) = 0.0;
    yp(1) = 3.5;
    yp(2) = 3.5;
    yp(3) = 2.5;
    yp(4) = 4.0;
    yp(5) = 2.0;

    zp(0) = 0.0;
    zp(1) = 0.1;
    zp(2) = 0.2;
    zp(3) = 0.3;
    zp(4) = 0.4;
    zp(5) = 0.5;

    zp.fill(0.01f);

    axis({-1.0, -1.0, -1.0}, {5.0, 5.0, 1.0});
    plot3(xp, yp, zp, properties::LineWidth(60), properties::Color(200, 200, 200));
    scatter3(xp, yp, zp, properties::PointSize(10), properties::Color(255, 0, 0));

    const size_t num_elements = 30;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);
    Vector<float> xf(num_elements), yf(num_elements), zf(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        xf(k) = 1.0 * cos(t) + 0.0;
        yf(k) = 1.0 * sin(t) + 0.0;
        zf(k) = t;

        x(k) = xf(k);
        y(k) = yf(k);
        z(k) = t;
        t = t + 0.3;
    }

    setCurrentElement("p_view_1");
    clearView();

    plot3(x, y, z, properties::Color(212, 14, 55), properties::LineWidth(1));
    plot3(xf + 0.1f, yf, zf, properties::Color(21, 14, 55), properties::LineWidth(7));
}

void testFastPlot3()
{
    const size_t num_elements = 30;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);
    Vector<float> xf(num_elements), yf(num_elements), zf(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        xf(k) = 1.0 * cos(t) + 0.0;
        yf(k) = 1.0 * sin(t) + 0.0;
        zf(k) = t;

        x(k) = xf(k);
        y(k) = yf(k);
        z(k) = t;
        t = t + 0.3;
    }

    setCurrentElement("p_view_0");
    clearView();

    fastPlot3(x, y, z, properties::Color(212, 14, 55));
    fastPlot3(xf + 0.1f, yf, zf, properties::Color(21, 14, 55));
}

void testImShow4()
{
    const uint32_t num_rows = 800, num_cols = 800;
    ImageRGBA<float> img4(num_rows, num_cols);
    ImageRGB<float> img3(num_rows, num_cols);

    for (uint32_t r = 0; r < num_rows; r++)
    {
        for (uint32_t c = 0; c < num_cols; c++)
        {
            const float xr = 3.0f * (static_cast<float>(c) - 300.5f) / 800.0f;
            const float yr = 3.0f * (static_cast<float>(r) - 400.5f) / 800.0f;
            const float rr = std::sqrt(xr * xr + yr * yr);

            const float xg = 2.0f * (static_cast<float>(c) - 500.5f) / 800.0f;
            const float yg = 2.0f * (static_cast<float>(r) - 350.5f) / 800.0f;
            const float rg = std::sqrt(xg * xg + yg * yg);

            const float xb = 4.0f * (static_cast<float>(c) - 200.5f) / 800.0f;
            const float yb = 4.0f * (static_cast<float>(r) - 600.5f) / 800.0f;
            const float rb = std::sqrt(xb * xb + yb * yb);

            img4(r, c, 0) = (std::sin(rb) / rr + 1.0f) * 0.5f;
            img4(r, c, 1) = (std::sin(rb) / rg + 1.0f) * 0.5f;
            img4(r, c, 2) = (std::sin(rb) / rb + 1.0f) * 0.5f;
            img4(r, c, 3) = static_cast<float>(r + c) / 1600;

            img3(r, c, 0) = (std::sin(rb) / rr + 1.0f) * 0.5f;
            img3(r, c, 1) = (std::sin(rb) / rg + 1.0f) * 0.5f;
            img3(r, c, 2) = (std::sin(rb) / rb + 1.0f) * 0.5f;
        }
    }
    setCurrentElement("p_view_0");
    clearView();
    imShow(img4);

    setCurrentElement("p_view_1");
    clearView();
    imShow(img3);
}

void testImShow()
{
    const uint32_t num_rows = 800, num_cols = 800;
    ImageRGB<float> img3(num_rows, num_cols);
    ImageGray<float> img1(num_rows, num_cols);
    ImageRGBA<float> img4(num_rows, num_cols);

    ImageGray<uint8_t> img1_uint8(num_rows, num_cols);
    ImageRGB<uint8_t> img3_uint8(num_rows, num_cols);

    for (uint32_t r = 0; r < num_rows; r++)
    {
        for (uint32_t c = 0; c < num_cols; c++)
        {
            const float xr = 3.0f * (static_cast<float>(c) - 300.5f) / 800.0f;
            const float yr = 3.0f * (static_cast<float>(r) - 400.5f) / 800.0f;
            const float rr = std::sqrt(xr * xr + yr * yr);

            const float xg = 2.0f * (static_cast<float>(c) - 500.5f) / 800.0f;
            const float yg = 2.0f * (static_cast<float>(r) - 350.5f) / 800.0f;
            const float rg = std::sqrt(xg * xg + yg * yg);

            const float xb = 4.0f * (static_cast<float>(c) - 200.5f) / 800.0f;
            const float yb = 4.0f * (static_cast<float>(r) - 600.5f) / 800.0f;
            const float rb = std::sqrt(xb * xb + yb * yb);

            img3(r, c, 0) = (std::sin(rb) / rr + 1.0f) * 0.5f;
            img3(r, c, 1) = (std::sin(rb) / rg + 1.0f) * 0.5f;
            img3(r, c, 2) = (std::sin(rb) / rb + 1.0f) * 0.5f;

            img4(r, c, 0) = (std::sin(rb) / rr + 1.0f) * 0.5f;
            img4(r, c, 1) = (std::sin(rb) / rg + 1.0f) * 0.5f;
            img4(r, c, 2) = (std::sin(rb) / rb + 1.0f) * 0.5f;
            img4(r, c, 3) = static_cast<float>(r + c) / 1600;

            img1(r, c) = (std::sin(rb) / rr + 1.0f) * 0.5f;

            img1_uint8(r, c) = static_cast<uint8_t>(100.0f * (std::sin(rb) + 1.0f) / rr);

            img3_uint8(r, c, 0) = static_cast<uint8_t>(100.0f * (std::sin(rb) + 1.0f) / rr);
            img3_uint8(r, c, 1) = static_cast<uint8_t>(100.0f * (std::sin(rb) + 1.0f) / rg);
            img3_uint8(r, c, 2) = static_cast<uint8_t>(100.0f * (std::sin(rb) + 1.0f) / rb);
        }
    }
    setCurrentElement("p_view_0");
    clearView();
    setAxesBoxScaleFactor({1.0, 1.0, 1.0});
    imShow(img3, properties::Alpha(127));

    setCurrentElement("p_view_1");
    clearView();
    imShow(img1);

    setCurrentElement("p_view_2");
    clearView();
    imShow(img1_uint8);

    setCurrentElement("w1_p_view_0");
    clearView();
    imShow(img3_uint8);

    setCurrentElement("p1");
    clearView();
    imShow(img4);
}

void testAxis()
{
    setCurrentElement("p_view_0");
    clearView();

    axis({1.0, 2.5}, {7.2, 8.4});

    setCurrentElement("p_view_1");
    clearView();

    axis({-1.0, 1.5, 3.4}, {5.4, 9.2, 5.5});
}

void testLegend()
{
    const int num_rows = 20, num_cols = 25;
    Matrix<double> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);

    double inc = 0.4;
    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++)
        {
            const double rd = static_cast<double>(r - 5) * inc;
            const double cd = static_cast<double>(c - 5) * inc * 2;
            x(r, c) = c;
            y(r, c) = r;
            z(r, c) = std::sin(std::sqrt(rd * rd + cd * cd));
        }
    }

    const size_t num_elements = 30;
    Vector<float> x0(num_elements), y0(num_elements);
    Vector<float> x1(num_elements), y1(num_elements);
    Vector<float> x2(num_elements), y2(num_elements);

    double t = 0.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        x0(k) = 10.0 * cos(t);
        y0(k) = 10.0 * sin(t) + k;

        t = t + 0.3;
    }

    x1 = x0 * 2.0f;
    y1 = y0 * 2.0f;

    x2 = x0 * 3.0f;
    y2 = y0 * 3.0f;

    setCurrentElement("p_view_0");
    clearView();

    axis({-32.0, 0.0, -2.0}, {32.0, 110.0, 2.0});
    plot(x0, y0, properties::Color(212, 14, 55), properties::Name("AAAaaaBBBbbb"));
    plot(x1, y1, properties::Color(12, 255, 55), properties::Name("sig0"));
    plot(x2, y2, properties::Color(127, 14, 255), properties::Name("ej0293e2?rq430#€pqigj"));
    surf(x, y, z, properties::EdgeColor(0, 255, 0), properties::FaceColor(255, 0, 0), properties::Name("SURF"));
    surf(x,
         y,
         z + 1.0,
         properties::EdgeColor(255, 255, 255),
         properties::ColorMap::JET,
         properties::Name("SURF_COLOR_MAP"));
    scatter(x0, y0, properties::Color(255, 0, 0), properties::Name("Scatter"));
    scatter(x1,
            y1,
            properties::Color(255, 0, 255),
            properties::Name("Scatter"),
            properties::PointSize(40),
            properties::ScatterStyle::CROSS);
    showLegend();

    setCurrentElement("p_view_1");
    clearView();

    axis({-32.0, 0.0, -2.0}, {32.0, 110.0, 2.0});
    plot(x0, y0, properties::Color(212, 14, 55), properties::Name("AAAaaaBBBbbb"));
    plot(x1, y1, properties::Color(12, 255, 55), properties::Name("sig0"));
    plot(x2, y2, properties::Color(127, 14, 255), properties::Name("ej0293e2?rq430#€pqigj"));
    surf(x, y, z, properties::EdgeColor(0, 255, 0), properties::FaceColor(255, 0, 0), properties::Name("SURF"));
    surf(x, y, z + 1.0, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET, properties::Name("SURF_COLOR_MAP"));
    showLegend();

    setCurrentElement("p_view_2");
    clearView();

    surf(x, y, z, properties::EdgeColor(0, 255, 0), properties::ColorMap::JET, properties::Name("Jet"));
    surf(x + 1.0, y, z, properties::EdgeColor(0, 255, 0), properties::ColorMap::RAINBOW, properties::Name("Rainbow"));
    surf(x + 2.0, y, z, properties::EdgeColor(0, 255, 0), properties::ColorMap::MAGMA, properties::Name("Magma"));
    surf(x + 3.0, y, z, properties::EdgeColor(0, 255, 0), properties::ColorMap::VIRIDIS, properties::Name("Viridis"));
    surf(x + 4.0, y, z, properties::EdgeColor(0, 255, 0), properties::ColorMap::PASTEL, properties::Name("Pastel"));

    showLegend();
}

void testDrawMesh()
{
    using tp = double;

    Vector<Point3<tp>> vertices(12);
    vertices(0) = Point3<tp>(0.0, 0.0, 0.0);
    vertices(1) = Point3<tp>(1.0, 0.0, 0.0);
    vertices(2) = Point3<tp>(0.5, -1.0, 1.0);

    vertices(3) = Point3<tp>(0.0, 0.0, 0.0);
    vertices(4) = Point3<tp>(-1.0, 0.0, 0.0);
    vertices(5) = Point3<tp>(0.0, 1.0, 1.0);

    vertices(6) = Point3<tp>(0.0, 0.0, 0.0);
    vertices(7) = Point3<tp>(-1.0, 0.0, 0.0);
    vertices(8) = Point3<tp>(0.0, 1.0, -1.0);

    vertices(9) = Point3<tp>(0.0, 0.0, 0.0);
    vertices(10) = Point3<tp>(1.0, 0.0, 0.0);
    vertices(11) = Point3<tp>(0.5, -1.0, -1.0);

    Vector<IndexTriplet> indices(4);
    indices(0) = IndexTriplet(0, 1, 2);
    indices(1) = IndexTriplet(3, 4, 5);
    indices(2) = IndexTriplet(6, 7, 8);
    indices(3) = IndexTriplet(9, 10, 11);

    Vector<tp> x(12), y(12), z(12);

    for (size_t k = 0; k < x.size(); k++)
    {
        x(k) = vertices(k).x;
        y(k) = vertices(k).y;
        z(k) = vertices(k).z + 2.0;
    }

    setCurrentElement("p_view_0");
    clearView();

    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    drawMesh(vertices, indices, properties::EdgeColor(0, 0, 0), properties::FaceColor(12, 244, 244));
    drawMesh(x, y, z, indices, properties::EdgeColor(0, 0, 0), properties::FaceColor(255, 0, 244));

    drawMesh(x + 2.0, y, z, indices, properties::EdgeColor::NONE, properties::FaceColor(255, 0, 244));
    drawMesh(x + 2.0, y, z - 2.0, indices, properties::EdgeColor::RED, properties::FaceColor::NONE);
}

void testSetProperties()
{
    const size_t num_elements = 30;

    const Vector<float> x = linspaceFromBoundariesAndCount<float>(0.0f, 5.0f, num_elements);
    const Vector<float> y = dvs::sin(x);

    setCurrentElement("p_view_0");
    clearView();
    axis({0.0, -1.0, -1.0}, {5.0, 5.0, 1.0});

    setProperties(properties::ID0, properties::Color::CYAN, properties::LineWidth(20));
    plot(x, y, properties::ID0);

    scatter(x, y + 2.0f, properties::ID1, properties::Color::MAGENTA, properties::ScatterStyle::PLUS);

    std::cout << "Initializing plot to be CYAN and LineWidth(10), press enter to continue..." << std::endl;

    std::cin.ignore();

    setProperties(properties::ID0, properties::Color::YELLOW, properties::LineWidth(3));
    setProperties(properties::ID1, properties::Color::RED, properties::ScatterStyle::CIRCLE);
    std::cout << "Setting properties of plot to be YELLOW and LineWidth(3)" << std::endl;
}

#endif  // TEST_APPLICATIONS_BASIC_TESTS_CPP_TESTS_H_
