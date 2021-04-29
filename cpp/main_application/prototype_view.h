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

    float x_min_screen_margin;
    float x_max_screen_margin;
    float y_min_screen_margin;
    float y_max_screen_margin;

    float x_min_gl;
    float x_max_gl;
    float y_min_gl;
    float y_max_gl;

    float x;
    float y;
    float width;
    float height;

public:

    float getX() const
    {
        return x;
    }

    float getY() const
    {
        return y;
    }

    float getWidth() const
    {
        return width;
    }

    float getHeight() const
    {
        return height;
    }

    CursorSquareState mouseState(const float mouse_x, const float mouse_y)
    {
        if((x_min_screen <= mouse_x) && (mouse_x <= x_max_screen) && (y_min_screen <= mouse_y) && (mouse_y <= y_max_screen))
        {
            if(mouse_x <= x_min_screen_margin)
            {
                if(mouse_y <= y_min_screen_margin)
                {
                    return CursorSquareState::BOTTOM_LEFT;
                }
                else if(y_max_screen_margin <= mouse_y)
                {
                    return CursorSquareState::TOP_LEFT;
                }
                else
                {
                    return CursorSquareState::LEFT;
                }
            }
            else if(x_max_screen_margin <= mouse_x)
            {
                if(mouse_y <= y_min_screen_margin)
                {
                    return CursorSquareState::BOTTOM_RIGHT;
                }
                else if(y_max_screen_margin <= mouse_y)
                {
                    return CursorSquareState::TOP_RIGHT;
                }
                else
                {
                    return CursorSquareState::RIGHT;
                }
            }
            else if(mouse_y <= y_min_screen_margin)
            {
                return CursorSquareState::BOTTOM;
            }
            else if(y_max_screen_margin <= mouse_y)
            {
                return CursorSquareState::TOP;
            }
            else
            {
                return CursorSquareState::INSIDE;
            }
        }
        else
        {
            return CursorSquareState::OUTSIDE;
        }
    }

    void setWidth(const float width_in)
    {
        width = width_in;
    }

    void setWidthAndX(const float width_in, const float x_in)
    {
        width = width_in;
        x = x_in;
    }

    void setHeightAndY(const float height_in, const float y_in)
    {
        height = height_in;
        y = y_in;
    }

    void setHeight(const float height_in)
    {
        height = height_in;
    }

    void setPosition(const float x_in, const float y_in)
    {
        x = x_in;
        y = y_in;
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
        x_min_screen = x * screen_grid_state.cell_size_x;
        x_max_screen = x_min_screen + width * screen_grid_state.cell_size_x;
        y_min_screen = y * screen_grid_state.cell_size_y;
        y_max_screen = y_min_screen + height * screen_grid_state.cell_size_y;

        const float center_x = (x_min_screen + x_max_screen) / 2.0f;
        const float center_y = (y_min_screen + y_max_screen) / 2.0f;

        x_min_screen_margin = std::min(x_min_screen + 20.0f, center_x - 5.0f);
        x_max_screen_margin = std::max(x_max_screen - 20.0f, center_x + 5.0f);
        y_min_screen_margin = std::min(y_min_screen + 20.0f, center_y - 5.0f);
        y_max_screen_margin = std::max(y_max_screen - 20.0f, center_y + 5.0f);

        x_min_gl = -1.0f + x * gl_grid_state.cell_size_x;
        x_max_gl = -1.0f + (x + width) * gl_grid_state.cell_size_x;
        y_min_gl = -1.0f + y * gl_grid_state.cell_size_y;
        y_max_gl = -1.0f + (y + height) * gl_grid_state.cell_size_y;
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

    bool left_button_pressed_;
    bool is_inside_square_;
    bool is_editing_;

    float mouse_x;
    float mouse_y;
    float prev_mouse_x;
    float prev_mouse_y;

    float grid_pos_pressed_x_;
    float grid_pos_pressed_y_;

    float pos_of_pressed_x_;
    float pos_of_pressed_y_;

    float size_of_pressed_width_;
    float size_of_pressed_height_;

    CursorSquareState cursor_square_state;

    int* getArgsPtr();

    GridSettings grid_settings_;

    std::vector<Square> squares_;
    int idx_of_selected_square_;

    void updateGridStates();

public:
    PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size);
    virtual ~PrototypeView();

    void setPosAndSize(const wxPoint pos, const wxSize size);
    void render(wxPaintEvent& evt);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseMoved(wxMouseEvent& event);

    void changeNumCellsX(const float change);
    void changeNumCellsY(const float change);

    DECLARE_EVENT_TABLE()
};


#endif
