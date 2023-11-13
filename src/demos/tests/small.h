#ifndef DEMOS_SMALL_H
#define DEMOS_SMALL_H

#include <complex>
#include <random>

#include "args.h"

namespace small
{

namespace
{

RGB888 calculateColormapViridis(double value)
{
    if (value < 0.0)
    {
        value = 0.0;
    }
    else if (value >= 1.0)
    {
        value = 0.99999;
    }

    double full_range_value = value * 8.0;
    double integer_part = floor(full_range_value);
    double fraction_part = full_range_value - integer_part;

    double r = 0.0, g = 0.0, b = 0.0;

    switch (static_cast<int>(integer_part))
    {
        case 0:
            r = 0.2627450980392157 + fraction_part * 0.01568627450980392;
            g = fraction_part * 0.16470588235294117;
            b = 0.32941176470588235 + fraction_part * 0.1568627450980392;
            break;
        case 1:
            r = 0.2784313725490196 - fraction_part * 0.03529411764705884;
            g = 0.16470588235294117 + fraction_part * 0.16470588235294117;
            b = 0.48627450980392156 + fraction_part * 0.05882352941176466;
            break;
        case 2:
            r = 0.24313725490196078 - fraction_part * 0.03137254901960784;
            g = 0.32941176470588235 + fraction_part * 0.1215686274509804;
            b = 0.5450980392156862 + fraction_part * 0.007843137254902044;
            break;
        case 3:
            r = 0.21176470588235294 - fraction_part * 0.01568627450980392;
            g = 0.45098039215686275 + fraction_part * 0.10980392156862745;
            b = 0.5529411764705883 - fraction_part * 0.007843137254902044;
            break;
        case 4:
            r = 0.19607843137254902 + fraction_part * 0.023529411764705882;
            g = 0.5607843137254902 + fraction_part * 0.11372549019607847;
            b = 0.5450980392156862 - fraction_part * 0.039215686274509776;
            break;
        case 5:
            r = 0.2196078431372549 + fraction_part * 0.1215686274509804;
            g = 0.6745098039215687 + fraction_part * 0.09411764705882353;
            b = 0.5058823529411764 - fraction_part * 0.0784313725490196;
            break;
        case 6:
            r = 0.3411764705882353 + fraction_part * 0.2823529411764706;
            g = 0.7686274509803922 + fraction_part * 0.09411764705882353;
            b = 0.42745098039215684 - fraction_part * 0.17254901960784313;
            break;
        case 7:
            r = 0.6235294117647059 + fraction_part * 0.3411764705882353;
            g = 0.8627450980392157 + fraction_part * 0.050980392156862675;
            b = 0.2549019607843137 - fraction_part * 0.0549019607843137;
            break;
    }

    return RGB888{static_cast<std::uint8_t>(r * 255.0),
                  static_cast<std::uint8_t>(g * 255.0),
                  static_cast<std::uint8_t>(b * 255.0)};
}

inline RGB888 calculateColormapJet(double value)
{
    if (value < 0.0)
    {
        value = 0.0;
    }
    else if (value >= 1.0)
    {
        value = 0.99999;
    }

    double full_range_value = value * 6.0;
    double integer_part = floor(full_range_value);
    double fraction_part = full_range_value - integer_part;

    double r = 0.0, g = 0.0, b = 0.0;

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

    return RGB888{static_cast<std::uint8_t>(r * 255.0),
                  static_cast<std::uint8_t>(g * 255.0),
                  static_cast<std::uint8_t>(b * 255.0)};
}
}  // namespace

void testBasic() {}

void testLorenz()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const Vec3f p0{8.5f, 8.5f, 27.0f};
    const Vec3f p1{-8.5f, -8.5f, 27.0f};

    const size_t num_its = 50000;

    Vector<float> x(num_its), y(num_its), z(num_its);
    Vector<RGB888> colorp(num_its);

    float x0 = 1.0f;
    float y0 = 1.0f;
    float z0 = 1.0f;

    const float dt = 0.005f;
    const float sigma = 10.0f;
    const float rho = 28.0f;
    const float beta = 8.0f / 3.0f;

    const float max_dist = 32.0f;

    for (size_t k = 0; k < num_its; k++)
    {
        const float dx = sigma * (y0 - x0);
        const float dy = x0 * (rho - z0) - y0;
        const float dz = x0 * y0 - beta * z0;

        const float x1 = x0 + dx * dt;
        const float y1 = y0 + dy * dt;
        const float z1 = z0 + dz * dt;

        x(k) = x0;
        y(k) = y0;
        z(k) = z0;

        const float dist0 =
            std::sqrt((x1 - p0.x) * (x1 - p0.x) + (y1 - p0.y) * (y1 - p0.y) + (z1 - p0.z) * (z1 - p0.z));
        const float dist1 =
            std::sqrt((x1 - p1.x) * (x1 - p1.x) + (y1 - p1.y) * (y1 - p1.y) + (z1 - p1.z) * (z1 - p1.z));

        if (dist0 < dist1)
        {
            colorp(k) = calculateColormapJet(dist0 / max_dist);
        }
        else
        {
            colorp(k) = calculateColormapJet(dist1 / max_dist);
        }

        x0 = x1;
        y0 = y1;
        z0 = z1;
    }

