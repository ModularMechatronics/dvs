#ifndef MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_
#define MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "dvs/logging.h"
#include "misc/rgb_triplet.h"

extern RGBTriplet<float> kMainWindowBackgroundColor;

enum class GuiElementType : std::uint8_t
{
    Button,
    Slider,
    CheckBox,
    EditableText,
    DropDownMenu,
    ListBox,
    RadioButtonGroup,
    TextLabel,
    StaticBox,
    PlotPane,
    Unknown
};

/*
{
    "attributes": {
        "label": "Button2"
    },
    "type": "BUTTON",
},
{
    "type": "SLIDER",
    "attributes": {
        "min": 0,
        "max": 100,
        "step_size": 1,
        "init_value": 0,
        "style": "horizontal"
    },
},
{
    "attributes": {
        "label": "Check BoxButton"
    },
    "type": "CHECK_BOX",
},
{
    "attributes": {
        "init_value": "Editable Text"
    },
    "type": "EDITABLE_TEXT",
},
{
    "type": "DROP_DOWN_MENU",
    "attributes": {
        "init_value": "Drop down menu"
    },
},
{
    "handle_string": "list_box0",
    "type": "LIST_BOX",
},
{
    "type": "RADIO_BUTTON",
    "attributes": {
        "label": "Radio Button"
    },
},
{
    "type": "TEXT_LABEL",
    "attributes": {
        "label": "Text Label"
    },
}
*/

struct ElementSettings
{
    // Base attributes
    float x;
    float y;
    float width;
    float height;

    std::string handle_string;

    int z_order;

    GuiElementType type;

    std::string label;

    // Plot pane settings
    std::string title;

    RGBTripletf background_color;
    RGBTripletf plot_box_color;
    RGBTripletf grid_color;
    RGBTripletf axes_numbers_color;
    RGBTripletf axes_letters_color;

    bool grid_on;
    bool plot_box_on;
    bool axes_numbers_on;
    bool axes_letters_on;
    bool clipping_on;

    float pane_radius;

    enum class ProjectionMode
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    ProjectionMode projection_mode;

    static GuiElementType parseType(const nlohmann::json& j);

    ElementSettings();
    explicit ElementSettings(const nlohmann::json& j);

    void parseBaseAttributes(const nlohmann::json& j);
    void parsePlotPaneAttributes(const nlohmann::json& j);

    nlohmann::json toJson() const;

    bool operator==(const ElementSettings& other) const;
    bool operator!=(const ElementSettings& other) const;
};

struct TabSettings
{
    std::string name;
    RGBTripletf background_color;
    RGBTripletf button_normal_color;
    RGBTripletf button_clicked_color;
    RGBTripletf button_selected_color;
    RGBTripletf button_text_color;
    std::vector<ElementSettings> elements;

    TabSettings();
    TabSettings(const nlohmann::json& j);
    nlohmann::json toJson() const;

    bool hasElementWithHandleString(const std::string& handle_string) const;
    ElementSettings getElementWithHandleString(const std::string& handle_string) const;

    bool operator==(const TabSettings& other) const;
    bool operator!=(const TabSettings& other) const;
};

struct WindowSettings
{
    float x;
    float y;
    float width;
    float height;
    std::string name;
    std::vector<TabSettings> tabs;

    WindowSettings();
    WindowSettings(const nlohmann::json& j);

    nlohmann::json toJson() const;

    bool hasTabWithName(const std::string& name) const;

    TabSettings getTabWithName(const std::string& name) const;

    bool operator==(const WindowSettings& other) const;
    bool operator!=(const WindowSettings& other) const;
};

class ProjectSettings
{
private:
    std::vector<WindowSettings> windows_;

public:
    ProjectSettings() = default;
    ProjectSettings(const std::string& file_path);

    void pushBackWindowSettings(const WindowSettings& ws);

    std::vector<WindowSettings> getWindows() const;

    nlohmann::json toJson() const;

    bool hasWindowWithName(const std::string& name) const;
    WindowSettings getWindowWithName(const std::string& name) const;

    bool operator==(const ProjectSettings& other) const;
    bool operator!=(const ProjectSettings& other) const;
};

#endif  // MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_
