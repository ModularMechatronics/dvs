#include "axes/structures/axes_settings.h"

AxesSettings::AxesSettings()
{
    // Dark
    plot_pane_background_ = RGBTripletf(58.0f / 255.0f, 63.0f / 255.0f, 66.0f / 255.0f);
    window_background_ = RGBTripletf(35.0f / 255.0f, 42.0f / 255.0f, 48.0f / 255.0f);
    plot_box_wall_color_ = RGBTripletf(44.0f / 255.0f, 45.0f / 255.0f, 108.0f / 255.0f);
    grid_color_ = RGBTripletf(1.0f, 1.0f, 1.0f);

    // Light
    plot_pane_background_ = RGBTripletf(164.0f / 255.0f, 217.0f / 255.0f, 200.0f / 255.0f);
    window_background_ = RGBTripletf(174.0f / 255.0f, 227.0f / 255.0f, 209.0f / 255.0f);
    plot_box_wall_color_ = RGBTripletf(1.0f, 1.0f, 1.0f);
    grid_color_ = RGBTripletf(0.7f, 0.7f, 0.7f);

    grid_on_ = false;

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
