#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../test_data/test_data_rotations.h"
#include "arl/math.h"
#include "test/unit_tests/math/math_test_utils.h"

namespace arl
{
namespace
{
class QuaternionTest : public testing::Test
{
protected:
    void SetUp() override
    {
        TEST_SETUP_DATA_ROTATIONS();
    }

    void TearDown() override {}
};

// https://www.andre-gaschler.com/rotationconverter/

TEST_F(QuaternionTest, Initialization)
{
    const double eps = 1e-6;

    const double w = 24.5234;
    const double x = 53.9023;
    const double y = 3.32;
    const double z = -23.332;
    const Quaterniond q(w, x, y, z);

    ASSERT_NEAR(q.w, w, eps);
    ASSERT_NEAR(q.x, x, eps);
    ASSERT_NEAR(q.y, y, eps);
    ASSERT_NEAR(q.z, z, eps);
}

TEST_F(QuaternionTest, NormalizeQuaternion)
{
    const double eps = 1e-6;

    const Quaterniond q(0.9252, 1.2141, 0.634, 1.623);
    const Quaterniond q_expected(0.3993976, 0.5241123, 0.2736901, 0.7006294);
    const Quaterniond q_actual = q.normalized();

    ASSERT_TRUE(fabs(q.norm() - 1.0) > 1.0);

    ASSERT_NEAR(q_expected.w, q_actual.w, eps);
    ASSERT_NEAR(q_expected.x, q_actual.x, eps);
    ASSERT_NEAR(q_expected.y, q_actual.y, eps);
    ASSERT_NEAR(q_expected.z, q_actual.z, eps);
}

TEST_F(QuaternionTest, QuaternionToRotationMatrix)
{
    const double eps = 1e-6;

    for (size_t k = 0; k < 13; k++)
    {
        const Quaterniond q(quaternion_arrays[k][3],
                            quaternion_arrays[k][0],
                            quaternion_arrays[k][1],
                            quaternion_arrays[k][2]);

        Matrixd rotation_matrix_expected = Matrix<double>(rotation_matrix_arrays[k]);
        const Matrixd rotation_matrix_actual = q.toRotationMatrix();

        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual, eps);
    }
}

TEST_F(QuaternionTest, QuaternionToAxisAngle)
{
    const double eps = 1e-5;

    for (size_t k = 0; k < 13; k++)
    {
        const Quaterniond q(quaternion_arrays[k][3],
                            quaternion_arrays[k][0],
                            quaternion_arrays[k][1],
                            quaternion_arrays[k][2]);
        const AxisAngled axis_angle_expected(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);
        const AxisAngled axis_angle_actual = q.toAxisAngle();

        ASSERT_NEAR(axis_angle_actual.phi, axis_angle_expected.phi, eps);
        ASSERT_NEAR(axis_angle_actual.x, axis_angle_expected.x, eps);
        ASSERT_NEAR(axis_angle_actual.y, axis_angle_expected.y, eps);
        ASSERT_NEAR(axis_angle_actual.z, axis_angle_expected.z, eps);
    }
}

TEST_F(QuaternionTest, AxisAngleToQuaternion)
{
    const double eps = 1e-6;

    for (size_t k = 0; k < 13; k++)
    {
        const Quaterniond q_expected(quaternion_arrays[k][3],
                                     quaternion_arrays[k][0],
                                     quaternion_arrays[k][1],
                                     quaternion_arrays[k][2]);
        const AxisAngled axis_angle(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);
        const Quaterniond q_actual = axis_angle.toQuaternion();

        ASSERT_NEAR(q_expected.w, q_actual.w, eps);
        ASSERT_NEAR(q_expected.x, q_actual.x, eps);
        ASSERT_NEAR(q_expected.y, q_actual.y, eps);
        ASSERT_NEAR(q_expected.z, q_actual.z, eps);
    }
}

TEST_F(QuaternionTest, AxisAngleQuaternionBackAndForth)
{
    const double eps = 1e-6;

    // From quaternion to axis angle and back
    for (size_t k = 0; k < 13; k++)
    {
        const Quaterniond q_expected(quaternion_arrays[k][3],
                                     quaternion_arrays[k][0],
                                     quaternion_arrays[k][1],
                                     quaternion_arrays[k][2]);

        const AxisAngled axis_angle = q_expected.toAxisAngle();
        const Quaterniond q_actual = axis_angle.toQuaternion();

        ASSERT_NEAR(q_expected.w, q_actual.w, eps);
        ASSERT_NEAR(q_expected.x, q_actual.x, eps);
        ASSERT_NEAR(q_expected.y, q_actual.y, eps);
        ASSERT_NEAR(q_expected.z, q_actual.z, eps);
    }

    // From axis angle to quaternion and back
    for (size_t k = 0; k < 13; k++)
    {
        const AxisAngled axis_angle_expected(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);

        const Quaterniond q = axis_angle_expected.toQuaternion();
        const AxisAngled axis_angle_actual = q.toAxisAngle();

        ASSERT_NEAR(axis_angle_expected.phi, axis_angle_actual.phi, eps);
        ASSERT_NEAR(axis_angle_expected.x, axis_angle_actual.x, eps);
        ASSERT_NEAR(axis_angle_expected.y, axis_angle_actual.y, eps);
        ASSERT_NEAR(axis_angle_expected.z, axis_angle_actual.z, eps);
    }
}

TEST_F(QuaternionTest, RollPitchYawToQuaternion)
{
    const double eps = 1e-7;

    for (size_t k = 0; k < 13; k++)
    {
        const Quaterniond q_expected(quaternion_arrays[k][3],
                                     quaternion_arrays[k][0],
                                     quaternion_arrays[k][1],
                                     quaternion_arrays[k][2]);
        const RollPitchYawd rpy(rpy_arrays[k][0], rpy_arrays[k][1], rpy_arrays[k][2]);
        const Quaterniond q_actual = rpy.toQuaternion();

        ASSERT_NEAR(q_expected.w, q_actual.w, eps);
        ASSERT_NEAR(q_expected.x, q_actual.x, eps);
        ASSERT_NEAR(q_expected.y, q_actual.y, eps);
        ASSERT_NEAR(q_expected.z, q_actual.z, eps);
    }
}

}  // namespace
}  // namespace arl
