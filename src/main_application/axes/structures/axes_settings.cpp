#include "axes/structures/axes_settings.h"

AxesSettings::AxesSettings()
{
    plot_box_wall_color_ = RGBTripletf(1.0f, 1.0f, 1.0f);
    plot_box_size_factor_ = 0.5;

    grid_on_ = false;
    grid_color_ = RGBTripletf(0.7f, 0.7f, 0.7f);

    coordinate_axes_on_ = true;
    coord_axes_extension_factor_ = 1.5;

    x_label_ = "x";
    y_label_ = "y";
    z_label_ = "z";

    num_axes_ticks_ = 4;

    axes_state_ = AxesState::DEFAULT;

    min_vec_ = Vec3d(-1.0, -1.0, -1.0);
    max_vec_ = Vec3d(1.0, 1.0, 1.0);
}

Vec3d AxesSettings::getAxesScale() const
{
    return axes_scale_;
}

void AxesSettings::setAxesScale(const Vec3d& new_scale)
{
    axes_scale_ = new_scale;
}

double AxesSettings::getPlotBoxSizeFactor() const
{
    return plot_box_size_factor_;
}

Vec3d AxesSettings::getMinVec() const
{
    return min_vec_;
}

Vec3d AxesSettings::getMaxVec() const
{
    return max_vec_;
}

AxesState AxesSettings::getAxesState() const
{
    return axes_state_;
}

std::string AxesSettings::getXLabel() const
{
    return x_label_;
}

std::string AxesSettings::getYLabel() const
{
    return y_label_;
}

std::string AxesSettings::getZLabel() const
{
    return z_label_;
}

size_t AxesSettings::getNumAxesTicks() const
{
    return num_axes_ticks_;
}

RGBTripletf AxesSettings::getGridColor() const
{
    return grid_color_;
}

bool AxesSettings::getGridOn() const
{
    return grid_on_;
}

bool AxesSettings::getCoordinateAxesOn() const
{
    return coordinate_axes_on_;
}

RGBTripletf AxesSettings::getPlotBoxWallColor() const
{
    return plot_box_wall_color_;
}

double AxesSettings::getCoordAxExtFac() const
{
    return coord_axes_extension_factor_;
}
