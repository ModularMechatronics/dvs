#ifndef MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_
#define MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>

#include "dvs/logging.h"
#include "opengl_low_level/data_structures.h"

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

struct ElementSettings
{
    float x;
    float y;
    float width;
    float height;

    int z_order;

    std::string name;

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

    ElementSettings()
    {
        x = 0.0f;
        y = 0.0f;
        width = 100.0f;
        height = 100.0f;

        z_order = -1;

        pane_radius = 10.0f;

        name = "<NO-NAME>";

        background_color = RGBTripletf(164.0f / 255.0f, 217.0f / 255.0f, 200.0f / 255.0f);
        plot_box_color = RGBTripletf(1.0f, 1.0f, 1.0f);
        grid_color = RGBTripletf(0.7f, 0.7f, 0.7f);
        axes_numbers_color = RGBTripletf(0.7f, 0.7f, 0.7f);
        axes_letters_color = RGBTripletf(0.7f, 0.7f, 0.7f);

        grid_on = true;
        plot_box_on = true;
        axes_numbers_on = true;
        axes_letters_on = true;
        clipping_on = true;
    }

    explicit ElementSettings(const nlohmann::json& j)
    {
        name = j["name"];

        x = j["x"];
        y = j["y"];
        width = j["width"];
        height = j["height"];

        width = std::min(std::max(width, 0.1f), 1.0f);
        height = std::min(std::max(height, 0.1f), 1.0f);

        x = std::max(std::min(x, 0.9f), 0.0f);
        y = std::max(std::min(y, 0.9f), 0.0f);

        background_color = (j.count("background_color") > 0)
                               ? jsonObjToColor(j["background_color"])
                               : RGBTripletf(164.0f / 255.0f, 217.0f / 255.0f, 200.0f / 255.0f);
        plot_box_color =
            (j.count("plot_box_color") > 0) ? jsonObjToColor(j["plot_box_color"]) : RGBTripletf(1.0f, 1.0f, 1.0f);
        grid_color = (j.count("grid_color") > 0) ? jsonObjToColor(j["grid_color"]) : RGBTripletf(0.7f, 0.7f, 0.7f);
        axes_numbers_color = (j.count("axes_numbers_color") > 0) ? jsonObjToColor(j["axes_numbers_color"])
                                                                 : RGBTripletf(0.0f, 0.0f, 0.0f);
        axes_letters_color = (j.count("axes_letters_color") > 0) ? jsonObjToColor(j["axes_letters_color"])
                                                                 : RGBTripletf(0.0f, 0.0f, 0.0f);
        grid_on = (j.count("grid_on") > 0) ? static_cast<bool>(j["grid_on"]) : true;
        plot_box_on = (j.count("plot_box_on") > 0) ? static_cast<bool>(j["plot_box_on"]) : true;
        axes_numbers_on = (j.count("axes_numbers_on") > 0) ? static_cast<bool>(j["axes_numbers_on"]) : true;
        axes_letters_on = (j.count("axes_letters_on") > 0) ? static_cast<bool>(j["axes_letters_on"]) : true;
        clipping_on = (j.count("clipping_on") > 0) ? static_cast<bool>(j["clipping_on"]) : true;
        pane_radius = (j.count("pane_radius") > 0) ? static_cast<float>(j["pane_radius"]) : 10.0f;

        z_order = (j.count("z_order") > 0) ? static_cast<int>(j["z_order"]) : -1;
    }

    nlohmann::json toJson() const
    {
        nlohmann::json j;

        j["name"] = name;

        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;

        j["z_order"] = z_order;

        j["background_color"] = colorToJsonObj(background_color);
        j["plot_box_color"] = colorToJsonObj(plot_box_color);
        j["grid_color"] = colorToJsonObj(grid_color);
        j["axes_numbers_color"] = colorToJsonObj(axes_numbers_color);
        j["axes_letters_color"] = colorToJsonObj(axes_letters_color);
        j["grid_on"] = grid_on;
        j["plot_box_on"] = plot_box_on;
        j["axes_numbers_on"] = axes_numbers_on;
        j["axes_letters_on"] = axes_letters_on;
        j["clipping_on"] = clipping_on;
        j["pane_radius"] = pane_radius;

        return j;
    }

