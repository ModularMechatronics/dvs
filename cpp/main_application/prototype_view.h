#ifndef PROTOTYPE_VIEW_H_
#define PROTOTYPE_VIEW_H_

#include <wx/glcanvas.h>
#include <wx/wx.h>

#include "axes/axes.h"
#include "io_devices/io_devices.h"
#include "plot_data.h"
#include "opengl_low_level/opengl_header.h"
#include "dvs.h"
#include "udp_server.h"
#include "received_data.h"

struct GridSettings
{
    int num_cells_x;
    int num_cells_y;
    float margin_x;
    float margin_y;

    GridSettings() = default;

    GridSettings(const int num_cells_x_, const int num_cells_y_, const float margin_x_, const float margin_y_)
    {
        num_cells_x = num_cells_x_;
        num_cells_y = num_cells_y_;
        margin_x = margin_x_;
        margin_y = margin_y_;
    }
};

class PrototypeView : public wxGLCanvas
{
private:
    wxGLContext* m_context;
    wxSize size_;

    int args[9];

    int* getArgsPtr();

    GridSettings grid_settings_;

public:
    PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size);
    virtual ~PrototypeView();

    void setParameters(const GridSettings& grid_settings);
    void setPosAndSize(const wxPoint pos, const wxSize size);
    void render(wxPaintEvent& evt);
    void mouseLeftPressed(wxMouseEvent& event);

    DECLARE_EVENT_TABLE()
};


#endif