    setCurrentElement("p_view_0");
    clearView();
    axis({-32.0f, -32.0f, -10.0f}, {32.0f, 32.0f, 55.0f});

    plot(x, y, colorp);

    plot3(x, y, z, colorp);
}

void testBump()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const int num_rows = 70, num_cols = 70;
    Matrix<double> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);

    const double inc = 0.4;

    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++)
        {
            const double rd = static_cast<double>(r - num_cols / 2) * inc;
            const double cd = static_cast<double>(c - num_rows / 2) * inc;
            x(r, c) = c;
            y(r, c) = r;
            const float r_val = std::sqrt(rd * rd + cd * cd);
            // const float r_val = std::sqrt(rd * rd + cd * cd);
            const float rand_val = static_cast<float>(rand() % 1001) / 1000.0f - 0.5f;
            z(r, c) = std::exp(-r_val * r_val * 0.1) + rand_val * 0.05f;
        }
    }

    setCurrentElement("p_view_0");
    clearView();
    surf(x, y, z, properties::EdgeColor::BLACK, properties::ColorMap::MAGMA);
}

void testScatterColorMap()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const int num_rows = 70, num_cols = 70;
    Matrix<float> x(num_rows, num_cols), y(num_rows, num_cols);

    const float inc = 0.4;
    const float nr = static_cast<float>(num_rows - 1);
    const float nc = static_cast<float>(num_cols - 1);

    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++)
        {
            const float rand_val_x = static_cast<float>(rand() % 1001) / 1000.0f - 0.5f;
            const float rand_val_y = static_cast<float>(rand() % 1001) / 1000.0f - 0.5f;

            x(r, c) = static_cast<float>(c) / nc + rand_val_x * 0.01f;
            y(r, c) = static_cast<float>(r) / nr + rand_val_y * 0.01f;
        }
    }

    const VectorConstView<float> x_vec{x.data(), x.size()}, y_vec{y.data(), y.size()};

    setCurrentElement("p_view_0");
    clearView();
    scatter(x_vec,
            y_vec,
            properties::ScatterStyle::DISC,
            properties::DistanceFrom::xy({0.5, 0.5}, 0.0, 1.0),
            properties::ColorMap::VIRIDIS);
}

void testScatterSamples()
{
    std::vector<float> x, y;

    float mean_val = 0.0f;
    for (std::int32_t k = 0; k < 20; k++)
    {
        std::int32_t n_samples = static_cast<std::int32_t>(rand() % 6) - 3 + 10;
        const float y_val = static_cast<float>(rand() % 1001) / 1000.0f + 1.2f;

        for (std::int32_t i = 0; i < n_samples; i++)
        {
            x.push_back(static_cast<float>(k) / 2.0f);
            const float y_rand = static_cast<float>(rand() % 1001) / 1000.0f - 0.5f;
            mean_val += y_val + y_rand * 0.1f;
            y.push_back(y_val + y_rand * 0.1f);
        }
    }
    Vector<float> x_vec = x;
    Vector<float> y_vec = y;

    mean_val /= static_cast<float>(x.size());

    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();
    scatter(x_vec,
            y_vec,
            properties::ScatterStyle::DISC,
            properties::DistanceFrom::y(mean_val, 0.0, 0.5),
            properties::ColorMap::VIRIDIS,
            properties::PointSize(20.0f));
}

