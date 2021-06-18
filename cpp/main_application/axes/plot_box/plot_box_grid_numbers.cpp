#include "axes/plot_box/plot_box_grid_numbers.h"

#include <cassert>

#include "misc/misc.h"
#include "opengl_low_level/opengl_low_level.h"

void drawNumbersForYAxis(const Vectord& y_values,
                         const double x_offset,
                         const double z_offset,
                         const AxesLimits& axes_limits,
                         const CoordinateConverter& coord_converter,
                         const float width,
                         const float height)
{
    const Vec3Dd axes_center = axes_limits.getAxesCenter();
    const Vec3Dd s = axes_limits.getAxesScale();

    for (size_t k = 0; k < y_values.size(); k++)
    {
        const Vec3Dd v(x_offset / s.x, y_values(k) / s.y, z_offset / s.z);
        const Vec2Dd vv = coord_converter.modelToViewCoordinate(v);
        const std::string num_string = formatNumber(y_values(k) + axes_center.y, 3);
        putTextAt(num_string, vv.x - static_cast<double>(num_string.length()) * 13.3 / width, vv.y, width, height, 100.0f);
    }
}

void drawNumbersForZAxis(const Vectord& z_values,
                         const double x_offset,
                         const double y_offset,
                         const AxesLimits& axes_limits,
                         const CoordinateConverter& coord_converter,
                         const float width,
                         const float height)
{
    const Vec3Dd axes_center = axes_limits.getAxesCenter();
    const Vec3Dd s = axes_limits.getAxesScale();

    for (size_t k = 0; k < z_values.size(); k++)
    {
        const Vec3Dd v(x_offset / s.x, y_offset / s.y, z_values(k) / s.z);
        const Vec2Dd vv = coord_converter.modelToViewCoordinate(v);
        putTextAt(formatNumber(z_values(k) + axes_center.z, 3), vv.x, vv.y, width, height, 100.0f);
    }
}

void drawNumbersForXAxis(const Vectord& x_values,
                         const double y_offset,
                         const double z_offset,
                         const AxesLimits& axes_limits,
                         const CoordinateConverter& coord_converter,
                         const float width,
                         const float height)
{
    const Vec3Dd axes_center = axes_limits.getAxesCenter();
    const Vec3Dd s = axes_limits.getAxesScale();

    for (size_t k = 0; k < x_values.size(); k++)
    {
        const Vec3Dd v(x_values(k) / s.x, y_offset / s.y, z_offset / s.z);
        const Vec2Dd vv = coord_converter.modelToViewCoordinate(v);
        putTextAt(formatNumber(x_values(k) + axes_center.x, 3), vv.x, vv.y, width, height, 100.0f);
    }
}

void drawAxisNumbers(const GridVectors& gv,
                     const AxesLimits& axes_limits,
                     const ViewAngles& view_angles,
                     const CoordinateConverter& coord_converter,
                     const float width,
                     const float height)
{
    const Vec3Dd s = axes_limits.getAxesScale();
    const double box_x = s.x;
    const double box_y = s.y;
    const double box_z = s.z;
    setColor(0.0, 0.0, 0.0);

    bool draw_x_numbers = !view_angles.isSnappedAlongX();
    bool draw_y_numbers = !view_angles.isSnappedAlongY();
    bool draw_z_numbers = !view_angles.isSnappedAlongZ();

    double x_value_to_use_vert, z_value_to_use_vert;
    double x_value_to_use, y_value_to_use, z_value_to_use, y_value_to_use_2;
    if (view_angles.getSnappedElevation() < 0.0)
    {
        y_value_to_use = box_y / 2.0;
    }
    else
    {
        y_value_to_use = -box_y / 2.0;
    }

    // Magic numbers are for moving text away from the plot box
    if ((view_angles.getSnappedAzimuth() > -M_PI_2) && (view_angles.getSnappedAzimuth() < M_PI_2))
    {
        z_value_to_use = -1.1 * box_z / 2.0;
        y_value_to_use_2 = box_y / 2.0;
    }
    else
    {
        z_value_to_use = box_z / 2.0;
        y_value_to_use_2 = -box_y / 2.0;
    }

    if ((view_angles.getSnappedAzimuth() > 0.0) && (view_angles.getSnappedAzimuth() < M_PI))
    {
        x_value_to_use = -1.15 * box_x / 2.0;
    }
    else
    {
        x_value_to_use = 1.05 * box_x / 2.0;
    }

    if (view_angles.getSnappedElevation() > 0)
    {
        z_value_to_use_vert = -box_z / 2.0;
    }
    else
    {
        z_value_to_use_vert = box_z / 2.0;
    }

    if((view_angles.getSnappedAzimuth() > 0.0f) && (view_angles.getSnappedAzimuth() <= M_PI))
    {
        x_value_to_use_vert = -box_x / 2.0;
    }
    else
    {
        x_value_to_use_vert = box_x / 2.0;
    }

    if(draw_x_numbers)
    {
        drawNumbersForXAxis(gv.x, y_value_to_use, z_value_to_use, axes_limits, coord_converter, width, height);
    }
    if(draw_y_numbers)
    {
        drawNumbersForYAxis(
        gv.y, x_value_to_use_vert, z_value_to_use_vert, axes_limits, coord_converter, width, height);
    }
    if(draw_z_numbers)
    {
        drawNumbersForZAxis(gv.z, x_value_to_use, y_value_to_use_2, axes_limits, coord_converter, width, height);
    }
}
