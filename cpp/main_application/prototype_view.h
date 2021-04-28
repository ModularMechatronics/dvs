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
    int margin_x;
    int margin_y;

    GridSettings() = default;
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

    void setPosAndSize(const wxPoint pos, const wxSize size);
    void render(wxPaintEvent& evt);
    void mouseLeftPressed(wxMouseEvent& event);

    void changeNumCellsX(const int change);
    void changeNumCellsY(const int change);
    void changeMarginX(const int change);
    void changeMarginY(const int change);

    DECLARE_EVENT_TABLE()
};


#endif
