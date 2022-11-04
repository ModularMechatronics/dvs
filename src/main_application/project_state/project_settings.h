#ifndef MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_
#define MAIN_APPLICATION_PROJECT_STATE_PROJECT_SETTINGS_H_

#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>

inline void throwIfMissing(const nlohmann::json& j, const std::string& field_name, const std::string& exception_string)
{
    if (j.count(field_name) == 0)
    {
        throw std::runtime_error(exception_string);
    }
}

struct ElementSettings
{
    float x;
    float y;
    float width;
    float height;

    std::string name;

    ElementSettings() = default;
    ElementSettings(const float x_, const float y_, const float width_, const float height_, const std::string& name_)
        : x(x_), y(y_), width(width_), height(height_), name(name_)
    {
    }

    explicit ElementSettings(const nlohmann::json& j)
    {
        name = j["name"];

        x = j["x"];
        y = j["y"];
        width = j["width"];
        height = j["height"];
    }

    nlohmann::json toJson() const
    {
        nlohmann::json j;

        j["name"] = name;

        j["x"] = x;
        j["y"] = y;
        j["width"] = width;
        j["height"] = height;

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

    TabSettings() = default;
    TabSettings(const nlohmann::json& j)
    {
        name = j["name"];

        for (size_t k = 0; k < j["elements"].size(); k++)
        {
            elements.emplace_back(j["elements"][k]);
        }
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
        assert(hasElementWithName(name));
        ElementSettings res(0.0f, 0.0f, 0.0f, 0.0f, "");

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
        assert(hasTabWithName(name));
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

    std::vector<ElementSettings> getElementSettingsList() const
    {
        return std::vector<ElementSettings>();
        // return elements_;
    }

    bool hasElementWithName(const std::string& name) const
    {
        return false;
        /*return std::find_if(elements_.begin(), elements_.end(), [&](const ElementSettings& es) -> bool {
                   return es.name == name;
               }) != elements_.end();*/
    }

    ElementSettings getElementWithName(const std::string& name) const
    {
        assert(hasElementWithName(name));
        ElementSettings res(0.0f, 0.0f, 0.0f, 0.0f, "");
        /*for (const ElementSettings& e : elements_)
        {
            if (e.name == name)
            {
                res = e;
                break;
            }
        }*/
        return res;
    }

    bool operator==(const WindowSettings& other) const
    {
        if ((name != other.name) || (tabs.size() != other.tabs.size()))
        {
            return false;
        }

        // TODO: Use find_if?
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

    std::vector<ElementSettings> getElementSettingsList() const
    {
        std::vector<ElementSettings> all_elements;

        /*for (auto t : tabs_)
        {
            std::vector<ElementSettings> v = t.getElementSettingsList();
            for (auto tv : v)
            {
                all_elements.push_back(tv);
            }
        }*/

        return all_elements;
    }

    bool hasWindowWithName(const std::string& name) const
    {
        return std::find_if(windows_.begin(), windows_.end(), [&](const WindowSettings& ws) -> bool {
                   return ws.name == name;
               }) != windows_.end();
    }

    WindowSettings getWindowWithName(const std::string& name) const
    {
        assert(hasWindowWithName(name));
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