void testFakeContour()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const std::int32_t num_rows = 100, num_cols = 100;
    Matrix<double> x(num_rows, num_cols), y(num_rows, num_cols), z(num_rows, num_cols);

    const double inc = 1.0 / static_cast<double>(num_rows);

    const auto r_func = [](const double x, const double y) -> double { return std::sqrt(x * x + y * y); };

    for (std::int32_t r = 0; r < num_rows; r++)
    {
        for (std::int32_t c = 0; c < num_cols; c++)
        {
            x(r, c) = c;
            y(r, c) = r;

            const double yd = (static_cast<double>(r + 1) + 0.5) * inc;
            const double xd = (static_cast<double>(c + 1) + 0.5) * inc;

            const float r_val0 = r_func(xd - 0.1, yd - 0.4);
            z(r, c) = std::sin(r_val0 * 15.0) / (r_val0 * 15.0);

            const float r_val1 = r_func(xd - 0.3, yd - 1.0);
            z(r, c) += std::sin(r_val1 * 15.0) / (r_val1 * 15.0);

            const float r_val2 = r_func(xd - 0.5, yd - 0.7);
            z(r, c) += std::sin(r_val2 * 7.0) / (r_val2 * 7.0);
        }
    }

    double min_val = dvs::min(z);
    double max_val = dvs::max(z);

    z = z + (std::abs(min_val) * 2.0);
    min_val = dvs::min(z);
    max_val = dvs::max(z);

    ImageRGB<std::uint8_t> img(num_rows, num_cols);

    const double num_values = 3.0;

    for (std::int32_t r = 0; r < num_rows; r++)
    {
        for (std::int32_t c = 0; c < num_cols; c++)
        {
            const double mapped_value = std::floor(num_values * (z(r, c) - min_val) / (max_val - min_val)) / num_values;
            const RGB888 col = calculateColormapViridis(mapped_value);
            img(r, c, 0) = col.red;
            img(r, c, 1) = col.green;
            img(r, c, 2) = col.blue;
        }
    }

    ImageRGB<std::uint8_t> img2 = img;

    RGB888 segment_color = RGB888{img(0, 0, 0), img(0, 0, 1), img(0, 0, 2)};

    std::int64_t idx = 0;
    std::int64_t current_row = 0, current_col = 0;

    // For vertical, 1 means down, -1 means up
    std::int64_t current_horizontal_dir = 1, current_vertical_dir = 1;

    Vec2<std::int64_t> start_index{0, 0};
    Vec2<std::int64_t> next_index{0, 0};

    for (std::int32_t r = 1; r < (num_rows - 1U); r++)
    {
        for (std::int32_t c = 1; c < (num_cols - 1U); c++)
        {
            const RGB888 col{img(r, c, 0), img(r, c, 1), img(r, c, 2)};
            const RGB888 col_left{img(r, c - 1, 0), img(r, c - 1, 1), img(r, c - 1, 2)};
            const RGB888 col_right{img(r, c + 1, 0), img(r, c + 1, 1), img(r, c + 1, 2)};
            const RGB888 col_up{img(r - 1, c, 0), img(r - 1, c, 1), img(r - 1, c, 2)};
            const RGB888 col_down{img(r + 1, c, 0), img(r + 1, c, 1), img(r + 1, c, 2)};
            if (((col == col_left) && (col == col_right) && (col == col_up) && (col == col_down)))
            {
                img2(r, c, 0) = 0;
                img2(r, c, 1) = 0;
                img2(r, c, 2) = 0;
            }
        }
    }

    while (false)
    {
        // Paint the edge pixel black
        img(current_row, current_col, 0) = 255;
        img(current_row, current_col, 1) = 255;
        img(current_row, current_col, 2) = 255;

        // Get the color of the current pixel
        const RGB888 current_color = RGB888{
            img(current_row, current_col, 0), img(current_row, current_col, 1), img(current_row, current_col, 2)};

        // Get the next pixel
        if (current_row == 0)
        {
            // Going right
            next_index = {current_row, current_col + 1};
        }

        if (current_color != segment_color)
        {
            segment_color = current_color;
            // img2(r, c, 0) = 255;
            // img2(r, c, 1) = 255;
            // img2(r, c, 2) = 255;
        }
    }

    setCurrentElement("p_view_0");
    clearView();
    // surf(x, y, z, properties::EdgeColor::BLACK, properties::ColorMap::VIRIDIS);

    imShow(img, properties::ZOffset(-0.05f));
    imShow(img2);
}

void testStocks()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const size_t num_elements = 1000;
    Vector<double> x(num_elements), y0(num_elements), yp0(num_elements), y1(num_elements), y2(num_elements);

    const auto r_func = []() -> double { return static_cast<double>(rand() % 1001) / 1000.0; };

    x(0) = 0.0;

    y0(0) = r_func();
    yp0(0) = 0.0;
    // y1(0) = r_func();
    // y2(0) = r_func();

    const double dt = 0.1;
    const double h = 0.5;
    const double h1 = 1.0 - h;

    for (size_t k = 1; k < num_elements; k++)
    {
        x(k) = static_cast<double>(k) * 0.1;

        yp0(k - 1) = yp0(k) + h1 * ((r_func() - 0.5) + 0.0 * std::sin(x(k) / 10.0));
        y0(k - 1) = y0(k) + yp0(k - 1) * dt;
        // y1(k - 1) = y1(k) + r_func();
        // y2(k - 1) = y2(k) + r_func();
    }

    setCurrentElement("p_view_0");
    clearView();
    plot(x, y0, properties::LineWidth(3.0f));
    // plot(x, y1);
    // plot(x, y2);
}

