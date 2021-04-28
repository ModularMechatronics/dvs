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
    int cell_idx_x;
    int cell_idx_y;
    int width;
    int height;
    std::string name;

    Element() = default;
    Element(const nlohmann::json& j)
    {
        name = j["name"];

        cell_idx_x = j["cell-idx-x"];
        cell_idx_y = j["cell-idx-y"];
        width = j["width"];
        height = j["height"];
    }
};

class Tab
{
private:
    nlohmann::json j_;
    int tab_idx_;

    std::vector<Element> elements_;

    std::string name_;

    void parseElements()
    {
        margin_x = j_["grid-settings"]["margin-x"];
        margin_y = j_["grid-settings"]["margin-y"];
        num_cells_x = j_["grid-settings"]["num-cells-x"];
        num_cells_y = j_["grid-settings"]["num-cells-y"];

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
    int margin_x;
    int margin_y;

    Tab() = delete;
    Tab(const nlohmann::json& j, const int tab_idx) : j_(j), tab_idx_(tab_idx)
    {
        checkFields();
        name_ = j_["name"];

        parseElements();
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
            tabs_.emplace_back(j_["tabs"][k], k);
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
