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
    // GuiElementType type_;

    PlotWindowGLPane* gl_pane_;
    std::string name_;

    bool is_dockable_;
    bool is_docked_;
public:
    GuiElement() = delete;
    GuiElement(wxPanel* parent, const wxPoint& position, const wxSize& size, const std::string& name)
    {
        is_dockable_ = false;
        is_docked_ = false;
        name_ = name;

        gl_pane_ = new PlotWindowGLPane(parent, position, size);
    }

    GuiElement(wxFrame* parent, const wxPoint& position, const wxSize& size, const std::string& name)
    {
        is_dockable_ = false;
        is_docked_ = false;
        name_ = name;

        gl_pane_ = new PlotWindowGLPane(parent, position, size);
    }

    void addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr)
    {
        gl_pane_->addData(std::move(received_data), hdr);
    }

    std::string name() const
    {
        return name_;
    }

    void windowSizeChanged(wxSizeEvent& event)
    {
        // std::cout << "Size changed from" << name_ << std::endl;
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