void testScatterCluster()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const size_t num_elements = 1000, num_cluester = 6;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);

    const auto r_func = []() -> double { return static_cast<double>(rand() % 1001) / 1000.0; };

    setCurrentElement("p_view_0");
    clearView();

    std::vector<properties::Color> colors{properties::Color{218, 83, 29},
                                          properties::Color{0, 114, 187},
                                          properties::Color{238, 177, 49},
                                          properties::Color{118, 172, 59},
                                          properties::Color{29, 218, 186},
                                          properties::Color{218, 29, 155}};

    // random device class instance, source of 'true' randomness for initializing random seed
    std::random_device rd;

    // Mersenne twister PRNG, initialized with seed from previous random device instance
    std::mt19937 gen(rd());

    for (size_t i = 0; i < num_cluester; i++)
    {
        const double x_center = r_func() * 10.0;
        const double y_center = r_func() * 10.0;
        const double z_center = r_func() * 10.0;

        const double x_std = (r_func() + 1.0) * 1.0;
        const double y_std = (r_func() + 1.0) * 1.0;
        const double z_std = (r_func() + 1.0) * 1.0;

        std::normal_distribution<double> dx{x_center, x_std};
        std::normal_distribution<double> dy{y_center, y_std};
        std::normal_distribution<double> dz{z_center, z_std};

        for (size_t k = 1; k < num_elements; k++)
        {
            x(k) = dx(gen);
            y(k) = dy(gen);
            z(k) = dz(gen);
        }

        scatter3(x, y, z, colors[i], properties::ScatterStyle::DISC);
    }
}

void testScatterVaryingSize()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const size_t num_elements = 200;
    Vector<double> x(num_elements), y(num_elements), z(num_elements);
    Vector<double> point_sizes(num_elements);
    Vector<RGB888> colorp(num_elements);

    double t = 0.0;
    double r = 1.0;

    for (size_t k = 0; k < num_elements; k++)
    {
        x(k) = r * std::cos(t);
        y(k) = r * std::sin(t);
        z(k) = r;
        point_sizes(k) = static_cast<double>(k) * 0.1 + 5.0;
        colorp(k) = calculateColormapJet(std::sin(t) * 0.5 + 0.5);
        t += 0.1;
        r += 0.01;
    }

    setCurrentElement("p_view_0");
    clearView();
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    scatter(x, y, colorp, point_sizes, properties::ScatterStyle::DISC);
    scatter3(x, y, z, colorp, point_sizes, properties::ScatterStyle::DISC);
    // scatter(x, y, point_sizes, properties::ScatterStyle::DISC);
    plot(x, y, properties::ScatterStyle::DISC);
}

void testScatterSmallPoints()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const size_t num_elements = 50000;
    Vector<double> x(num_elements), y(num_elements);

    double t = 0.0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<double> dx{0.0, 0.1};

    for (size_t k = 0; k < num_elements; k++)
    {
        x(k) = std::cos(t) + dx(gen);
        y(k) = std::sin(t) + dx(gen);
        t += 0.1;
    }

    setCurrentElement("p_view_0");
    clearView();
    axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    scatter(x,
            y,
            properties::PointSize(1),
            properties::ScatterStyle::SQUARE,
            properties::DistanceFrom::xy({0.0, 0.0}, 0.625, 1.25),
            properties::ColorMap::MAGMA);

    scatter(x + 0.3,
            y - 0.5,
            properties::PointSize(1),
            properties::ScatterStyle::SQUARE,
            properties::DistanceFrom::xy({0.3, -0.5}, 0.625, 1.25),
            properties::ColorMap::HSV);
}

