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
    float num_cells_x;
    float num_cells_y;
    float margin_x;
    float margin_y;

    GridSettings() = default;
};

enum class CursorSquareState
{
    INSIDE,
    OUTSIDE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM,
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT
};

struct GridState
{
    float grid_size_x;
    float grid_size_y;
    float cell_size_x;
    float cell_size_y;
    float offset_x;
    float offset_y;

    float x_min;
    float x_max;
    float y_min;
    float y_max;
};

class Square
{
private:
    float x_min_screen;
    float x_max_screen;
    float y_min_screen;
    float y_max_screen;

    float x_min_gl;
    float x_max_gl;
    float y_min_gl;
    float y_max_gl;

    float x;
    float y;
    float width;
    float height;

public:

    CursorSquareState mouseState(const float mouse_x, const float mouse_y)
    {
        if((x_min_screen <= mouse_x) && (mouse_x <= x_max_screen) && (y_min_screen <= mouse_y) && (mouse_y <= y_max_screen))
        {
            return CursorSquareState::INSIDE;
        }
        else
        {
            return CursorSquareState::OUTSIDE;
        }
    }

    void render() const
    {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
        glVertex2f(x_min_gl, y_max_gl);
        glVertex2f(x_min_gl, y_min_gl);
        glVertex2f(x_max_gl, y_min_gl);
        glVertex2f(x_max_gl, y_max_gl);
        glEnd();
    }

    void updateInternals(const GridSettings& grid_settings, const GridState& screen_grid_state, const GridState& gl_grid_state)
    {
        x_min_screen = grid_settings.margin_x + x * screen_grid_state.cell_size_x;
        x_max_screen = x_min_screen + width * screen_grid_state.cell_size_x;
        y_min_screen = grid_settings.margin_y + y * screen_grid_state.cell_size_y;
        y_max_screen = y_min_screen + height * screen_grid_state.cell_size_y;

        x_min_gl = -1.0f + gl_grid_state.offset_x + x * gl_grid_state.cell_size_x;
        x_max_gl = -1.0f + gl_grid_state.offset_x + (x + width) * gl_grid_state.cell_size_x;
        y_min_gl = -1.0f + gl_grid_state.offset_y + y * gl_grid_state.cell_size_y;
        y_max_gl = -1.0f + gl_grid_state.offset_y + (y + height) * gl_grid_state.cell_size_y;
    }

    Square() = default;
    Square(const float x_, const float y_, const float width_, const float height_) : x(x_), y(y_), width(width_), height(height_) {}
};

class PrototypeView : public wxGLCanvas
{
private:
    wxGLContext* m_context;
    wxSize size_;

    GridState screen_grid_state_;
    GridState gl_grid_state_;

    int args[9];

    int* getArgsPtr();

    GridSettings grid_settings_;

    std::vector<Square> squares_;

    void updateGridStates();

public:
    PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size);
    virtual ~PrototypeView();

    void setPosAndSize(const wxPoint pos, const wxSize size);
    void render(wxPaintEvent& evt);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseMoved(wxMouseEvent& event);

    void changeNumCellsX(const float change);
    void changeNumCellsY(const float change);
    void changeMarginX(const float change);
    void changeMarginY(const float change);

    DECLARE_EVENT_TABLE()
};


#endif
