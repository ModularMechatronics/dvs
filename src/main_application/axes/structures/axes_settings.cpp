#include "axes/structures/axes_settings.h"

AxesSettings::AxesSettings()
{
    // Dark
    plot_pane_background_color = RGBTripletf(58.0f / 255.0f, 63.0f / 255.0f, 66.0f / 255.0f);
    tab_background_color = RGBTripletf(35.0f / 255.0f, 42.0f / 255.0f, 48.0f / 255.0f);
    plot_box_wall_color = RGBTripletf(44.0f / 255.0f, 45.0f / 255.0f, 108.0f / 255.0f);
    grid_color = RGBTripletf(1.0f, 1.0f, 1.0f);

    // Light
    plot_pane_background_color = RGBTripletf(164.0f / 255.0f, 217.0f / 255.0f, 200.0f / 255.0f);
    tab_background_color = RGBTripletf(174.0f / 255.0f, 227.0f / 255.0f, 209.0f / 255.0f);
    plot_box_wall_color = RGBTripletf(1.0f, 1.0f, 1.0f);
    grid_color = RGBTripletf(0.7f, 0.7f, 0.7f);

    grid_on = false;

    x_label = "x";
    y_label = "y";
    z_label = "z";

    num_axes_ticks = 4;
}
