#ifndef MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_
#define MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "dvs/logging.h"
#include "misc/color_map.h"

enum class GuiElementType : std::uint8_t
{
    Button,
    Slider,
    CheckBox,
    EditableText,
    DropDownMenu,
    ListBox,
    RadioButton,
    RadioButtonGroup,
    TextLabel,
    StaticBox,
    PlotPane,
    Unknown
};

constexpr RGBTriplet<float> kElementBackgroundColorDefault{0.543f, 0.751f, 0.684f};
constexpr RGBTriplet<float> kPlotBoxColorDefault{1.0f, 1.0f, 1.0f};
constexpr RGBTriplet<float> kGridColorDefault{0.7f, 0.7f, 0.7f};
constexpr RGBTriplet<float> kAxesNumbersColorDefault{0.0f, 0.0f, 0.0f};
constexpr RGBTriplet<float> kAxesLettersColorDefault{0.0f, 0.0f, 0.0f};
constexpr bool kGridOnDefault{true};
constexpr bool kPlotBoxOnDefault{true};
constexpr bool kAxesNumbersOnDefault{true};
constexpr bool kAxesLettersOnDefault{true};
constexpr bool kClippingOnDefault{true};
constexpr float kPaneRadiusDefault{10.0f};
constexpr int kZOrderDefault{-1};

constexpr RGBTriplet<float> kMainWindowBackgroundColor{0.643f, 0.851f, 0.784f};
constexpr RGBTriplet<float> kTabBackgroundColorDefault{0.643f, 0.851f, 0.784f};
constexpr RGBTriplet<float> kButtonNormalColorDefault{0.682f, 0.890f, 0.820f};
constexpr RGBTriplet<float> kButtonClickedColorDefault{0.682f, 0.890f, 0.820f};
constexpr RGBTriplet<float> kButtonSelectedColorDefault{0.643f, 0.851f, 0.780f};
constexpr RGBTriplet<float> kButtonTextColorDefault{0.0f, 0.0f, 0.0f};

inline void throwIfMissing(const nlohmann::json& j, const std::string& field_name, const std::string& exception_string)
{
    if (j.count(field_name) == 0)
    {
        throw std::runtime_error(exception_string);
    }
}

inline RGBTripletf jsonObjToColor(const nlohmann::json& j)
{
    return RGBTripletf{j["r"], j["g"], j["b"]};
}

inline nlohmann::json colorToJsonObj(const RGBTripletf& c)
{
    nlohmann::json j;
    j["r"] = c.red;
    j["g"] = c.green;
    j["b"] = c.blue;
    return j;
}

