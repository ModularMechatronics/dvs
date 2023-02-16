#ifndef MAIN_APPLICATION_AXES_AXES_SIDE_CONFIGURATION_H_
#define MAIN_APPLICATION_AXES_AXES_SIDE_CONFIGURATION_H_

#include <cmath>
#include <utility>
#include <vector>

#include "axes/structures/axes_limits.h"
#include "axes/structures/axes_settings.h"
#include "axes/structures/grid_vectors.h"
#include "axes/structures/view_angles.h"
#include "dvs/math/math.h"
#include "io_devices/io_devices.h"

using namespace dvs;

struct AxesSideConfiguration
{
    float xy_plane_z_value;
    float xz_plane_y_value;
    float yz_plane_x_value;

    AxesSideConfiguration() = default;
    AxesSideConfiguration(const ViewAngles& view_angles)
    {
        const float azimuth = view_angles.getSnappedAzimuth();
        const float elevation = view_angles.getSnappedElevation();

        // XY
        xy_plane_z_value = (elevation >= 0.0f) ? -1.0f : 1.0f;

        // XZ
        xz_plane_y_value = (((-M_PI / 2.0f) <= azimuth) && (azimuth <= (M_PI / 2.0f))) ? 1.0f : -1.0f;

        // YZ
        const bool cond = static_cast<int>(azimuth * 180.0f / M_PI) == -180;
        yz_plane_x_value = ((azimuth >= 0.0f) || cond) ? 1.0f : -1.0f;

        if (view_angles.isSnappedAlongX()) {}
        else if (view_angles.isSnappedAlongY())
        {
        }
        else if (view_angles.isSnappedAlongZ())
        {
        }
    }
};

#endif  // MAIN_APPLICATION_AXES_AXES_SIDE_CONFIGURATION_H_
