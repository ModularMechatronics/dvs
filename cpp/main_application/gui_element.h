#ifndef GUI_ELEMENT_H_
#define GUI_ELEMENT_H_

#include <memory>

#include <wx/wx.h>

#include "enumerations.h"
#include "math/math.h"
#include "plot_window_gl_pane.h"

class GuiElement
{
private:
    GuiElementType type_;

    PlotWindowGLPane* gl_pane_;

    bool is_dockable_;
    bool is_docked_;
public:
    GuiElement() = delete;
    GuiElement(wxPanel* parent, wxSize size, const std::string& name)
    {
        is_dockable_ = false;
        is_docked_ = false;

        gl_pane_ = new PlotWindowGLPane(parent, wxPoint(0, 0), size);
    }

    GuiElement(wxFrame* parent, wxSize size, const std::string& name)
    {
        is_dockable_ = false;
        is_docked_ = false;

        gl_pane_ = new PlotWindowGLPane(parent, wxPoint(0, 0), size);
    }


};

class GuiElementDocked : GuiElement
{

};

class GuiElementWindow : GuiElement
{
private:

public:
};



#endif
