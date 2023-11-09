#include "project_state/project_settings.h"

constexpr RGBTriplet<float> kElementBackgroundColorDefault{hexToRgbTripletf(0x8EE6DA)};
constexpr RGBTriplet<float> kPlotBoxColorDefault{hexToRgbTripletf(0xFFFFFF)};
constexpr RGBTriplet<float> kGridColorDefault{0.7f, 0.7f, 0.7f};
constexpr RGBTriplet<float> kAxesNumbersColorDefault{0.0f, 0.0f, 0.0f};
constexpr RGBTriplet<float> kAxesLettersColorDefault{0.0f, 0.0f, 0.0f};
constexpr bool kGridOnDefault{true};
constexpr bool kPlotBoxOnDefault{true};
constexpr bool kAxesNumbersOnDefault{true};
constexpr bool kAxesLettersOnDefault{true};
constexpr bool kClippingOnDefault{true};
constexpr float kPaneRadiusDefault{10.0f};

PlotPaneSettings::PlotPaneSettings()
    : ElementSettings{},
      title{"<NO-NAME>"},
      background_color{kElementBackgroundColorDefault},
      plot_box_color{kPlotBoxColorDefault},
      grid_color{kGridColorDefault},
      axes_numbers_color{kAxesNumbersColorDefault},
      axes_letters_color{kAxesLettersColorDefault},
      grid_on{kGridOnDefault},
      plot_box_on{kPlotBoxOnDefault},
      axes_numbers_on{kAxesNumbersOnDefault},
      axes_letters_on{kAxesLettersOnDefault},
      clipping_on{kClippingOnDefault},
      pane_radius{kPaneRadiusDefault},
      projection_mode{ProjectionMode::ORTHOGRAPHIC}
{
}

PlotPaneSettings::PlotPaneSettings(const nlohmann::json& j) : ElementSettings{j}
{
    parsePlotPaneAttributes(j);
}

bool PlotPaneSettings::operator==(const PlotPaneSettings& other) const
{
    return ElementSettings::operator==(other) && (title == other.title) &&
           (background_color == other.background_color) && (plot_box_color == other.plot_box_color) &&
           (grid_color == other.grid_color) && (axes_numbers_color == other.axes_numbers_color) &&
           (axes_letters_color == other.axes_letters_color) && (grid_on == other.grid_on) &&
           (plot_box_on == other.plot_box_on) && (axes_numbers_on == other.axes_numbers_on) &&
           (axes_letters_on == other.axes_letters_on) && (clipping_on == other.clipping_on) &&
           (pane_radius == other.pane_radius) && (projection_mode == other.projection_mode);
}

bool PlotPaneSettings::operator!=(const PlotPaneSettings& other) const
{
    return !(*this == other);
}

void PlotPaneSettings::parsePlotPaneAttributes(const nlohmann::json& j)
{
    background_color =
        (j.count("background_color") > 0) ? jsonObjToColor(j["background_color"]) : kElementBackgroundColorDefault;
    plot_box_color = (j.count("plot_box_color") > 0) ? jsonObjToColor(j["plot_box_color"]) : kPlotBoxColorDefault;
    grid_color = (j.count("grid_color") > 0) ? jsonObjToColor(j["grid_color"]) : kGridColorDefault;
    axes_numbers_color =
        (j.count("axes_numbers_color") > 0) ? jsonObjToColor(j["axes_numbers_color"]) : kAxesNumbersColorDefault;
    axes_letters_color =
        (j.count("axes_letters_color") > 0) ? jsonObjToColor(j["axes_letters_color"]) : kAxesLettersColorDefault;
    grid_on = (j.count("grid_on") > 0) ? static_cast<bool>(j["grid_on"]) : kGridOnDefault;
    plot_box_on = (j.count("plot_box_on") > 0) ? static_cast<bool>(j["plot_box_on"]) : kPlotBoxOnDefault;
    axes_numbers_on =
        (j.count("axes_numbers_on") > 0) ? static_cast<bool>(j["axes_numbers_on"]) : kAxesNumbersOnDefault;
    axes_letters_on =
        (j.count("axes_letters_on") > 0) ? static_cast<bool>(j["axes_letters_on"]) : kAxesLettersOnDefault;
    clipping_on = (j.count("clipping_on") > 0) ? static_cast<bool>(j["clipping_on"]) : kClippingOnDefault;
    pane_radius = (j.count("pane_radius") > 0) ? static_cast<float>(j["pane_radius"]) : kPaneRadiusDefault;

    if (j.count("projection_mode") > 0)
    {
        const std::string projection_type_str = j["projection_mode"];
        if (projection_type_str == "orthographic")
        {
            projection_mode = ProjectionMode::ORTHOGRAPHIC;
        }
        else if (projection_type_str == "perspective")
        {
            projection_mode = ProjectionMode::PERSPECTIVE;
        }
        else
        {
            throw std::runtime_error("Invalid option for \"projection_mode\": \"" + projection_type_str + "\"");
        }
    }
    else
    {
        projection_mode = ProjectionMode::ORTHOGRAPHIC;
    }

    if (j.count("title") > 0)
    {
        title = j["title"];
    }
    else
    {
        title = handle_string;
    }
}

nlohmann::json PlotPaneSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();

    if (title != handle_string)
    {
        j["title"] = title;
    }

    j["element_specific_settings"] = nlohmann::json::object();

    // if (background_color != kElementBackgroundColorDefault)
    {
        j["element_specific_settings"]["background_color"] = colorToJsonObj(background_color);
    }

    if (plot_box_color != kPlotBoxColorDefault)
    {
        j["plot_box_color"] = colorToJsonObj(plot_box_color);
    }

    if (grid_color != kGridColorDefault)
    {
        j["grid_color"] = colorToJsonObj(grid_color);
    }

    if (axes_numbers_color != kAxesNumbersColorDefault)
    {
        j["axes_numbers_color"] = colorToJsonObj(axes_numbers_color);
    }

    if (axes_letters_color != kAxesLettersColorDefault)
    {
        j["axes_letters_color"] = colorToJsonObj(axes_letters_color);
    }

    assignIfNotDefault(j, "grid_on", grid_on, kGridOnDefault);
    assignIfNotDefault(j, "plot_box_on", plot_box_on, kPlotBoxOnDefault);
    assignIfNotDefault(j, "axes_numbers_on", axes_numbers_on, kAxesNumbersOnDefault);
    assignIfNotDefault(j, "axes_letters_on", axes_letters_on, kAxesLettersOnDefault);
    assignIfNotDefault(j, "clipping_on", clipping_on, kClippingOnDefault);
    assignIfNotDefault(j, "pane_radius", pane_radius, kPaneRadiusDefault);

    if (projection_mode == ProjectionMode::ORTHOGRAPHIC)
    {
        j["projection_mode"] = "orthographic";
    }
    else if (projection_mode == ProjectionMode::PERSPECTIVE)
    {
        j["projection_mode"] = "perspective";
    }

    return j;
}