void testTransparentFillBelowPlot()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const size_t num_elements = 1000;
    Vector<double> t(num_elements);

    const auto create_mesh = [](const Vector<double>& t,
                                const Vector<double>& x,
                                const double z_offset) -> std::tuple<Vector<Point3<double>>, Vector<IndexTriplet>> {
        const size_t num_elements = t.size();

        const size_t num_gaps = num_elements - 1U;
        const size_t num_triangles = num_gaps * 2U;
        const size_t num_vertices = num_elements * 2U;

        Vector<Point3<double>> vertices(num_vertices);
        Vector<IndexTriplet> indices(num_triangles);

        for (size_t k = 0; k < num_elements; k++)
        {
            vertices(k) = {t(k), x(k), z_offset};
            vertices(k + num_elements) = {t(k), 0.0, z_offset};
        }

        const std::uint32_t num_elements_uint32 = num_elements;

        for (size_t k = 0; k < num_gaps; k++)
        {
            const std::uint32_t ku = k;
            indices(k * 2U) = {ku, ku + 1U, ku + num_elements_uint32};
            indices(k * 2U + 1U) = {ku + 1U, ku + num_elements_uint32 + 1U, ku + num_elements_uint32};
        }

        return {vertices, indices};
    };

    t(0) = 0.0;
    double dt = 0.01;

    struct FunctionParameters
    {
        double Kf;
        double Cf;
        double m;
        double L;
        double dt;
        double x0;
        double dx0;
        double mu;
        double sigma;
        size_t num_elements;
    };

    for (size_t k = 0U; k < (num_elements - 1U); k++)
    {
        t(k + 1U) = t(k) + dt;
    }

    const auto create_function = [](const Vector<double>& t,
                                    const FunctionParameters& function_parameters) -> Vector<double> {
        const size_t num_elements = t.size();

        Vector<double> x{num_elements}, dx{num_elements};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<double> rand_gen{function_parameters.mu, function_parameters.sigma};

        x(0) = function_parameters.x0;
        dx(0) = function_parameters.dx0;

        for (size_t k = 0U; k < (function_parameters.num_elements - 1U); k++)
        {
            const double diff_x = x(k) - function_parameters.L;

            const double u = std::sin(t(k)) + rand_gen(gen);
            dx(k + 1) = dx(k) + function_parameters.dt *
                                    (-function_parameters.Kf * diff_x - function_parameters.Cf * dx(k) + u) /
                                    function_parameters.m;
            x(k + 1) = x(k) + function_parameters.dt * dx(k + 1);
        }

        return x;
    };

    const FunctionParameters fp0{.Kf = 5.0,
                                 .Cf = 0.1,
                                 .m = 1.0,
                                 .L = 0.25,
                                 .dt = dt,
                                 .x0 = 0.0,
                                 .dx0 = 0.0,
                                 .mu = 0.0,
                                 .sigma = 1.0,
                                 .num_elements = num_elements};

    const FunctionParameters fp1{.Kf = 4.0,
                                 .Cf = 0.3,
                                 .m = 1.3,
                                 .L = 0.25,
                                 .dt = dt,
                                 .x0 = 0.1,
                                 .dx0 = -1.0,
                                 .mu = 0.0,
                                 .sigma = 1.0,
                                 .num_elements = num_elements};
    const FunctionParameters fp2{.Kf = 3.0,
                                 .Cf = 0.2,
                                 .m = 1.0,
                                 .L = 0.25,
                                 .dt = dt,
                                 .x0 = 0.0,
                                 .dx0 = 0.0,
                                 .mu = 0.0,
                                 .sigma = 1.0,
                                 .num_elements = num_elements};

    const Vector<double> x0 = create_function(t, fp0);
    const Vector<double> x1 = create_function(t, fp1);
    const Vector<double> x2 = create_function(t, fp2);

    Vector<Point3<double>> vertices0, vertices1, vertices2;
    Vector<IndexTriplet> indices0, indices1, indices2;
    std::tie(vertices0, indices0) = create_mesh(t, x0, 0.0);
    std::tie(vertices1, indices1) = create_mesh(t, x1, -0.2);
    std::tie(vertices2, indices2) = create_mesh(t, x2, -0.4);

    setCurrentElement("p_view_0");
    clearView();
    // axis({-2.0, -2.0, -2.0}, {2.0, 2.0, 2.0});

    view(0, 90);

    plot(t, x0, properties::LineWidth(5), properties::Color::BLACK);
    plot(t, x1, properties::LineWidth(5), properties::Color::BLACK);
    plot(t, x2, properties::LineWidth(5), properties::Color::BLACK);
    drawMesh(vertices2, indices2, properties::Color::GREEN, properties::EdgeColor::NONE, properties::Alpha(0.5f));
    drawMesh(vertices1, indices1, properties::Color::RED, properties::EdgeColor::NONE, properties::Alpha(0.5f));
    drawMesh(vertices0, indices0, properties::Color::BLUE, properties::EdgeColor::NONE, properties::Alpha(0.5f));
}

