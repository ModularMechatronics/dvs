#ifndef MAIN_APPLICATION_AXES_STRUCTURES_AXES_SETTINGS_H_
#define MAIN_APPLICATION_AXES_STRUCTURES_AXES_SETTINGS_H_

#include <unistd.h>

#include <string>

#include "dvs/math/math.h"
#include "opengl_low_level/opengl_low_level.h"

using namespace dvs;

struct AxesSettings
{
public:
    RGBTripletf plot_pane_background_color;
    RGBTripletf tab_background_color;
    RGBTripletf tab_button_color;
    RGBTripletf plot_box_wall_color;
    RGBTripletf grid_color;
    RGBTripletf axes_numbers_color;
    RGBTripletf axes_letters_color;

    bool grid_on;

    std::string x_label;
    std::string y_label;
    std::string z_label;

    size_t num_axes_ticks;

    AxesSettings();
};

#endif  // MAIN_APPLICATION_AXES_STRUCTURES_AXES_SETTINGS_H_
