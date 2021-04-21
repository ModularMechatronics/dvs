#ifndef PLOTWINDOW_GL_PLANE_
#define PLOTWINDOW_GL_PLANE_

#include <wx/glcanvas.h>
#include <wx/wx.h>

#include "axes/axes.h"
// #include "communication/rx_list.h"
#include "io_devices/io_devices.h"
#include "plot_data.h"
#include "opengl_low_level/opengl_header.h"
#include "dvs.h"
#include "udp_server.h"

class PlotWindowGLPane : public wxGLCanvas
{
private:
    wxGLContext* m_context;

    AxesInteractor* axes_interactor_;
    AxesPainter* axes_painter_;
    MouseButtonState left_mouse_button_;
    KeyboardState keyboard_state_;

    bool hold_on_;
    bool axes_set_;

    int args[9];

    int* getArgsPtr();

    // PlotDataHandler plot_data_handler_;

public:
    PlotWindowGLPane(wxPanel* parent, const wxPoint& position, const wxSize& size);
    PlotWindowGLPane(wxFrame* parent, const wxPoint& position, const wxSize& size);
    virtual ~PlotWindowGLPane();

    void resized(wxSizeEvent& evt);

    int getWidth();
    int getHeight();

    void render(wxPaintEvent& evt);

    // void addData(const plot_tool::RxList& rx_list, const std::vector<char*> data_vec);
    void addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr);

    // Event callback function
    void mouseMoved(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void keyPressed(wxKeyEvent& event);
    void keyReleased(wxKeyEvent& event);

    DECLARE_EVENT_TABLE()
};
#endif