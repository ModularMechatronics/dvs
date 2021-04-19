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

struct Level
{
    int row;
    int column;
    Level() : row(0), column(0) {}
};

class ElementBase
{
protected:
    nlohmann::json j_;
    Level level_;
    ElementType type_;
    std::string name_;

    std::vector<std::shared_ptr<ElementBase>> all_elements_;
    std::vector<std::shared_ptr<ElementBase>> elements_;

public:

    std::vector<std::shared_ptr<ElementBase>> getElements() const
    {
        std::vector<std::shared_ptr<ElementBase>> output;
        std::copy(elements_.begin(), elements_.end(), std::back_inserter(output));

        for(auto e : all_elements_)
        {
            auto elems = e->getElements();
            std::copy(elems.begin(), elems.end(), std::back_inserter(output));
        }
        return output;
    }

    Level getLevel() const
    {
        return level_;
    }

    ElementType getType() const
    {
        return type_;
    }

    std::string getName() const
    {
        return name_;
    }

    ElementBase(const nlohmann::json& j, const Level& level, const ElementType type) : j_(j), level_(level), type_(type) {}
    ElementBase() : type_(ElementType::BASE) {}
};

inline void constructElements(const nlohmann::json& j_,
                              Level& level,
                              std::vector<std::shared_ptr<ElementBase>>& all_elements,
                              std::vector<std::shared_ptr<ElementBase>>& elements);

class Element : public ElementBase
{
private:

public:
    Element() = delete;
    Element(const nlohmann::json& j, Level& level) : ElementBase(j, level, ElementType::ELEMENT)
    {
        name_ = j["name"];
    }
};

class Column : public ElementBase
{
private:

public:
    Column() = delete;
    Column(const nlohmann::json& j, Level& level) : ElementBase(j, level, ElementType::COLUMN)
    {
        name_ = "<column," + std::to_string(level.row) + "," + std::to_string(level.column) + ">";
        level.column += 1;

        constructElements(j_, level, all_elements_, elements_);
    }
};

class Row : public ElementBase
{
private:
    void checkFields()
    {

    }

public:
    Row() = delete;
    Row(const nlohmann::json& j, Level& level) : ElementBase(j, level, ElementType::ROW)
    {
        name_ = "<row," + std::to_string(level.row) + "," + std::to_string(level.column) + ">";
        level.row += 1;

        constructElements(j_, level, all_elements_, elements_);
    }
};

void constructElements(const nlohmann::json& j_,
                       Level& level,
                       std::vector<std::shared_ptr<ElementBase>>& all_elements,
                       std::vector<std::shared_ptr<ElementBase>>& elements)
{
    if(j_.count("columns") == 1 || j_.count("rows") == 1)
    {
        if(j_.count("columns") == 1 && j_.count("rows") == 1)
        {
            throw std::runtime_error("Found both 'rows' and 'columns' on the same level, which is ambiguous!");
        }
        std::string key = "";
        if(j_.count("rows") == 1)
        {
            key = "rows";
        }
        else if(j_.count("columns") == 1)
        {
            key = "columns";
        }

        for(size_t k = 0; k < j_[key].size(); k++)
        {
            if(j_[key][k].count("type") == 1)
            {
                std::shared_ptr<Element> elem(new Element(j_[key][k], level));
                all_elements.push_back(std::dynamic_pointer_cast<ElementBase>(elem));
                elements.push_back(std::dynamic_pointer_cast<ElementBase>(elem));
            }
            else if(j_[key][k].count("columns") == 1)
            {
                std::shared_ptr<Column> col(new Column(j_[key][k], level));
                all_elements.push_back(std::dynamic_pointer_cast<ElementBase>(col));
            }
            else if(j_[key][k].count("rows") == 1)
            {
                std::shared_ptr<Row> row(new Row(j_[key][k], level));
                all_elements.push_back(std::dynamic_pointer_cast<ElementBase>(row));
            }
            else
            {
                throw std::runtime_error("U oh!");
            }
        }
    }
    else
    {
        throw std::runtime_error("Can't find valid key!");
    }
}

class Tab
{
private:
    nlohmann::json j_;
    int tab_idx_;

    std::vector<std::shared_ptr<ElementBase>> all_elements_;
    std::vector<std::shared_ptr<ElementBase>> elements_;

    std::string name_;
    TabType type_;

    void parseFreeformElements()
    {

    }

    void parseGridElements()
    {
        Level level;

        constructElements(j_, level, all_elements_, elements_);
    }

    void checkFields()
    {
        throwIfMissing(j_, "name", "Field 'name' is missing from tab with index: " + std::to_string(tab_idx_) + "!");
        throwIfMissing(j_, "type", "Field 'type' is missing from tab with index: " + std::to_string(tab_idx_) + "!");

        const std::string tt = j_["type"];

        if(tt == grid_str)
        {
            type_ = TabType::GRID;
            throwIfMissing(j_, "rows", "Field 'rows' (needed for type 'grid') is missing from tab with index: " + std::to_string(tab_idx_) + "!");
        }
        else if(tt == freeform_str)
        {
            type_ = TabType::FREEFORM;
            throwIfMissing(j_, "elements", "Field 'elements' (needed for type 'freeform') is missing from tab with index: " + std::to_string(tab_idx_) + "!");
        }
        else
        {
            throw std::runtime_error("Invalid tab type '" + tt + " in tab with index " + std::to_string(tab_idx_));
        }
    }

public:
    Tab() = delete;
    Tab(const nlohmann::json& j, const int tab_idx) : j_(j), tab_idx_(tab_idx)
    {
        checkFields();
        name_ = j_["name"];

        if(type_ == TabType::GRID)
        {
            parseGridElements();
        }
        else
        {
            parseFreeformElements();
        }
    }

    std::vector<std::shared_ptr<ElementBase>> getElements() const
    {
        std::vector<std::shared_ptr<ElementBase>> output;
        std::copy(elements_.begin(), elements_.end(), std::back_inserter(output));

        for(auto e : all_elements_)
        {
            auto elems = e->getElements();
            std::copy(elems.begin(), elems.end(), std::back_inserter(output));
        }
        return output;
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
        for(size_t k = 0; k < j_["layout"]["tabs"].size(); k++)
        {
            tabs_.emplace_back(j_["layout"]["tabs"][k], k);
        }
    }

    void checkFields()
    {
        throwIfMissing(j_, "layout", "Field 'layout' is missing!");
        throwIfMissing(j_["layout"], "tabs", "Field 'tabs' is missing in the 'layout' field!");
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

    std::vector<std::shared_ptr<ElementBase>> getElements() const
    {
        std::vector<std::shared_ptr<ElementBase>> all_elements;

        for(auto t : tabs_)
        {
            std::vector<std::shared_ptr<ElementBase>> v = t.getElements();
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
