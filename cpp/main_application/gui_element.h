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
    std::string name_;
    Element element_settings_;
    bool is_editing_;

public:

    GuiElement() = delete;
    GuiElement(const Element& element_settings)
    {
        is_editing_ = false;
        name_ = element_settings.name;
        element_settings_ = element_settings;
    }

    std::string getName() const
    {
        return name_;
    }

    void setIsEditing(const bool is_editing)
    {
        is_editing_ = is_editing;
    }

    virtual void updateSize(const Vec2Df& tab_cell_size) = 0;
    virtual void addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr) = 0;
    virtual void setPosAndSize(const wxPoint& pos, const wxSize& size) = 0;
    virtual void show() = 0;
    virtual void hide() = 0;

};

#endif
