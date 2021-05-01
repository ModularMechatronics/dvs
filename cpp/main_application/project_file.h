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

class Element
{
public:
    float x;
    float y;
    float width;
    float height;

    std::string name;

    Element() = default;
    Element(const nlohmann::json& j)
    {
        name = j["name"];

        x = j["x"];
        y = j["y"];
        width = j["width"];
        height = j["height"];
    }
};

class Tab
{
private:
    nlohmann::json j_;

    std::vector<Element> elements_;

    std::string name_;

    void parseElements()
    {
        for(size_t k = 0; k < j_["elements"].size(); k++)
        {
            elements_.emplace_back(j_["elements"][k]);
        }
    }

    void checkFields()
    {
        
    }

public:
    int num_cells_x;
    int num_cells_y;

    Tab() = default;
    Tab(const nlohmann::json& j) : j_(j)
    {
        checkFields();
        name_ = j_["name"];

        parseElements();
    }

    std::string getName() const
    {
        return name_;
    }

    std::vector<Element> getElements() const
    {
        return elements_;
    }
};

class ProjectFile
{
private:
    nlohmann::json j_;
    std::string file_path_;

    std::vector<Tab> tabs_;

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

    Tab getTabFromIdx(const size_t idx) const
    {
        return tabs_.at(idx);
    }

    std::vector<Tab> getTabs() const
    {
        return tabs_;
    }

    std::vector<Element> getElements() const
    {
        std::vector<Element> all_elements;

        for(auto t : tabs_)
        {
            std::vector<Element> v = t.getElements();
            for(auto tv : v)
            {
                all_elements.push_back(tv);
            }
        }

        return all_elements;
    }
};

}

#endif
