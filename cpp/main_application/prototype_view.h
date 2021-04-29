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

struct Bound2Df
{
    float x_min;
    float x_max;
    float y_min;
    float y_max;

    Bound2Df() = default;
};

struct Size2Df
{
    float width;
    float height;
};

struct GridSettings
{
    float num_cells_x;
    float num_cells_y;

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
};

class Square
{
private:
    Bound2Df screen_bound;
    Bound2Df screen_bound_margin;
    Bound2Df gl_bound;

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

    CursorSquareState mouseState(const Vec2Df mouse_pos) const
    {
        if((screen_bound.x_min <= mouse_pos.x) && (mouse_pos.x <= screen_bound.x_max) && (screen_bound.y_min <= mouse_pos.y) && (mouse_pos.y <= screen_bound.y_max))
        {
            if(mouse_pos.x <= screen_bound_margin.x_min)
            {
                if(mouse_pos.y <= screen_bound_margin.y_min)
                {
                    return CursorSquareState::BOTTOM_LEFT;
                }
                else if(screen_bound_margin.y_max <= mouse_pos.y)
                {
                    return CursorSquareState::TOP_LEFT;
                }
                else
                {
                    return CursorSquareState::LEFT;
                }
            }
            else if(screen_bound_margin.x_max <= mouse_pos.x)
            {
                if(mouse_pos.y <= screen_bound_margin.y_min)
                {
                    return CursorSquareState::BOTTOM_RIGHT;
                }
                else if(screen_bound_margin.y_max <= mouse_pos.y)
                {
                    return CursorSquareState::TOP_RIGHT;
                }
                else
                {
                    return CursorSquareState::RIGHT;
                }
            }
            else if(mouse_pos.y <= screen_bound_margin.y_min)
            {
                return CursorSquareState::BOTTOM;
            }
            else if(screen_bound_margin.y_max <= mouse_pos.y)
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
        glVertex2f(gl_bound.x_min, gl_bound.y_max);
        glVertex2f(gl_bound.x_min, gl_bound.y_min);
        glVertex2f(gl_bound.x_max, gl_bound.y_min);
        glVertex2f(gl_bound.x_max, gl_bound.y_max);
        glEnd();
    }

    void updateInternals(const GridSettings& grid_settings, const GridState& screen_grid_state, const GridState& gl_grid_state)
    {
        screen_bound.x_min = x * screen_grid_state.cell_size_x;
        screen_bound.x_max = screen_bound.x_min + width * screen_grid_state.cell_size_x;
        screen_bound.y_min = y * screen_grid_state.cell_size_y;
        screen_bound.y_max = screen_bound.y_min + height * screen_grid_state.cell_size_y;

        const float center_x = (screen_bound.x_min + screen_bound.x_max) / 2.0f;
        const float center_y = (screen_bound.y_min + screen_bound.y_max) / 2.0f;

        screen_bound_margin.x_min = std::min(screen_bound.x_min + 20.0f, center_x - 5.0f);
        screen_bound_margin.x_max = std::max(screen_bound.x_max - 20.0f, center_x + 5.0f);
        screen_bound_margin.y_min = std::min(screen_bound.y_min + 20.0f, center_y - 5.0f);
        screen_bound_margin.y_max = std::max(screen_bound.y_max - 20.0f, center_y + 5.0f);

        gl_bound.x_min = -1.0f + x * gl_grid_state.cell_size_x;
        gl_bound.x_max = -1.0f + (x + width) * gl_grid_state.cell_size_x;
        gl_bound.y_min = -1.0f + y * gl_grid_state.cell_size_y;
        gl_bound.y_max = -1.0f + (y + height) * gl_grid_state.cell_size_y;
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

    Bound2Df gl_bounds_;

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
