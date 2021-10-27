#ifndef SHADER_APP_GL_CANVAS_H_
#define SHADER_APP_GL_CANVAS_H_

#include <wx/glcanvas.h>
#include <wx/notebook.h>
#include <wx/wx.h>

#include "axes/axes.h"
#include "communication/received_data.h"
#include "communication/udp_server.h"
#include "dvs/dvs.h"
#include "gui_element.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_header.h"
#include "plot_data.h"
#include "view_base.h"

class GlCanvas : public wxGLCanvas
{
private:
    wxGLContext* m_context;

    /*AxesInteractor* axes_interactor_;
    AxesPainter* axes_painter_;
    AxesSettings axes_settings_;
    MouseButtonState left_mouse_button_;
    KeyboardState keyboard_state_;
    CursorSquareState cursor_state_at_press_;*/

    int args[9];

    int* getArgsPtr()
    {
        return args;
    }

    MouseInteractionAxis current_mouse_interaction_axis_;
    AxesInteractor* axes_interactor_;
    AxesPainter* axes_painter_;
    AxesSettings axes_settings_;
    MouseButtonState left_mouse_button_;
    KeyboardState keyboard_state_;

    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);
    void mouseMoved(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);

    int getWidth();
    int getHeight();

public:
    GlCanvas(wxNotebookPage* parent);
    ~GlCanvas();

    void render(wxPaintEvent& evt);
    void show();

};

#endif
