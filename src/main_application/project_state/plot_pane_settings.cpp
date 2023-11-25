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
    x = 0.0;
    y = 0.0;
    width = 0.4;
    height = 0.4;
    type = dvs::GuiElementType::PlotPane;
}

void PlotPaneSettings::defaultInitializeAllSettings()
{
    title = "";
    background_color = kElementBackgroundColorDefault;
    plot_box_color = kPlotBoxColorDefault;
    grid_color = kGridColorDefault;
    axes_numbers_color = kAxesNumbersColorDefault;
    axes_letters_color = kAxesLettersColorDefault;
    grid_on = kGridOnDefault;
    plot_box_on = kPlotBoxOnDefault;
    axes_numbers_on = kAxesNumbersOnDefault;
    axes_letters_on = kAxesLettersOnDefault;
    clipping_on = kClippingOnDefault;
    pane_radius = kPaneRadiusDefault;
    projection_mode = ProjectionMode::ORTHOGRAPHIC;
}

PlotPaneSettings::PlotPaneSettings(const nlohmann::json& j) : ElementSettings{j}
{
    defaultInitializeAllSettings();
    parsePlotPaneSettings(j);
}

bool PlotPaneSettings::operator==(const PlotPaneSettings& other) const
{
    return ElementSettings::operator==(other) && title == other.title && background_color == other.background_color &&
           plot_box_color == other.plot_box_color && grid_color == other.grid_color &&
           axes_numbers_color == other.axes_numbers_color && axes_letters_color == other.axes_letters_color &&
           grid_on == other.grid_on && plot_box_on == other.plot_box_on && axes_numbers_on == other.axes_numbers_on &&
           axes_letters_on == other.axes_letters_on && clipping_on == other.clipping_on &&
           pane_radius == other.pane_radius && projection_mode == other.projection_mode;
}

bool PlotPaneSettings::operator!=(const PlotPaneSettings& other) const
{
    return !(*this == other);
}

void PlotPaneSettings::parsePlotPaneSettings(const nlohmann::json& j)
{
    if (!j.contains("element_specific_settings"))
    {
        return;
    }

    nlohmann::json j_ess = j["element_specific_settings"];

    background_color =
        j_ess.contains("background_color") ? jsonObjToColor(j_ess["background_color"]) : kElementBackgroundColorDefault;
    plot_box_color = j_ess.contains("plot_box_color") ? jsonObjToColor(j_ess["plot_box_color"]) : kPlotBoxColorDefault;
    grid_color = j_ess.contains("grid_color") ? jsonObjToColor(j_ess["grid_color"]) : kGridColorDefault;
    axes_numbers_color =
        j_ess.contains("axes_numbers_color") ? jsonObjToColor(j_ess["axes_numbers_color"]) : kAxesNumbersColorDefault;
    axes_letters_color =
        j_ess.contains("axes_letters_color") ? jsonObjToColor(j_ess["axes_letters_color"]) : kAxesLettersColorDefault;
    grid_on = j_ess.contains("grid_on") ? static_cast<bool>(j_ess["grid_on"]) : kGridOnDefault;
    plot_box_on = j_ess.contains("plot_box_on") ? static_cast<bool>(j_ess["plot_box_on"]) : kPlotBoxOnDefault;
    axes_numbers_on =
        j_ess.contains("axes_numbers_on") ? static_cast<bool>(j_ess["axes_numbers_on"]) : kAxesNumbersOnDefault;
    axes_letters_on =
        j_ess.contains("axes_letters_on") ? static_cast<bool>(j_ess["axes_letters_on"]) : kAxesLettersOnDefault;
    clipping_on = j_ess.contains("clipping_on") ? static_cast<bool>(j_ess["clipping_on"]) : kClippingOnDefault;
    pane_radius = j_ess.contains("pane_radius") ? static_cast<float>(j_ess["pane_radius"]) : kPaneRadiusDefault;

    if (j_ess.contains("projection_mode"))
    {
        const std::string projection_type_str = j_ess["projection_mode"];
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

    if (j_ess.contains("title"))
    {
        title = j_ess["title"];
    }
    else
    {
        title = "";
    }
}

nlohmann::json PlotPaneSettings::toJson() const
{
    nlohmann::json j = ElementSettings::toJson();

    if (title != "")
    {
        j["element_specific_settings"]["title"] = title;
    }

    if (background_color != kElementBackgroundColorDefault)
    {
        j["element_specific_settings"]["background_color"] = colorToJsonObj(background_color);
    }

    if (plot_box_color != kPlotBoxColorDefault)
    {
        j["element_specific_settings"]["plot_box_color"] = colorToJsonObj(plot_box_color);
    }

    if (grid_color != kGridColorDefault)
    {
        j["element_specific_settings"]["grid_color"] = colorToJsonObj(grid_color);
    }

    if (axes_numbers_color != kAxesNumbersColorDefault)
    {
        j["element_specific_settings"]["axes_numbers_color"] = colorToJsonObj(axes_numbers_color);
    }

    if (axes_letters_color != kAxesLettersColorDefault)
    {
        j["element_specific_settings"]["axes_letters_color"] = colorToJsonObj(axes_letters_color);
    }

    assignIfNotDefault(j, "element_specific_settings", "grid_on", grid_on, kGridOnDefault);
    assignIfNotDefault(j, "element_specific_settings", "plot_box_on", plot_box_on, kPlotBoxOnDefault);
    assignIfNotDefault(j, "element_specific_settings", "axes_numbers_on", axes_numbers_on, kAxesNumbersOnDefault);
    assignIfNotDefault(j, "element_specific_settings", "axes_letters_on", axes_letters_on, kAxesLettersOnDefault);
    assignIfNotDefault(j, "element_specific_settings", "clipping_on", clipping_on, kClippingOnDefault);
    assignIfNotDefault(j, "element_specific_settings", "pane_radius", pane_radius, kPaneRadiusDefault);

    if (projection_mode == ProjectionMode::PERSPECTIVE)
    {
        j["element_specific_settings"]["projection_mode"] = "perspective";
    }

    return j;
}
