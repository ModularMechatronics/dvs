#ifndef DEMOS_TESTS_CAR_CAR_H_
#define DEMOS_TESTS_CAR_CAR_H_

#include <fstream>
#include <iterator>
#include <tuple>
#include <vector>

#include "dvs/dvs.h"

using namespace dvs;

namespace car
{

ImageRGBA<uint8_t> readCarImage()
{
    const std::string bin_path = "../demos/tests/car/car.bin";
    std::ifstream input(bin_path, std::ios::binary);

    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});

    const size_t num_rows = 150;
    const size_t num_cols = 300;

    ImageGrayView<uint8_t> img_view{buffer.data(), num_rows, num_cols};

    ImageRGBA<uint8_t> img_c4(num_rows, num_cols);

    for (size_t r = 0; r < num_rows; r++)
    {
        for (size_t c = 0; c < num_cols; c++)
        {
            const uint8_t pixel_val = img_view(r, c);

            if (pixel_val == 255)
            {
                img_c4(r, c, 3) = 0;
            }
            else
            {
                img_c4(r, c, 0) = pixel_val;
                img_c4(r, c, 1) = pixel_val;
                img_c4(r, c, 2) = pixel_val;
                img_c4(r, c, 3) = 255;
            }
        }
    }

    return img_c4;
}

std::tuple<Vector<float>, Vector<float>, Vector<float>> generatePath()
{
    float phi = 0.0f, v = 2.0f;
    const size_t num_its = 100;
    srand(4);

    Vector<float> xv{num_its}, yv{num_its}, phiv{num_its};

    xv(0) = 0.0f;
    yv(0) = 0.0f;
    phiv(0) = 0.0f;
    const float h = 0.001;
    const float h_1 = 1.0f - h;

    for (size_t k = 1; k < num_its; k++)
    {
        xv(k) = xv(k - 1) + v * std::cos(phi);
        yv(k) = yv(k - 1) + v * std::sin(phi);
        phiv(k) = phi;

        float r0 = 100.0f * (static_cast<float>(rand() % 1001) / 1000.0f - 0.5f);
        float r1 = 100.0f * (static_cast<float>(rand() % 1001) / 1000.0f - 0.5f);

        v = h_1 * v + h * r0;
        phi = h_1 * phi + h * r1;
    }

    return {xv, yv, phiv};
}

void testBasic()
{
    const ImageRGBA<uint8_t> img = readCarImage();
    ImageGray<uint8_t> road(1, 1);
    ImageGray<uint8_t> line(1, 1);

    road(0, 0) = 127U;
    line(0, 0) = 200U;

    Vector<float> x, y, phi;
    std::tie(x, y, phi) = generatePath();

    const Vec3d scale{0.1, 0.1, 1.0};
    const Vec3d center_of_rotation = Vec3d{150.0, 75.0, 0.0}.elementWiseMultiply(scale);

    setCurrentElement("p_view_0");
    clearView();

    axis({-32.0, -32.0, -10.0}, {-32.0 + 223, -32.0 + 115.733333184, 10.0});
    view(0, 90);

    imShow(road,
           properties::ZOffset(-0.25f),
           properties::Transform{{204, 40, 0}, rotationMatrixZ<double>(0), {-20, -20, -3}});

    for (size_t k = 0; k < 17; k++)
    {
        const double kf = k;
        imShow(line, properties::Transform{{3, 2, 0}, unitMatrix<double>(3, 3), {kf * 12.0 - 18.0, -1.0, -2}});
    }

    for (size_t k = 0; k < 14; k++)
    {
        const double kf = k;
        const double dy = 17.0;
        imShow(line, properties::Transform{{5, 2, 0}, unitMatrix<double>(3, 3), {kf * 15.0 - 19.0, -1.0 - dy, -2}});
        imShow(line, properties::Transform{{5, 2, 0}, unitMatrix<double>(3, 3), {kf * 15.0 - 19.0, -1.0 + dy, -2}});
    }

    plot(x, y, properties::Color(0, 255, 0), properties::LineWidth(5), properties::ZOffset(-0.15));
    scatter(x, y, properties::Color(255, 0, 0), properties::ZOffset(-0.13));
    imShow(img,
           properties::SLOT0,
           properties::ZOffset(-0.05f),
           properties::Transform{
               scale, rotationMatrixZ<double>(M_PI), -rotationMatrixZ<double>(M_PI) * center_of_rotation});

    for (size_t k = 0; k < x.size(); k++)
    {
        const double ang = phi(k) - M_PI;

        const Matrix<double> r_mat = rotationMatrixZ<double>(ang);

        const Vec3d center_of_rotation_r = r_mat * center_of_rotation;
        const Vec3d v_res = Vec3d{x(k), y(k), 0.0f} - center_of_rotation_r;

        setTransform(properties::SLOT0, scale, r_mat, v_res);
        usleep(50 * 1000);
    }
}

}  // namespace car

#endif  // DEMOS_TESTS_CAR_CAR_H_