void testTransitioningSurfs()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const size_t num_rows = 100, num_cols = 100;
    Matrix<double> x{num_rows, num_cols}, y{num_rows, num_cols}, z{num_rows, num_cols};

    const double x_min = 0.0, x_max = 1.0, y_min = 0.0, y_max = 1.0;

    const double inc = (x_max - x_min) / static_cast<double>(num_rows - 1U);

    for (int r = 0; r < num_rows; r++)
    {
        for (int c = 0; c < num_cols; c++)
        {
            x(r, c) = c * inc;
            y(r, c) = r * inc;

            const double rd = x(r, c) - 5.5 * inc;
            const double cd = y(r, c) - 5.5 * inc;
            const double r_val = std::sqrt(rd * rd + cd * cd) * 10.0;
            z(r, c) = std::sin(r_val) / r_val;
        }
    }

    const size_t num_surfaces{5U};

    Vector<Matrix<double>> surfaces{num_surfaces};

    /////////////////////////////////////////////////////////////////////////
    //////////////////////////// Create surfaces ////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    const Matrix<double> rd = x - 0.5;
    const Matrix<double> cd = y - 0.5;
    const Matrix<double> r_val = dvs::sqrt(elementWiseMultiply(rd, rd) + elementWiseMultiply(cd, cd)) * 10.0;

    surfaces(0) = dvs::sin(r_val) / r_val;

    const Matrix<double> r_val1 = dvs::sqrt(elementWiseMultiply(rd, rd) + elementWiseMultiply(cd, cd)) * 20.0;

    surfaces(1) = dvs::sin(r_val1) / r_val1;

    const Matrix<double> r_val2 = dvs::sqrt(elementWiseMultiply(rd, rd) + elementWiseMultiply(cd, cd)) * 30.0;

    surfaces(2) = elementWiseMultiply(rd, rd) - elementWiseMultiply(cd, cd);

    const Matrix<double> r_val3 = dvs::sqrt(elementWiseMultiply(rd, rd) + elementWiseMultiply(cd, cd)) * 40.0;

    surfaces(3) = elementWiseMultiply(rd, rd) + elementWiseMultiply(cd, cd);

    const Matrix<double> r_val4 = dvs::sqrt(elementWiseMultiply(rd, rd) + elementWiseMultiply(cd, cd)) * 50.0;

    surfaces(4) = dvs::sin(r_val4) / r_val4;

    /////////////////////////////////////////////////////////////////////////
    //////////////////////////// Create surfaces ////////////////////////////
    /////////////////////////////////////////////////////////////////////////

    setCurrentElement("p_view_0");
    clearView();
    view(-60, 40);
    axis({0.0, 0.0, -1.0}, {1.0, 1.0, 1.0});

    const size_t num_its = 100U;

    Matrix<double> z_mix{surfaces(0)};

    for (size_t k = 0U; k < (num_surfaces - 1U); k++)
    {
        Matrix<double>& z_k = surfaces(k);
        Matrix<double>& z_kp1 = surfaces(k + 1U);

        for (size_t i = 0; i < num_its; i++)
        {
            const double id = i;
            const double h = id / static_cast<double>(num_its - 1U);
            // 0, 1, 2, ..., i - 1
            // const Matrix<double> z_mix = z_k * (1.0 - h) + z_kp1 * h;
            const double dt = 0.5;
            const double dt1 = 1.0 - dt;
            z_mix = dt1 * z_mix + dt * (z_k * (1.0 - h) + z_kp1 * h);
            surf(x, y, z_mix, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET_BRIGHT);
            usleep(1000 * 10);
            softClearView();
        }
    }
    // surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET_BRIGHT);
}

void testIsoSurfaces()
{
    const size_t n_elems{100U};

    // const std::pair<Matrix<double>, Matrix<double>> uv_mats =
    //     meshGrid<double>(0.0, M_PI, 0.0, 2.0 * M_PI, n_elems, n_elems);
    const std::pair<Matrix<double>, Matrix<double>> uv_mats = meshGrid<double>(1.0, 5.0, 1.0, 10.0, n_elems, n_elems);
    Matrix<double> u{uv_mats.first}, v{uv_mats.second};

    const Matrix<double> r = 2.0 + sin(7.0 * u + 5.0 * v);
    // const Matrix<double> x = elementWiseMultiply(r, elementWiseMultiply(dvs::cos(u), dvs::sin(v)));
    // const Matrix<double> y = elementWiseMultiply(r, elementWiseMultiply(dvs::sin(u), dvs::sin(v)));
    // const Matrix<double> z = elementWiseMultiply(r, dvs::cos(v));

    const Matrix<double> x = elementWiseMultiply(u, sin(v));
    const Matrix<double> y = elementWiseMultiply(-u, cos(v));
    const Matrix<double> z = v;

    // funx = @(u,v) u.*sin(v);
    // funy = @(u,v) -u.*cos(v);
    // funz = @(u,v) v;

    setCurrentElement("p_view_0");
    clearView();
    surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET_BRIGHT);
}

