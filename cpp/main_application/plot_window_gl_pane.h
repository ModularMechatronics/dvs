#ifndef PLOTWINDOW_GL_PLANE_
#define PLOTWINDOW_GL_PLANE_

#include <wx/glcanvas.h>
#include <wx/wx.h>
#include <wx/notebook.h>

#include "axes/axes.h"
#include "io_devices/io_devices.h"
#include "plot_data.h"
#include "opengl_low_level/opengl_header.h"
#include "dvs.h"
#include "udp_server.h"
#include "received_data.h"
#include "gui_element.h"
#include "prototype_view.h"
#include "tab_view.h"

using namespace project_file;

class PlotWindowGLPane : public wxGLCanvas, public GuiElement
{
private:
    wxGLContext* m_context;

    AxesInteractor* axes_interactor_;
    AxesPainter* axes_painter_;
    MouseButtonState left_mouse_button_;
    KeyboardState keyboard_state_;
    CursorSquareState cursor_state_at_press_;

    bool hold_on_;
    bool axes_set_;

    int args[9];

    int* getArgsPtr();
    wxSize parent_size_;
    Vec2Df mouse_pos_at_press_;
    wxPoint pos_at_press_;
    wxSize size_at_press_;
    float grid_size_;

    PlotDataHandler plot_data_handler_;
    
    TabView* tab_view_parent_;

public:
    PlotWindowGLPane(wxNotebookPage* parent, const Element& element_settings, const float grid_size);
    ~PlotWindowGLPane();

    int getWidth();
    int getHeight();

    void render(wxPaintEvent& evt);

    void setPosition(const wxPoint& new_pos);
    void setSize(const wxSize& new_size);
    void setPosAndSize(const wxPoint& pos, const wxSize& size) override;
    void updateSizeFromParent(const wxSize& parent_size) override;
    void addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr) override;
    void show() override;
    void hide() override;
    void resetSelection() override;
    void destroy() override;

    // Event callback function
    void mouseMoved(wxMouseEvent& event);
    void mouseLeftWindow(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);

    void bindCallbacks();
};

#endif