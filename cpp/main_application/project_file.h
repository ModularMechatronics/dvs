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

    /*bool operator==(const ElementSettings& other)
    {
        return (x == other.x) && 
            (y == other.y) && 
            (width == other.width) && 
            (height == other.height) && 
            (name == other.name);
    }

    bool operator!=(const ElementSettings& other)
    {
        return !(*this == other);
    }*/
};

class TabSettings
{
private:
    std::vector<ElementSettings> elements_;

    std::string name_;

public:

    TabSettings() = default;
    TabSettings(const nlohmann::json& j)
    {
        name_ = j["name"];
        for(size_t k = 0; k < j["elements"].size(); k++)
        {
            elements_.emplace_back(j["elements"][k]);
        }
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

    bool operator==(const TabSettings& other) const
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
                /*if(other_element != elements_[k])
                {
                    return false;
                }*/
            }
            else
            {
                return false;
            }
        }

        return true;
    }
};

class ProjectFile
{
private:
    nlohmann::json j_;
    std::string file_path_;

    std::vector<TabSettings> tabs_;

    void parseTabs()
    {
        for(size_t k = 0; k < j_["tabs"].size(); k++)
        {
            tabs_.emplace_back(j_["tabs"][k]);
        }
    }

    void checkFields()
    {
        // throwIfMissing(j_, "layout", "Field 'layout' is missing!");
        // throwIfMissing(j_["layout"], "tabs", "Field 'tabs' is missing in the 'layout' field!");
    }

public:
    ProjectFile() = default;
    ProjectFile(const std::string& file_path) : file_path_(file_path)
    {
        std::ifstream input_file(file_path);
        input_file >> j_;
        checkFields();
        parseTabs();
    }

    TabSettings getTabFromIdx(const size_t idx) const
    {
        return tabs_.at(idx);
    }

    std::vector<TabSettings> getTabs() const
    {
        return tabs_;
    }

    nlohmann::json getJsonObject() const
    {
        return j_;
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

    bool operator==(const ProjectFile& other) const
    {
        return file_path_ == other.file_path_;
    }

    bool operator!=(const ProjectFile& other) const
    {
        return !(*this == other.file_path_);
    }

};

}

#endif
