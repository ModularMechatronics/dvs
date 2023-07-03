#ifndef DEMOS_SMALL_H
#define DEMOS_SMALL_H

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
            ;
            y(r, c) = static_cast<float>(r) / nr + rand_val_y * 0.01f;
            ;
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
    surf(x, y, z, properties::EdgeColor::BLACK, properties::ColorMap::VIRIDIS);

    imShow(img);
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

}  // namespace small

#endif  // DEMOS_SMALL_H
