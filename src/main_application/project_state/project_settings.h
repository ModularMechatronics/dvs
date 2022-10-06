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

class SettingsBase
{
protected:
    std::vector<ElementSettings> elements_;
    std::string name_;

public:
    SettingsBase() {}
    SettingsBase(const nlohmann::json& j)
    {
        name_ = j["name"];
        for (size_t k = 0; k < j["elements"].size(); k++)
        {
            elements_.emplace_back(j["elements"][k]);
        }
    }

    void pushBackElementSettings(const ElementSettings& es)
    {
        elements_.push_back(es);
    }

    std::string getName() const
    {
        return name_;
    }

    void setName(const std::string name)
    {
        name_ = name;
    }

    std::vector<ElementSettings> getElementSettingsList() const
    {
        return elements_;
    }

    bool hasElementWithName(const std::string& name) const
    {
        return std::find_if(elements_.begin(), elements_.end(), [&](const ElementSettings& es) -> bool {
                   return es.name == name;
               }) != elements_.end();
    }

    ElementSettings getElementWithName(const std::string& name) const
    {
        assert(hasElementWithName(name));
        ElementSettings res(0.0f, 0.0f, 0.0f, 0.0f, "");
        for (const ElementSettings& e : elements_)
        {
            if (e.name == name)
            {
                res = e;
                break;
            }
        }
        return res;
    }

    bool operator==(const SettingsBase& other) const
    {
        if ((name_ != other.name_) || (elements_.size() != other.elements_.size()))
        {
            return false;
        }

        for (size_t k = 0; k < elements_.size(); k++)
        {
            if (other.hasElementWithName(elements_[k].name))
            {
                const ElementSettings other_element = other.getElementWithName(elements_[k].name);
                if (other_element != elements_[k])
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

    bool operator!=(const SettingsBase& other) const
    {
        return !(*this == other);
    }
};

class TabSettings : public SettingsBase
{
public:
    TabSettings() : SettingsBase() {}
    TabSettings(const nlohmann::json& j) : SettingsBase(j) {}
};

class WindowSettings : public SettingsBase
{
public:
    float x;
    float y;
    float width;
    float height;

    WindowSettings() {}
    WindowSettings(const nlohmann::json& j) : SettingsBase(j)
    {
        x = j["x"];
        y = j["y"];
        width = j["width"];
        height = j["height"];
    }
};

class ProjectSettings
{
private:
    std::vector<TabSettings> tabs_;
    std::vector<WindowSettings> windows_;

public:
    ProjectSettings() = default;
    ProjectSettings(const std::string& file_path)
    {
        std::ifstream input_file(file_path);
        nlohmann::json j;
        input_file >> j;
        for (size_t k = 0; k < j["tabs"].size(); k++)
        {
            tabs_.emplace_back(j["tabs"][k]);
        }

        for (size_t k = 0; k < j["windows"].size(); k++)
        {
            windows_.emplace_back(j["windows"][k]);
        }
    }

    void pushBackTabSettings(const TabSettings& ts)
    {
        tabs_.push_back(ts);
    }

    void pushBackWindowSettings(const WindowSettings& ws)
    {
        windows_.push_back(ws);
    }

    std::vector<TabSettings> getTabs() const
    {
        return tabs_;
    }

    std::vector<WindowSettings> getWindows() const
    {
        return windows_;
    }

    nlohmann::json toJson() const
    {
        nlohmann::json j_to_save;
        j_to_save["tabs"] = nlohmann::json::array();

        for (auto te : tabs_)
        {
            nlohmann::json elements = nlohmann::json::array();

            const std::vector<ElementSettings> elems = te.getElementSettingsList();
            for (const ElementSettings& elem : elems)
            {
                nlohmann::json j;
                j["x"] = elem.x;
                j["y"] = elem.y;
                j["width"] = elem.width;
                j["height"] = elem.height;
                j["name"] = elem.name;
                elements.push_back(j);
            }

            nlohmann::json tab_obj = nlohmann::json();
            tab_obj["name"] = te.getName();
            tab_obj["elements"] = elements;

            j_to_save["tabs"].push_back(tab_obj);
        }

        for (auto we : windows_)
        {
            nlohmann::json elements = nlohmann::json::array();

            const std::vector<ElementSettings> elems = we.getElementSettingsList();
            for (const ElementSettings& elem : elems)
            {
                nlohmann::json j;
                j["x"] = elem.x;
                j["y"] = elem.y;
                j["width"] = elem.width;
                j["height"] = elem.height;
                j["name"] = elem.name;
                elements.push_back(j);
            }

            nlohmann::json window_obj = nlohmann::json();
            window_obj["name"] = we.getName();
            window_obj["height"] = we.height;
            window_obj["width"] = we.width;
            window_obj["x"] = we.x;
            window_obj["y"] = we.y;
            window_obj["elements"] = elements;

            j_to_save["windows"].push_back(window_obj);
        }

        return j_to_save;
    }

    std::vector<ElementSettings> getElementSettingsList() const
    {
        std::vector<ElementSettings> all_elements;

        for (auto t : tabs_)
        {
            std::vector<ElementSettings> v = t.getElementSettingsList();
            for (auto tv : v)
            {
                all_elements.push_back(tv);
            }
        }

        return all_elements;
    }

    bool hasTabWithName(const std::string& name) const
    {
        return std::find_if(tabs_.begin(), tabs_.end(), [&](const TabSettings& ts) -> bool {
                   return ts.getName() == name;
               }) != tabs_.end();
    }

    TabSettings getTabWithName(const std::string& name) const
    {
        assert(hasTabWithName(name));
        TabSettings res;
        for (const TabSettings& ts : tabs_)
        {
            if (ts.getName() == name)
            {
                res = ts;
                break;
            }
        }
        return res;
    }

    bool hasWindowWithName(const std::string& name) const
    {
        return std::find_if(windows_.begin(), windows_.end(), [&](const WindowSettings& ws) -> bool {
                   return ws.getName() == name;
               }) != windows_.end();
    }

    WindowSettings getWindowWithName(const std::string& name) const
    {
        assert(hasWindowWithName(name));
        WindowSettings res;
        for (const WindowSettings& ws : windows_)
        {
            if (ws.getName() == name)
            {
                res = ws;
                break;
            }
        }
        return res;
    }

    bool operator==(const ProjectSettings& other) const
    {
        if (tabs_.size() != other.tabs_.size())
        {
            return false;
        }

        if (windows_.size() != other.windows_.size())
        {
            return false;
        }

        for (const TabSettings& ts : tabs_)
        {
            if (other.hasTabWithName(ts.getName()))
            {
                if (other.getTabWithName(ts.getName()) != ts)
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }

        for (const WindowSettings& ws : windows_)
        {
            if (other.hasWindowWithName(ws.getName()))
            {
                if (other.getWindowWithName(ws.getName()) != ws)
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
