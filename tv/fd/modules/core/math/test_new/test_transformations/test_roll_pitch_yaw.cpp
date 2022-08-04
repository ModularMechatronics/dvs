#include <gtest/gtest.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "../test_data/test_data_rotations.h"
#include "arl/math.h"
#include "test/unit_tests/math/math_test_utils.h"

// https://www.andre-gaschler.com/rotationconverter/

namespace arl
{
namespace
{
class RollPitchYawTest : public testing::Test
{
protected:
    void SetUp() override
    {
        TEST_SETUP_DATA_ROTATIONS();
    }

    void TearDown() override {}
};

TEST_F(RollPitchYawTest, Initialization)
{
    const double eps = 1e-6;
    const double roll_expected = 0.2423;
    const double pitch_expected = 2.5394;
    const double yaw_expected = -3.2323;

    const RollPitchYawd rpy(roll_expected, pitch_expected, yaw_expected);

    ASSERT_NEAR(roll_expected, rpy.roll, eps);
    ASSERT_NEAR(pitch_expected, rpy.pitch, eps);
    ASSERT_NEAR(yaw_expected, rpy.yaw, eps);
}

TEST_F(RollPitchYawTest, RpyToMatrix)
{
    const double eps = 1e-6;

    for (int k = 0; k < 13; k++)
    {
        const RollPitchYawd rpy(rpy_arrays[k][0], rpy_arrays[k][1], rpy_arrays[k][2]);

        const Matrixd rotation_matrix_expected = Matrix<double>(rotation_matrix_arrays[k]);
        const Matrixd rotation_matrix_actual = rpy.toRotationMatrix();

        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual, eps);
    }
}

TEST_F(RollPitchYawTest, RpyToMatrixAndBack)
{
    const double eps = 1e-6;

    for (int k = 0; k < 13; k++)
    {
        const RollPitchYawd rpy_expected(rpy_arrays[k][0], rpy_arrays[k][1], rpy_arrays[k][2]);

        const Matrixd rotation_matrix = rpy_expected.toRotationMatrix();

        const RollPitchYawd rpy_actual = rotationMatrixToRollPitchYaw(rotation_matrix);

        ASSERT_NEAR(rpy_expected.roll, rpy_actual.roll, eps);
        ASSERT_NEAR(rpy_expected.pitch, rpy_actual.pitch, eps);
        ASSERT_NEAR(rpy_expected.yaw, rpy_actual.yaw, eps);
    }
}

TEST_F(RollPitchYawTest, MatrixToRpy)
{
    const double eps = 1e-6;

    for (int k = 0; k < 13; k++)
    {
        const RollPitchYawd rpy_expected(rpy_arrays[k][0], rpy_arrays[k][1], rpy_arrays[k][2]);
        const Matrixd rotation_matrix = Matrix<double>(rotation_matrix_arrays[k]);

        const RollPitchYawd rpy_actual = rotationMatrixToRollPitchYaw(rotation_matrix);

        ASSERT_NEAR(rpy_expected.roll, rpy_actual.roll, eps);
        ASSERT_NEAR(rpy_expected.pitch, rpy_actual.pitch, eps);
        ASSERT_NEAR(rpy_expected.yaw, rpy_actual.yaw, eps);
    }
}

TEST_F(RollPitchYawTest, RotationMatrixRollAndX)
{
    const double eps = 1e-7;

    for (size_t k = 8; k < 10; k++)
    {
        const Matrixd rotation_matrix_expected = Matrix<double>(rotation_matrix_arrays[k]);
        const Matrixd rotation_matrix_actual_roll = rotationMatrixFromRoll(rpy_arrays[k][0]);
        const Matrixd rotation_matrix_actual_x = rotationMatrixX(rpy_arrays[k][0]);

        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual_roll, eps);
        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual_x, eps);
    }
}

TEST_F(RollPitchYawTest, RotationMatrixPitchAndY)
{
    const double eps = 1e-7;

    for (size_t k = 10; k < 12; k++)
    {
        const Matrixd rotation_matrix_expected = Matrix<double>(rotation_matrix_arrays[k]);
        const Matrixd rotation_matrix_actual_pitch = rotationMatrixFromPitch(rpy_arrays[k][1]);
        const Matrixd rotation_matrix_actual_y = rotationMatrixY(rpy_arrays[k][1]);

        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual_pitch, eps);
        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual_y, eps);
    }
}

TEST_F(RollPitchYawTest, RotationMatrixYawAndZ)
{
    const double eps = 1e-7;

    for (size_t k = 12; k < 14; k++)
    {
        const Matrixd rotation_matrix_expected = Matrix<double>(rotation_matrix_arrays[k]);
        const Matrixd rotation_matrix_actual_yaw = rotationMatrixFromYaw(rpy_arrays[k][2]);
        const Matrixd rotation_matrix_actual_z = rotationMatrixZ(rpy_arrays[k][2]);

        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual_yaw, eps);
        ASSERT_MATRIX_NEAR_MATRIX(rotation_matrix_expected, rotation_matrix_actual_z, eps);
    }
}

TEST_F(RollPitchYawTest, QuaternionToRollPitchYaw)
{
    const double eps = 1e-6;

    for (size_t k = 0; k < 13; k++)
    {
        const RollPitchYawd rpy_expected(rpy_arrays[k][0], rpy_arrays[k][1], rpy_arrays[k][2]);
        const Quaterniond q(quaternion_arrays[k][3],
                            quaternion_arrays[k][0],
                            quaternion_arrays[k][1],
                            quaternion_arrays[k][2]);
        const RollPitchYawd rpy_actual = q.toRollPitchYaw();

        ASSERT_NEAR(rpy_expected.roll, rpy_actual.roll, eps);
        ASSERT_NEAR(rpy_expected.pitch, rpy_actual.pitch, eps);
        ASSERT_NEAR(rpy_expected.yaw, rpy_actual.yaw, eps);
    }
}

TEST_F(RollPitchYawTest, AxisAngleToRollPitchYaw)
{
    const double eps = 1e-6;

    for (size_t k = 0; k < 13; k++)
    {
        const RollPitchYawd rpy_expected(rpy_arrays[k][0], rpy_arrays[k][1], rpy_arrays[k][2]);
        const AxisAngled axis_angle(
            angle_arrays[k][0], axis_arrays[k][0], axis_arrays[k][1], axis_arrays[k][2]);
        const RollPitchYawd rpy_actual = axis_angle.toRollPitchYaw();

        ASSERT_NEAR(rpy_expected.roll, rpy_actual.roll, eps);
        ASSERT_NEAR(rpy_expected.pitch, rpy_actual.pitch, eps);
        ASSERT_NEAR(rpy_expected.yaw, rpy_actual.yaw, eps);
    }
}

}  // namespace
}  // namespace arl