void testHyperboloid()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);
    const size_t n_elems{30U};

    struct XYZMats
    {
        Matrix<double> x;
        Matrix<double> y;
        Matrix<double> z;
    };

    const auto hyberboloid = [](const double v0, const double v1, const double f) -> XYZMats {
        const std::pair<Matrix<double>, Matrix<double>> theta_v_mats =
            meshGrid<double>(0, 2.0 * M_PI, v0, v1, n_elems, n_elems);

        Matrix<double> theta{theta_v_mats.first}, v{theta_v_mats.second};
        const double a = 1.0;
        const double b = 1.0;
        const double c = 1.0;

        const Matrix<double> v2 = elementWiseMultiply(cosh(v), cosh(v));

        // if(f < )

        // const Matrix<double> x = a * elementWiseMultiply(dvs::sqrt(f + v2), cos(theta));
        // const Matrix<double> y = b * elementWiseMultiply(dvs::sqrt(f + v2), sin(theta));
        // const Matrix<double> z = c * sinh(v);

        const Matrix<double> x = a * elementWiseMultiply(dvs::sqrt(f + v2), dvs::cos(theta));
        const Matrix<double> y = b * elementWiseMultiply(dvs::sqrt(f + v2), dvs::sin(theta));
        const Matrix<double> z = c * sinh(v);

        return {x, y, z};

        // const Matrix<double> x = elementWiseMultiply(a * cosh(v), cos(theta));
        // const Matrix<double> y = elementWiseMultiply(b * cosh(v), sin(theta));
        // const Matrix<double> z = c * sinh(v);

        // const Matrix<double> x = elementWiseMultiply(a * sinh(v), cos(theta));
        // const Matrix<double> y = elementWiseMultiply(b * sinh(v), sin(theta));
        // const Matrix<double> z = c * cosh(v);

        // return {x, y, z};
    };

    setCurrentElement("p_view_0");
    clearView();
    disableScaleOnRotation();
    axis({-16.0, -16.0, -10.2}, {16.0, 16.0, 10.2});
    view(-65, 12);

    const size_t n_iters = 400U;

    for (size_t k = 0; k < n_iters; k++)
    {
        const double kf = static_cast<double>(k) / static_cast<double>(n_iters - 1U);

        const double f = -(4.0 - 8.0 * kf);
        const auto [x, y, z] = hyberboloid(-3.0, 3.0, f);

        softClearView();
        surf(x, y, z, properties::ID0, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET_SOFT);

        usleep(1000 * 1);
    }

    double t = 0.0;

    for (size_t k = 0; k < 500; k++)
    {
        const double theta_x = std::sin(t * 20.0);
        const double theta_y = std::sin(t * 10.0);
        const double theta_z = std::sin(t);

        const auto r_mat =
            rotationMatrixZ<double>(theta_z) * rotationMatrixY<double>(theta_y) * rotationMatrixX<double>(theta_x);

        const properties::Transform tr{diagMatrix<double>({1.0, 1.0, 1.0}), r_mat, {0, 0, 0}};

        setProperties(properties::ID0, properties::FaceColor::NONE);
        setTransform(properties::ID0, tr.scale, tr.rotation, tr.translation);

        usleep(1000 * 10);
        t += 0.001;
    }
}

void testSphere()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);
    const size_t n_elems{30U};

    const std::pair<Matrix<double>, Matrix<double>> theta_phi_mats =
        meshGrid<double>(0.0, M_PI, 0.0, 2.0 * M_PI, n_elems, n_elems);

    Matrix<double> theta{theta_phi_mats.first}, phi{theta_phi_mats.second};

    Matrix<double> r{n_elems, n_elems};
    r.fill(1.0);

    const Matrix<double> x = elementWiseMultiply(elementWiseMultiply(r, dvs::sin(theta)), dvs::cos(phi));
    const Matrix<double> y = elementWiseMultiply(elementWiseMultiply(r, dvs::sin(theta)), dvs::sin(phi));
    const Matrix<double> z = elementWiseMultiply(r, cos(theta));

    setCurrentElement("p_view_0");
    clearView();
    surf(x, y, z, properties::EdgeColor(0, 0, 0), properties::ColorMap::JET_SOFT);
}

