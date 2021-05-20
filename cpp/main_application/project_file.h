#ifndef PROJECT_FILE_H_
#define PROJECT_FILE_H_

#include <fstream>
#include <stdexcept>
#include <memory>

#include <nlohmann/json.hpp>

namespace project_file
{

inline void throwIfMissing(const nlohmann::json& j, const std::string& field_name, const std::string& exception_string)
{
    if(j.count(field_name) == 0)
    {
        throw std::runtime_error(exception_string);
    }
}

enum class GuiElementType
{
    PLOT,
    IMAGE,
    TEXT,
    BUTTON
};

enum class ElementType
{
    BASE,
    ROW,
    COLUMN,
    ELEMENT
};

enum class TabType
{
    GRID,
    FREEFORM
};

constexpr char* grid_str = "grid";
constexpr char* freeform_str = "freeform";

class ElementSettings
{
public:
    float x;
    float y;
    float width;
    float height;

    std::string name;

    ElementSettings() = default;
    ElementSettings(const float x_, const float y_, const float width_, const float height_, const std::string& name_) 
        : x(x_), y(y_), width(width_), height(height_), name(name_)
    {}

    ElementSettings(const nlohmann::json& j)
    {
        name = j["name"];

        x = j["x"];
        y = j["y"];
        width = j["width"];
        height = j["height"];
    }

    bool operator==(const ElementSettings& other) const
    {
        return (x == other.x) && 
               (y == other.y) && 
               (width == other.width) && 
               (height == other.height) && 
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
        for(size_t k = 0; k < j["elements"].size(); k++)
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
        for(const ElementSettings& e : elements_)
        {
            if(e.name == name)
            {
                return true;
            }
        }
        return false;
    }

    ElementSettings getElementWithName(const std::string& name) const
    {
        ElementSettings res(0.0f, 0.0f, 0.0f, 0.0f, "");
        for(const ElementSettings& e : elements_)
        {
            if(e.name == name)
            {
                res = e;
                break;
            }
        }
        return res;
    }

    bool operator==(const SettingsBase& other) const
    {
        if((name_ != other.name_) || (elements_.size() != other.elements_.size()))
        {
            return false;
        }

        for(size_t k = 0; k < elements_.size(); k++)
        {
            if(other.hasElementWithName(elements_[k].name))
            {
                const ElementSettings other_element = other.getElementWithName(elements_[k].name);
                if(other_element != elements_[k])
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

class ProjectFile
{
private:
    std::string file_path_;
    std::vector<TabSettings> tabs_;
    std::vector<WindowSettings> windows_;

public:
    ProjectFile() = default;
    ProjectFile(const std::string& file_path) : file_path_(file_path)
    {
        std::ifstream input_file(file_path);
        nlohmann::json j;
        input_file >> j;
        for(size_t k = 0; k < j["tabs"].size(); k++)
        {
            tabs_.emplace_back(j["tabs"][k]);
        }

        for(size_t k = 0; k < j["windows"].size(); k++)
        {
            windows_.emplace_back(j["windows"][k]);
        }
    }

    void pushBackTabSettings(const TabSettings& ts)
    {
        tabs_.push_back(ts);
    }

    TabSettings getTabFromIdx(const size_t idx) const
    {
        return tabs_.at(idx);
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

        for(auto te : tabs_)
        {
            nlohmann::json elements = nlohmann::json::array();

            const std::vector<ElementSettings> elems = te.getElementSettingsList();
            for(const ElementSettings& elem : elems)
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

        return j_to_save;
    }

    std::vector<ElementSettings> getElementSettingsList() const
    {
        std::vector<ElementSettings> all_elements;

        for(auto t : tabs_)
        {
            std::vector<ElementSettings> v = t.getElementSettingsList();
            for(auto tv : v)
            {
                all_elements.push_back(tv);
            }
        }

        return all_elements;
    }

    bool hasTabWithName(const std::string& name) const
    {
        for(const TabSettings& t : tabs_)
        {
            if(t.getName() == name)
            {
                return true;
            }
        }
        return false;
    }

    TabSettings getTabWithName(const std::string& name) const
    {
        TabSettings res;
        for(const TabSettings& ts : tabs_)
        {
            if(ts.getName() == name)
            {
                res = ts;
                break;
            }
        }
        return res;
    }

    bool operator==(const ProjectFile& other) const
    {
        if(tabs_.size() != other.tabs_.size())
        {
            return false;
        }

        for(const TabSettings ts : tabs_)
        {
            if(other.hasTabWithName(ts.getName()))
            {
                if(other.getTabWithName(ts.getName()) != ts)
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

    bool operator!=(const ProjectFile& other) const
    {
        return !(*this == other);
    }

};

}

#endif
