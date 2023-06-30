#ifndef DEMOS_SMALL_H
#define DEMOS_SMALL_H

namespace small
{

namespace
{

inline RGB888 calculateColormapJet(double value)
{
    if(value < 0.0)
    {
        value = 0.0;
    }
    else if(value >= 1.0)
    {
        value = 0.99999;
    }

    double full_range_value = value * 6.0;
    double integer_part = floor(full_range_value);
    double fraction_part = full_range_value - integer_part;

    double r = 0.0, g = 0.0, b = 0.0;

    switch(int(integer_part))
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

    return RGB888{
        static_cast<std::uint8_t>(r * 255.0),
        static_cast<std::uint8_t>(g * 255.0), 
        static_cast<std::uint8_t>(b * 255.0)};
}
}

void testBasic()
{

}

void testLorenz()
{
    const std::string project_file_path = "../../project_files/particle_field.dvs";
    openProjectFile(project_file_path);

    const Vec3f p0{8.5f, 8.5f, 27.0f};
    const Vec3f p1{-8.5f, -8.5f, 27.0f};

    const size_t num_its = 10000;

    Vector<float> x(num_its), y(num_its), z(num_its);
    Vector<RGB888> colorp(num_its);

    float x0 = 1.0f;
    float y0 = 1.0f;
    float z0 = 1.0f;

    const float dt = 0.01f;
    const float sigma = 10.0f;
    const float rho = 28.0f;
    const float beta = 8.0f / 3.0f;

    const float max_dist = 16.0f;

    for(size_t k = 0; k < num_its; k++)
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

        const float dist0 = std::sqrt((x1 - p0.x) * (x1 - p0.x) + (y1 - p0.y) * (y1 - p0.y) + (z1 - p0.z) * (z1 - p0.z));
        const float dist1 = std::sqrt((x1 - p1.x) * (x1 - p1.x) + (y1 - p1.y) * (y1 - p1.y) + (z1 - p1.z) * (z1 - p1.z));

        if(dist0 < dist1)
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
    
    // scatter(x, y);
    plot(x, y);
    // usleep(1000000);
    
    plot3(x, y, z);

}

void testBump()
{
    const std::string project_file_path = "../../project_files/particle_field.dvs";
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
    const std::string project_file_path = "../../project_files/particle_field.dvs";
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

            x(r, c) = static_cast<float>(c) / nc + rand_val_x * 0.01f;;
            y(r, c) = static_cast<float>(r) / nr + rand_val_y * 0.01f;;
        }
    }

    const VectorConstView<float> x_vec{x.data(), x.size()}, y_vec{y.data(), y.size()};

    setCurrentElement("p_view_0");
    clearView();
    scatter(x_vec, y_vec, properties::ScatterStyle::DISC, properties::DistanceFrom::xy({0.5, 0.5}, 0.0, 1.0), properties::ColorMap::VIRIDIS);
}

void testScatterSamples()
{
    std::vector<float> x, y;

    float mean_val = 0.0f;
    for(std::int32_t k = 0; k < 20; k++)
    {
        std::int32_t n_samples = static_cast<std::int32_t>(rand() % 6) - 3 + 10;
        const float y_val = static_cast<float>(rand() % 1001) / 1000.0f + 1.2f;

        for(std::int32_t i = 0; i < n_samples; i++)
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

    const std::string project_file_path = "../../project_files/particle_field.dvs";
    openProjectFile(project_file_path);

    setCurrentElement("p_view_0");
    clearView();
    scatter(x_vec, y_vec,
        properties::ScatterStyle::DISC,
        properties::DistanceFrom::y(mean_val, 0.0, 0.5),
        properties::ColorMap::VIRIDIS,
        properties::PointSize(20.0f));
}


}

#endif // DEMOS_SMALL_H
