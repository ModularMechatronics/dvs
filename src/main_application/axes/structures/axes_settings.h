#ifndef MAIN_APPLICATION_AXES_STRUCTURES_AXES_SETTINGS_H_
#define MAIN_APPLICATION_AXES_STRUCTURES_AXES_SETTINGS_H_

#include <unistd.h>

#include <string>

#include "dvs/math/math.h"
#include "opengl_low_level/opengl_low_level.h"

using namespace dvs;

enum class AxesState
{
    DEFAULT,
    EQUAL,
    SQUARE
};

class AxesSettings
{
private:
    RGBTripletf plot_box_wall_color_;

    Vec3d axes_scale_;

    bool grid_on_;
    RGBTripletf grid_color_;

    bool coordinate_axes_on_;
    double coord_axes_extension_factor_;

    std::string x_label_;
    std::string y_label_;
    std::string z_label_;

    size_t num_axes_ticks_;

    AxesState axes_state_;

    Vec3d min_vec_;
    Vec3d max_vec_;

public:
    AxesSettings();

    AxesState getAxesState() const;
    std::string getXLabel() const;
    std::string getYLabel() const;
    std::string getZLabel() const;
    size_t getNumAxesTicks() const;
    RGBTripletf getGridColor() const;
    bool getGridOn() const;
    bool getCoordinateAxesOn() const;
    RGBTripletf getPlotBoxWallColor() const;
    double getCoordAxExtFac() const;
    Vec3d getMinVec() const;
    Vec3d getMaxVec() const;

    Vec3d getAxesScale() const;
    void setAxesScale(const Vec3d& new_scale);
};

#endif // MAIN_APPLICATION_AXES_STRUCTURES_AXES_SETTINGS_H_