void testLinesAndDots()
{
    // IMPORTANT: For this demo, set minimum_y_pos_ = 0; in "plot_pane.cpp"
    // and remove the showing of handle. Also set the snapping angle to 0.0
    const std::string project_file_path = "../../project_files/lines_and_dots.dvs";
    openProjectFile(project_file_path);

    const double size_bias = getArg<double>("size-bias", 3.0);
    const double size_multiplier = getArg<double>("size-multiplier", 10.0);

    const size_t num_elements = 200U, num_its = 500U;
    Vector<double> x(num_elements), y(num_elements), z(num_elements), vx(num_elements), vy(num_elements),
        vz(num_elements), point_sizes(num_elements), bx(num_elements), by(num_elements), bz(num_elements);

    for (size_t k = 0; k < num_elements; k++)
    {
        x(k) = (static_cast<double>(rand() % 1001) / 1000.0 - 0.5) * 3.5;
        y(k) = (static_cast<double>(rand() % 1001) / 1000.0 - 0.5) * 3.5;
        z(k) = (static_cast<double>(rand() % 1001) / 1000.0 - 0.5) * 3.5;

        bx(k) = (static_cast<double>(rand() % 1001) / 1000.0 - 0.5) * 2.5;
        by(k) = (static_cast<double>(rand() % 1001) / 1000.0 - 0.5) * 2.5;
        bz(k) = (static_cast<double>(rand() % 1001) / 1000.0 - 0.5) * 2.5;

        vx(k) = 0.0;
        vy(k) = 0.0;
        vz(k) = 0.0;

        point_sizes(k) = (static_cast<double>(rand() % 1001) / 1000.0) * size_multiplier + size_bias;
    }

    const double thresh = getArg<double>("dist-thresh", 0.05);

    setCurrentElement("p_view_0");
    clearView();
    view(180.0, 90.0);
    setAxesBoxScaleFactor({1.0, 1.0, 1.0});
    axesSquare();
    disableAutomaticAxesAdjustment();
    disableScaleOnRotation();

    axis({-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0});

    double azimuth = 180.0;
    double elevation = 90.0;

    for (size_t k = 0; k < num_its; k++)
    {
        std::vector<double> x_lines_std, y_lines_std, z_lines_std;
        x_lines_std.reserve(num_elements * num_elements);
        y_lines_std.reserve(num_elements * num_elements);
        z_lines_std.reserve(num_elements * num_elements);

        for (size_t i = 0; i < num_elements; i++)
        {
            const double outer_x = x(i);
            const double outer_y = y(i);
            const double outer_z = z(i);

            for (size_t j = 0; j < num_elements; j++)
            {
                const double inner_x = x(j);
                const double inner_y = y(j);
                const double inner_z = z(j);

                const double dx = outer_x - inner_x;
                const double dy = outer_y - inner_y;
                const double dz = outer_z - inner_z;
                const double dx2 = dx * dx;
                const double dy2 = dy * dy;
                const double dz2 = dz * dz;

                if (dx2 + dy2 + dz2 < thresh)
                {
                    x_lines_std.push_back(inner_x);
                    x_lines_std.push_back(outer_x);
                    y_lines_std.push_back(inner_y);
                    y_lines_std.push_back(outer_y);
                    z_lines_std.push_back(inner_z);
                    z_lines_std.push_back(outer_z);
                }
            }
        }

        Vector<double> x_lines = x_lines_std;
        Vector<double> y_lines = y_lines_std;
        Vector<double> z_lines = z_lines_std;

        softClearView();
        // if (k > num_its / 2U)
        azimuth -= 0.5;
        if (azimuth < -180.0)
        {
            azimuth = 180.0;
        }
        elevation -= 0.1;
        view(azimuth, elevation);

        scatter3(x,
                 y,
                 z,
                 point_sizes,
                 properties::Color(0.1686 * 255.0, 0.1686 * 255.0, 0.2510 * 255.0),
                 properties::ScatterStyle::DISC);
        lineCollection3(x_lines, y_lines, z_lines, properties::Color(0.1686 * 255.0, 0.1686 * 255.0, 0.2510 * 255.0));

        const double h = 0.01;

        for (size_t i = 0; i < num_elements; i++)
        {
            vx(i) = (static_cast<double>(rand() % 1001) / 1000.0 - bx(i)) * 0.1;
            vy(i) = (static_cast<double>(rand() % 1001) / 1000.0 - by(i)) * 0.1;

            x(i) = x(i) + vx(i) * h;
            y(i) = y(i) + vy(i) * h;
            /*if (y(i) > 1.0)
            {
                y(i) = 1.0 - (y(i) - 1.0);
                vy(i) = -vy(i);
            }*/
        }

        usleep(10 * 1000);
    }
}

void testLissaJous()
{
    const std::string project_file_path = "../../project_files/small.dvs";
    openProjectFile(project_file_path);

    const size_t n_elements = 700;
    Vector<double> x(n_elements), y(n_elements);

    const double t_end = 2.0 * M_PI;

    double t = 0.0;

    const double dt = t_end / static_cast<double>(n_elements - 1U);

    const double a = 3.0;
    const double b = 4.0;
    const double d = M_PI / 2.0;

    for (size_t k = 0; k < n_elements; k++)
    {
        x(k) = std::sin(a * t + d);
        y(k) = std::sin(b * t);

        t = t + dt;
    }

    setCurrentElement("p_view_0");
    clearView();

    const double delta = 0.3;

    for (size_t k = 10; k < n_elements; k += 3)
    {
        Vector<double> xt(k), yt(k);

        for (size_t i = 0; i < k; i++)
        {
            xt(i) = x(i);
            yt(i) = y(i);
        }

        softClearView();
        axis({xt(k - 1U) - delta, yt(k - 1U) - delta}, {xt(k - 1U) + delta, yt(k - 1U) + delta});
        plot(xt, yt, properties::Color(201, 238, 121), properties::LineWidth(10.0f));
    }

    const double x_final = 1.0, y_final = 0.0;

    for (size_t k = 0; k < 100; k++)
    {
        const double d_val = static_cast<double>(k) / 99.0;
        const double d_val_1 = 1.0 - d_val;

        const double x_min = (x_final - delta) * d_val_1 + d_val * -1.0;
        const double x_max = (x_final + delta) * d_val_1 + d_val * 1.0;
        const double y_min = (y_final - delta) * d_val_1 + d_val * -1.0;
        const double y_max = (y_final + delta) * d_val_1 + d_val * 1.0;

        axis({x_min, y_min}, {x_max, y_max});
        usleep(1000 * 10);
    }

    axis({-1.0, -1.0}, {1.0, 1.0});

    plot(x, y, properties::Color(201, 238, 121), properties::LineWidth(10.0f));
}

}  // namespace small

#endif  // DEMOS_SMALL_H
