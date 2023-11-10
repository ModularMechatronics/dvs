#ifndef MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_
#define MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "dvs/enumerations.h"
#include "dvs/logging.h"
#include "misc/rgb_triplet.h"
#include "project_state/helper_functions.h"

extern RGBTriplet<float> kMainWindowBackgroundColor;

struct ElementSettings
{
    float x;
    float y;
    float width;
    float height;

    std::string handle_string;

    int z_order;

    dvs::GuiElementType type;

    ElementSettings();
    explicit ElementSettings(const nlohmann::json& j);
    virtual ~ElementSettings() {}

    void parseSettings(const nlohmann::json& j);

    virtual nlohmann::json toJson() const;

    bool operator==(const ElementSettings& other) const;
    bool operator!=(const ElementSettings& other) const;
};

bool areDerivedElementEqual(const std::shared_ptr<ElementSettings>& lhs, const std::shared_ptr<ElementSettings>& rhs);

struct PlotPaneSettings : ElementSettings
{
    PlotPaneSettings();
    explicit PlotPaneSettings(const nlohmann::json& j);

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

    void parsePlotPaneSettings(const nlohmann::json& j);

    nlohmann::json toJson() const override;

    void defaultInitializeAllSettings();

    bool operator==(const PlotPaneSettings& other) const;
    bool operator!=(const PlotPaneSettings& other) const;
};

struct ButtonSettings : public ElementSettings
{
    std::string label;

    ButtonSettings();
    explicit ButtonSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const ButtonSettings& other) const;
    bool operator!=(const ButtonSettings& other) const;
};

struct CheckBoxSettings : public ElementSettings
{
    std::string label;

    CheckBoxSettings();
    explicit CheckBoxSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const CheckBoxSettings& other) const;
    bool operator!=(const CheckBoxSettings& other) const;
};

struct EditableTextSettings : public ElementSettings
{
    std::string init_value;

    EditableTextSettings();
    explicit EditableTextSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const EditableTextSettings& other) const;
    bool operator!=(const EditableTextSettings& other) const;
};

struct DropDownMenuSettings : public ElementSettings
{
    std::string initially_selected_item;
    std::vector<std::string> elements;

    DropDownMenuSettings();
    explicit DropDownMenuSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const DropDownMenuSettings& other) const;
    bool operator!=(const DropDownMenuSettings& other) const;
};

struct ListBoxSettings : public ElementSettings
{
    std::vector<std::string> elements;

    ListBoxSettings();
    explicit ListBoxSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const ListBoxSettings& other) const;
    bool operator!=(const ListBoxSettings& other) const;
};

struct RadioButtonSettings
{
    std::string label;

    RadioButtonSettings();
    explicit RadioButtonSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const;

    bool operator==(const RadioButtonSettings& other) const;
    bool operator!=(const RadioButtonSettings& other) const;
};

struct RadioButtonGroupSettings : public ElementSettings
{
    std::string label;
    std::vector<RadioButtonSettings> radio_buttons;

    RadioButtonGroupSettings();
    explicit RadioButtonGroupSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const RadioButtonGroupSettings& other) const;
    bool operator!=(const RadioButtonGroupSettings& other) const;
};

struct TextLabelSettings : public ElementSettings
{
    std::string label;

    TextLabelSettings();
    explicit TextLabelSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const TextLabelSettings& other) const;
    bool operator!=(const TextLabelSettings& other) const;
};

struct SliderSettings : public ElementSettings
{
    int min_value;
    int max_value;
    int init_value;
    int step_size;
    bool is_horizontal;

    SliderSettings();
    explicit SliderSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const SliderSettings& other) const;
    bool operator!=(const SliderSettings& other) const;
};

struct StaticBoxSettings : public ElementSettings
{
    std::string label;
    std::vector<std::shared_ptr<ElementSettings>> elements;

    StaticBoxSettings();
    explicit StaticBoxSettings(const nlohmann::json& j_data);

    nlohmann::json toJson() const override;

    bool operator==(const StaticBoxSettings& other) const;
    bool operator!=(const StaticBoxSettings& other) const;
};

struct TabSettings
{
    std::string name;
    RGBTripletf background_color;
    RGBTripletf button_normal_color;
    RGBTripletf button_clicked_color;
    RGBTripletf button_selected_color;
    RGBTripletf button_text_color;
    std::vector<std::shared_ptr<ElementSettings>> elements;

    TabSettings();
    TabSettings(const nlohmann::json& j);
    nlohmann::json toJson() const;

    bool hasElementWithHandleString(const std::string& handle_string) const;
    std::shared_ptr<ElementSettings> getElementWithHandleString(const std::string& handle_string) const;

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