    bool operator==(const ElementSettings& other) const
    {
        return (x == other.x) && (y == other.y) && (width == other.width) && (height == other.height) &&
               (name == other.name);
    }

    bool operator!=(const ElementSettings& other) const
    {
        return !(*this == other);
    }
};

struct TabSettings
{
    std::vector<ElementSettings> elements;
    std::string name;
    RGBTripletf background_color;
    RGBTripletf button_normal_color;
    RGBTripletf button_clicked_color;
    RGBTripletf button_selected_color;
    RGBTripletf button_text_color;

    TabSettings()
    {
        background_color = RGBTripletf(35.0f / 255.0f, 42.0f / 255.0f, 48.0f / 255.0f);
        button_normal_color = RGBTripletf(174.0f / 255.0f, 227.0f / 255.0f, 209.0f / 255.0f);
        button_clicked_color = RGBTripletf(174.0f / 255.0f, 227.0f / 255.0f, 209.0f / 255.0f);
        button_selected_color = RGBTripletf(164.0f / 255.0f, 217.0f / 255.0f, 199.0f / 255.0f);
        button_text_color = RGBTripletf(0.0f, 0.0f, 0.0f);
    }

    TabSettings(const nlohmann::json& j)
    {
        name = j["name"];

        for (size_t k = 0; k < j["elements"].size(); k++)
        {
            elements.emplace_back(j["elements"][k]);
        }

        background_color = j.count("background_color") > 0
                               ? jsonObjToColor(j["background_color"])
                               : RGBTripletf(35.0f / 255.0f, 42.0f / 255.0f, 48.0f / 255.0f);
        button_normal_color = j.count("button_normal_color") > 0
                                  ? jsonObjToColor(j["button_normal_color"])
                                  : RGBTripletf(174.0f / 255.0f, 227.0f / 255.0f, 209.0f / 255.0f);
        button_clicked_color = j.count("button_clicked_color") > 0
                                   ? jsonObjToColor(j["button_clicked_color"])
                                   : RGBTripletf(174.0f / 255.0f, 227.0f / 255.0f, 209.0f / 255.0f);
        button_selected_color = j.count("button_selected_color") > 0
                                    ? jsonObjToColor(j["button_selected_color"])
                                    : RGBTripletf(164.0f / 255.0f, 217.0f / 255.0f, 199.0f / 255.0f);
        button_text_color =
            j.count("button_text_color") > 0 ? jsonObjToColor(j["button_text_color"]) : RGBTripletf(0.0f, 0.0f, 0.0f);
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

        j["background_color"] = colorToJsonObj(background_color);
        j["button_normal_color"] = colorToJsonObj(button_normal_color);
        j["button_clicked_color"] = colorToJsonObj(button_clicked_color);
        j["button_selected_color"] = colorToJsonObj(button_selected_color);
        j["button_text_color"] = colorToJsonObj(button_text_color);

        return j;
    }

    bool hasElementWithName(const std::string& name) const
    {
        return std::find_if(elements.begin(), elements.end(), [&](const ElementSettings& es) -> bool {
                   return es.name == name;
               }) != elements.end();
    }

    ElementSettings getElementWithName(const std::string& name) const
    {
        DVS_ASSERT(hasElementWithName(name));
        ElementSettings res{};

        // TODO: Use find_if?
        for (const ElementSettings& e : elements)
        {
            if (e.name == name)
            {
                res = e;
                break;
            }
        }
        return res;
    }

    bool operator==(const TabSettings& other) const
    {
        if ((name != other.name) || (elements.size() != other.elements.size()))
        {
            return false;
        }

        for (size_t k = 0; k < elements.size(); k++)
        {
            if (other.hasElementWithName(elements[k].name))
            {
                const ElementSettings other_element = other.getElementWithName(elements[k].name);
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

    WindowSettings() {}
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
