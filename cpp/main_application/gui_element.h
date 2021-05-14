#ifndef GUI_ELEMENT_H_
#define GUI_ELEMENT_H_

#include <memory>

#include <wx/wx.h>
#include <wx/notebook.h>

#include "enumerations.h"
#include "math/math.h"
#include "project_file.h"
#include "received_data.h"

using namespace dvs;
using namespace project_file;

class GuiElement
{
protected:
    // GuiElementType type_;
    ElementSettings element_settings_;
    bool is_editing_;
    bool is_selected_;

    Vec2Df parent_size_; // Pixels

public:

    GuiElement() = delete;
    GuiElement(const ElementSettings& element_settings)
    {
        is_editing_ = false;
        element_settings_ = element_settings;
    }

    std::string getName() const
    {
        return element_settings_.name;
    }

    void setName(const std::string& new_name)
    {
        element_settings_.name = new_name;
    }

    void setIsEditing(const bool is_editing)
    {
        is_editing_ = is_editing;
    }

    bool isSelected()
    {
        return is_selected_;
    }

    ElementSettings getElementSettings() const
    {
        return element_settings_;
    }

    virtual void updateSizeFromParent(const wxSize& parent_size) = 0;
    virtual void addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr) = 0;
    virtual void setPosAndSize(const wxPoint& pos, const wxSize& size) = 0;
    virtual void resetSelection() = 0;
    virtual void setSelection() = 0;
    virtual void show() = 0;
    virtual void hide() = 0;
    virtual void destroy() = 0;
    virtual void refresh() = 0;

};

#endif
