#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../test_data/test_data_rotations.h"
#include "arl/math.h"
#include "arl/math/lin_alg.h"
#include "test/unit_tests/math/math_test_utils.h"

// https://www.andre-gaschler.com/rotationconverter/

namespace arl
{
namespace
{
class AxisAngleTest : public testing::Test
{
protected:
    void SetUp() override
    {
        TEST_SETUP_DATA_ROTATIONS();
    }

    void TearDown() override {}
};

TEST_F(AxisAngleTest, Initialization)
{
    const double eps = 1e-7;
    const double phi = 0.4124;
    const double x = 1.0235;
    const double y = -0.2623;
    const double z = 0.001;

    const AxisAngled axis_angle(phi, x, y, z);

    ASSERT_NEAR(phi, axis_angle.phi, eps);
    ASSERT_NEAR(x, axis_angle.x, eps);
    ASSERT_NEAR(y, axis_angle.y, eps);
    ASSERT_NEAR(z, axis_angle.z, eps);
}

TEST_F(AxisAngleTest, AxisAngleToMatrix)
{
    const double eps = 1e-6;

    for (size_t k = 0; k < 13; k++)
    {
        const AxisAngled axis_angle(angle_arrays[k][0],
                                    axis_arrays[k][0] * 1.0,
                                    axis_arrays[k][1] * 1.0,
                                    axis_arrays[k][2] * 1.0);
        const Matrixd mat_expected = Matrix<double>(rotation_matrix_arrays[k]);
        const Matrixd mat_actual = axis_angle.toRotationMatrix();
        ASSERT_MATRIX_NEAR_MATRIX(mat_expected, mat_actual, eps);
    }
}

TEST_F(AxisAngleTest, MatrixToAxisAngle)
{
    const double eps = 1e-7;

    for (size_t k = 0; k < 13; k++)
    {
        const Matrixd rotation_matrix = Matrix<double>(rotation_matrix_arrays[k]);

        const AxisAngled axis_angle_expected(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);

        const AxisAngled axis_angle_actual = rotationMatrixToAxisAngle(rotation_matrix);
        ASSERT_NEAR(axis_angle_expected.phi, axis_angle_actual.phi, eps);
        ASSERT_NEAR(axis_angle_expected.x, axis_angle_actual.x, eps);
        ASSERT_NEAR(axis_angle_expected.y, axis_angle_actual.y, eps);
        ASSERT_NEAR(axis_angle_expected.z, axis_angle_actual.z, eps);
    }
}

TEST_F(AxisAngleTest, AxisAngleToMatrixAndBack)
{
    const double eps = 1e-6;

    for (size_t k = 0; k < 13; k++)
    {
        const AxisAngled axis_angle_expected(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);

        const Matrixd rotation_mat = axis_angle_expected.toRotationMatrix();
        const AxisAngled axis_angle_actual = rotationMatrixToAxisAngle(rotation_mat);
        ASSERT_NEAR(axis_angle_expected.phi, axis_angle_actual.phi, eps);
        ASSERT_NEAR(axis_angle_expected.x, axis_angle_actual.x, eps);
        ASSERT_NEAR(axis_angle_expected.y, axis_angle_actual.y, eps);
        ASSERT_NEAR(axis_angle_expected.z, axis_angle_actual.z, eps);
    }
}

TEST_F(AxisAngleTest, RollPitchYawToAxisAngle)
{
    const double eps = 1e-6;

    for (size_t k = 0; k < 13; k++)
    {
        const RollPitchYawd rpy(rpy_arrays[k][0], rpy_arrays[k][1], rpy_arrays[k][2]);
        const AxisAngled axis_angle_expected(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);
        const AxisAngled axis_angle_actual = rpy.toAxisAngle();

        ASSERT_NEAR(axis_angle_expected.phi, axis_angle_actual.phi, eps);
        ASSERT_NEAR(axis_angle_expected.x, axis_angle_actual.x, eps);
        ASSERT_NEAR(axis_angle_expected.y, axis_angle_actual.y, eps);
        ASSERT_NEAR(axis_angle_expected.z, axis_angle_actual.z, eps);
    }
}

TEST_F(AxisAngleTest, QuaternionToAxisAngle)
{
    const double eps = 1e-5;

    // From quaternion to axis angle and back
    for (size_t k = 0; k < 13; k++)
    {
        const Quaterniond q(quaternion_arrays[k][3],
                            quaternion_arrays[k][0],
                            quaternion_arrays[k][1],
                            quaternion_arrays[k][2]);
        const AxisAngled axis_angle_expected(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);
        const AxisAngled axis_angle_actual = q.toAxisAngle();

        ASSERT_NEAR(axis_angle_expected.phi, axis_angle_actual.phi, eps);
        ASSERT_NEAR(axis_angle_expected.x, axis_angle_actual.x, eps);
        ASSERT_NEAR(axis_angle_expected.y, axis_angle_actual.y, eps);
        ASSERT_NEAR(axis_angle_expected.z, axis_angle_actual.z, eps);
    }
}

}  // namespace
}  // namespace arl
