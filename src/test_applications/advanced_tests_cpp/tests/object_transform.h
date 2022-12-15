#ifndef TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_OBJECT_TRANSFORM_H_
#define TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_OBJECT_TRANSFORM_H_

#include <unistd.h>

#include "dvs/dvs.h"

using namespace dvs;

namespace object_transform
{

void testBasic()
{
    const size_t num_elements = 5;

    Vector<float> x{num_elements}, y{num_elements};

    x(0) = -1.0;
    x(1) = 1.0;
    x(2) = 1.0;
    x(3) = -1.0;
    x(4) = 0.0;

    y(0) = -1.0;
    y(1) = -1.0;
    y(2) = 1.0;
    y(3) = 1.0;
    y(4) = 0.0;

    setCurrentElement("p_view_0");
    clearView();
    axis({-2.0, -2.0}, {2.0, 2.0});
    plot(x, y, properties::SLOT0, properties::LineWidth(10.0f));

    usleep(100 * 1000);

    Vec3d translation{0.1, 0.1, 0.0};
    Vec3d scale{1.0, 1.0, 1.0};

    Matrix<double> r_mat = rotationMatrixZ(0.0);

    for (size_t k = 0; k < 50; k++)
    {
        scale.x += 0.01;

        setTransform(properties::SLOT0, scale, r_mat, translation);
        usleep(50 * 1000);
    }

    for (size_t k = 0; k < 50; k++)
    {
        translation.x += 0.01;

        setTransform(properties::SLOT0, scale, r_mat, translation);
        usleep(50 * 1000);
    }

    float angle = 0.0;

    for (size_t k = 0; k < 50; k++)
    {
        r_mat = rotationMatrixZ(angle);
        angle -= 0.01;

        setTransform(properties::SLOT0, scale, r_mat, translation);
        usleep(50 * 1000);
    }

    for (size_t k = 0; k < 50; k++)
    {
        scale.x += 0.01;

        setTransform(properties::SLOT0, scale, r_mat, translation);
        usleep(50 * 1000);
    }

    for (size_t k = 0; k < 50; k++)
    {
        translation.x += 0.01;

        setTransform(properties::SLOT0, scale, r_mat, translation);
        usleep(50 * 1000);
    }

    for (size_t k = 0; k < 50; k++)
    {
        r_mat = rotationMatrixZ(angle);
        angle -= 0.01;

        setTransform(properties::SLOT0, scale, r_mat, translation);
        usleep(50 * 1000);
    }
}

}  // namespace object_transform

#endif  // TEST_APPLICATIONS_ADVANCED_TESTS_CPP_TESTS_OBJECT_TRANSFORM_H_