template <typename T>
void assignIfNotDefault(nlohmann::json& j, const std::string& key, const T& val, const T& default_value)
{
    if (val != default_value)
    {
        j[key] = val;
    }
}

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

    GuiElementType parseType(const nlohmann::json& j)
    {
        const std::string type_string = j["type"];

        if (type_string == "BUTTON")
        {
            return GuiElementType::Button;
        }
        else if (type_string == "SLIDER")
        {
            return GuiElementType::Slider;
        }
        else if (type_string == "CHECKBOX")
        {
            return GuiElementType::CheckBox;
        }
        else if (type_string == "EDITABLE_TEXT")
        {
            return GuiElementType::EditableText;
        }
        else if (type_string == "DROPDOWN_MENU")
        {
            return GuiElementType::DropDownMenu;
        }
        else if (type_string == "LISTBOX")
        {
            return GuiElementType::ListBox;
        }
        else if (type_string == "RADIO_BUTTON")
        {
            return GuiElementType::RadioButton;
        }
        else if (type_string == "RADIO_BUTTON_GROUP")
        {
            return GuiElementType::RadioButtonGroup;
        }
        else if (type_string == "TEXT_LABEL")
        {
            return GuiElementType::TextLabel;
        }
        else if (type_string == "STATIC_BOX")
        {
            return GuiElementType::StaticBox;
        }
        else if (type_string == "PLOT_PANE")
        {
            return GuiElementType::PlotPane;
        }
        else
        {
            return GuiElementType::Unknown;
        }
    }

    ElementSettings()
        : x{0.0f},
          y{0.0f},
          width{100.0f},
          height{100.0f},
          handle_string{"<NO-NAME>"},
          z_order{kZOrderDefault},
          type{GuiElementType::Unknown},
          label{"<NO-NAME>"},
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

    void parseBaseAttributes(const nlohmann::json& j)
    {
        handle_string = j["handle_string"];

        x = j["x"];
        y = j["y"];
        width = j["width"];
        height = j["height"];

        width = std::min(std::max(width, 0.1f), 1.0f);
        height = std::min(std::max(height, 0.1f), 1.0f);

        x = std::max(std::min(x, 0.9f), 0.0f);
        y = std::max(std::min(y, 0.9f), 0.0f);

        z_order = (j.count("z_order") > 0) ? static_cast<int>(j["z_order"]) : kZOrderDefault;

        type = parseType(j);
    }

    void parsePlotPaneAttributes(const nlohmann::json& j)
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

    }

    explicit ElementSettings(const nlohmann::json& j)
    {
        parseBaseAttributes(j);
        parsePlotPaneAttributes(j);

        if(j.count("title") > 0)
        {
            title = j["title"];
        }
        else
        {
            title = handle_string;
        }
    }

    nlohmann::json toJson() const
    {
        nlohmann::json j;

        j["handle_string"] = handle_string;
        if(title != handle_string)
        {
            j["title"] = title;
        }

        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;

        if (background_color != kElementBackgroundColorDefault)
        {
            j["background_color"] = colorToJsonObj(background_color);
        }

        if (background_color != kPlotBoxColorDefault)
        {
            j["plot_box_color"] = colorToJsonObj(plot_box_color);
        }

        if (background_color != kGridColorDefault)
        {
            j["grid_color"] = colorToJsonObj(grid_color);
        }

        if (background_color != kAxesNumbersColorDefault)
        {
            j["axes_numbers_color"] = colorToJsonObj(axes_numbers_color);
        }

        if (background_color != kAxesLettersColorDefault)
        {
            j["axes_letters_color"] = colorToJsonObj(axes_letters_color);
        }

        assignIfNotDefault(j, "grid_on", grid_on, kGridOnDefault);
        assignIfNotDefault(j, "plot_box_on", plot_box_on, kPlotBoxOnDefault);
        assignIfNotDefault(j, "axes_numbers_on", axes_numbers_on, kAxesNumbersOnDefault);
        assignIfNotDefault(j, "axes_letters_on", axes_letters_on, kAxesLettersOnDefault);
        assignIfNotDefault(j, "clipping_on", clipping_on, kClippingOnDefault);
        assignIfNotDefault(j, "pane_radius", pane_radius, kPaneRadiusDefault);
        assignIfNotDefault(j, "z_order", z_order, kZOrderDefault);

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

    bool operator==(const ElementSettings& other) const
    {
        return (x == other.x) && (y == other.y) && (width == other.width) && (height == other.height) &&
               (handle_string == other.handle_string) && (title == other.title) && (background_color == other.background_color) &&
               (plot_box_color == other.plot_box_color) && (grid_color == other.grid_color) &&
               (axes_numbers_color == other.axes_numbers_color) && (axes_letters_color == other.axes_letters_color) &&
               (grid_on == other.grid_on) && (plot_box_on == other.plot_box_on) &&
               (axes_numbers_on == other.axes_numbers_on) && (axes_letters_on == other.axes_letters_on) &&
               (clipping_on == other.clipping_on) && (pane_radius == other.pane_radius) && (z_order == other.z_order) &&
               (projection_mode == other.projection_mode);
    }

    bool operator!=(const ElementSettings& other) const
    {
        return !(*this == other);
    }
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

    TabSettings()
        : name{""},
          background_color{kTabBackgroundColorDefault},
          button_normal_color{kButtonNormalColorDefault},
          button_clicked_color{kButtonClickedColorDefault},
          button_selected_color{kButtonSelectedColorDefault},
          button_text_color{kButtonTextColorDefault}
    {
    }

    TabSettings(const nlohmann::json& j)
    {
        name = j["name"];

        for (size_t k = 0; k < j["elements"].size(); k++)
        {
            elements.emplace_back(j["elements"][k]);
        }

        background_color =
            j.count("background_color") > 0 ? jsonObjToColor(j["background_color"]) : kTabBackgroundColorDefault;
        button_normal_color =
            j.count("button_normal_color") > 0 ? jsonObjToColor(j["button_normal_color"]) : kButtonNormalColorDefault;
        button_clicked_color = j.count("button_clicked_color") > 0 ? jsonObjToColor(j["button_clicked_color"])
                                                                   : kButtonClickedColorDefault;
        button_selected_color = j.count("button_selected_color") > 0 ? jsonObjToColor(j["button_selected_color"])
                                                                     : kButtonSelectedColorDefault;
        button_text_color =
            j.count("button_text_color") > 0 ? jsonObjToColor(j["button_text_color"]) : kButtonTextColorDefault;
    }

    nlohmann::json toJson() const
    {
        nlohmann::json json_elements = nlohmann::json::array();

        for (const auto& e : elements)
        {
            json_elements.push_back(e.toJson());
        }

        nlohmann::json j;
        j["name"] = name;
        j["elements"] = json_elements;

        if (background_color != kTabBackgroundColorDefault)
        {
            j["background_color"] = colorToJsonObj(background_color);
        }

        if (background_color != kButtonNormalColorDefault)
        {
            j["button_normal_color"] = colorToJsonObj(button_normal_color);
        }

        if (background_color != kButtonClickedColorDefault)
        {
            j["button_clicked_color"] = colorToJsonObj(button_clicked_color);
        }

        if (background_color != kButtonSelectedColorDefault)
        {
            j["button_selected_color"] = colorToJsonObj(button_selected_color);
        }

        if (background_color != kButtonTextColorDefault)
        {
            j["button_text_color"] = colorToJsonObj(button_text_color);
        }

        return j;
    }

    bool hasElementWithHandleString(const std::string& handle_string) const
    {
        return std::find_if(elements.begin(), elements.end(), [&](const ElementSettings& es) -> bool {
                   return es.handle_string == handle_string;
               }) != elements.end();
    }

    ElementSettings getElementWithHandleString(const std::string& handle_string) const
    {
        DVS_ASSERT(hasElementWithHandleString(handle_string));

        const auto q = std::find_if(
            elements.begin(), elements.end(), [&handle_string](const ElementSettings& e) -> bool { return e.handle_string == handle_string; });

        return *q;
    }

    bool operator==(const TabSettings& other) const
    {
        if ((name != other.name) || (elements.size() != other.elements.size()))
        {
            return false;
        }

        if ((background_color != other.background_color) || (button_normal_color != other.button_normal_color) ||
            (button_clicked_color != other.button_clicked_color) ||
            (button_selected_color != other.button_selected_color) || (button_text_color != other.button_text_color))
        {
            return false;
        }

        for (size_t k = 0; k < elements.size(); k++)
        {
            if (other.hasElementWithHandleString(elements[k].handle_string))
            {
                const ElementSettings other_element = other.getElementWithHandleString(elements[k].handle_string);
                if (other_element != elements[k])
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const TabSettings& other) const
    {
        return !(*this == other);
    }
};

struct WindowSettings
{
    float x;
    float y;
    float width;
    float height;
    std::string name;
    std::vector<TabSettings> tabs;

    WindowSettings() : x{0.0f}, y{0.0f}, width{0.0f}, height{0.0f}, name{""} {}

    WindowSettings(const nlohmann::json& j)
    {
        x = j["x"];
        y = j["y"];
        width = j["width"];
        height = j["height"];

        name = j["name"];

        for (size_t k = 0; k < j["tabs"].size(); k++)
        {
            tabs.emplace_back(j["tabs"][k]);
        }
    }

    nlohmann::json toJson() const
    {
        nlohmann::json json_tabs = nlohmann::json::array();

        for (const auto& t : tabs)
        {
            json_tabs.push_back(t.toJson());
        }

        nlohmann::json j;
        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;
        j["name"] = name;
        j["tabs"] = json_tabs;

        return j;
    }

    bool hasTabWithName(const std::string& name) const
    {
        return std::find_if(tabs.begin(), tabs.end(), [&](const TabSettings& ts) -> bool { return ts.name == name; }) !=
               tabs.end();
    }

    TabSettings getTabWithName(const std::string& name) const
    {
        DVS_ASSERT(hasTabWithName(name));
        TabSettings res{};

        // TODO: Use find_if?
        for (const TabSettings& t : tabs)
        {
            if (t.name == name)
            {
                res = t;
                break;
            }
        }
        return res;
    }

    bool operator==(const WindowSettings& other) const
    {
        if ((name != other.name) || (tabs.size() != other.tabs.size()))
        {
            return false;
        }

        if ((x != other.x) || (y != other.y) || (width != other.width) || (height != other.height))
        {
            return false;
        }

        for (size_t k = 0; k < tabs.size(); k++)
        {
            if (other.hasTabWithName(tabs[k].name))
            {
                const TabSettings other_element = other.getTabWithName(tabs[k].name);
                if (other_element != tabs[k])
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const WindowSettings& other) const
    {
        return !(*this == other);
    }
};

class ProjectSettings
{
private:
    std::vector<WindowSettings> windows_;

public:
    ProjectSettings() = default;
    ProjectSettings(const std::string& file_path)
    {
        std::ifstream input_file(file_path);
        nlohmann::json j;
        try
        {
            input_file >> j;

            for (size_t k = 0; k < j["windows"].size(); k++)
            {
                windows_.emplace_back(j["windows"][k]);
            }
        }
        catch (const std::exception& e)
        {
            std::cerr << "Exception thrown when reading file " << file_path << ": " << e.what() << std::endl;
        }
    }

    void pushBackWindowSettings(const WindowSettings& ws)
    {
        windows_.push_back(ws);
    }

    std::vector<WindowSettings> getWindows() const
    {
        return windows_;
    }

    nlohmann::json toJson() const
    {
        nlohmann::json j_to_save;
        j_to_save["windows"] = nlohmann::json::array();

        for (auto we : windows_)
        {
            j_to_save["windows"].push_back(we.toJson());
        }

        return j_to_save;
    }

    bool hasWindowWithName(const std::string& name) const
    {
        return std::find_if(windows_.begin(), windows_.end(), [&](const WindowSettings& ws) -> bool {
                   return ws.name == name;
               }) != windows_.end();
    }

    WindowSettings getWindowWithName(const std::string& name) const
    {
        DVS_ASSERT(hasWindowWithName(name));
        WindowSettings res;
        for (const WindowSettings& ws : windows_)
        {
            if (ws.name == name)
            {
                res = ws;
                break;
            }
        }
        return res;
    }

    bool operator==(const ProjectSettings& other) const
    {
        if (windows_.size() != other.windows_.size())
        {
            return false;
        }

        for (const WindowSettings& ws : windows_)
        {
            if (other.hasWindowWithName(ws.name))
            {
                if (other.getWindowWithName(ws.name) != ws)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const ProjectSettings& other) const
    {
        return !(*this == other);
    }
};

#endif  // MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_
