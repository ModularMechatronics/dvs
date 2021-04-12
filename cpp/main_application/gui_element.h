#ifndef GUI_ELEMENT_H_
#define GUI_ELEMENT_H_

#include <memory>

#include <wx/wx.h>

#include "math/math.h"
#include "plot_window_gl_pane.h"


enum class GuiElementType
{
    PLOT,
    IMAGE,
    BUTTON,
    TEXT_FIELD
};

class GuiElement
{
private:
    GuiElementType type_;

    std::shared_ptr<PlotWindowGLPane> gl_pane_;

    bool is_dockable_;
    bool is_docked_;
public:
    GuiElement() = delete;
    GuiElement(wxFrame* parent, const bool is_dockable, const bool is_docked)
    {
        is_dockable_ = is_dockable;
        is_docked_ = is_docked;

        // gl_pane_ = std::make_shared<PlotWindowGLPane>(parent, wxPoint(0, 0));

        /*int args[] = {WX_GL_RGBA,
                      WX_GL_DOUBLEBUFFER,
                      WX_GL_DEPTH_SIZE,
                      16,
                      WX_GL_SAMPLES,
                      4,
                      WX_GL_SAMPLE_BUFFERS,
                      1,
                      0};

        gl_pane_ = new PlotWindowGLPane(this, args, wxPoint(0, 0));

        gl_pane_ = std::make_shared<PlotWindowGLPane>();*/
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
