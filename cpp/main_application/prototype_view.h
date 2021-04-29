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

    Size2Df() = default;
    Size2Df(const float width_, const float height_) : width(width_), height(height_) {}
};

struct GridState
{
    Vec2Df grid_size;
    Vec2Df cell_size;
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

class Square
{
private:
    Bound2Df screen_bound_;
    Bound2Df screen_bound_margin_;
    Bound2Df gl_bound_;

    Vec2Df pos_;
    Size2Df size_;

public:

    Vec2Df getPos() const
    {
        return pos_;
    }

    Size2Df getSize() const
    {
        return size_;
    }

    CursorSquareState mouseState(const Vec2Df mouse_pos) const
    {
        if((screen_bound_.x_min <= mouse_pos.x) && (mouse_pos.x <= screen_bound_.x_max) &&
           (screen_bound_.y_min <= mouse_pos.y) && (mouse_pos.y <= screen_bound_.y_max))
        {
            if(mouse_pos.x <= screen_bound_margin_.x_min)
            {
                if(mouse_pos.y <= screen_bound_margin_.y_min)
                {
                    return CursorSquareState::BOTTOM_LEFT;
                }
                else if(screen_bound_margin_.y_max <= mouse_pos.y)
                {
                    return CursorSquareState::TOP_LEFT;
                }
                else
                {
                    return CursorSquareState::LEFT;
                }
            }
            else if(screen_bound_margin_.x_max <= mouse_pos.x)
            {
                if(mouse_pos.y <= screen_bound_margin_.y_min)
                {
                    return CursorSquareState::BOTTOM_RIGHT;
                }
                else if(screen_bound_margin_.y_max <= mouse_pos.y)
                {
                    return CursorSquareState::TOP_RIGHT;
                }
                else
                {
                    return CursorSquareState::RIGHT;
                }
            }
            else if(mouse_pos.y <= screen_bound_margin_.y_min)
            {
                return CursorSquareState::BOTTOM;
            }
            else if(screen_bound_margin_.y_max <= mouse_pos.y)
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

    void setWidth(const float width)
    {
        size_.width = width;
    }

    void setWidthAndX(const float width, const float x)
    {
        size_.width = width;
        pos_.x = x;
    }

    void setHeightAndY(const float height, const float y)
    {
        size_.height = height;
        pos_.y = y;
    }

    void setHeight(const float height)
    {
        size_.height = height;
    }

    void setPosition(const float x, const float y)
    {
        pos_.x = x;
        pos_.y = y;
    }

    void render() const
    {
        glColor3f(0.0f, 0.0f, 1.0f);
        glBegin(GL_POLYGON);
        glVertex2f(gl_bound_.x_min, gl_bound_.y_max);
        glVertex2f(gl_bound_.x_min, gl_bound_.y_min);
        glVertex2f(gl_bound_.x_max, gl_bound_.y_min);
        glVertex2f(gl_bound_.x_max, gl_bound_.y_max);
        glEnd();
    }

    void updateInternals(const GridState& screen_grid_state, const GridState& gl_grid_state)
    {
        screen_bound_.x_min = pos_.x * screen_grid_state.cell_size.x;
        screen_bound_.x_max = screen_bound_.x_min + size_.width * screen_grid_state.cell_size.x;
        screen_bound_.y_min = pos_.y * screen_grid_state.cell_size.y;
        screen_bound_.y_max = screen_bound_.y_min + size_.height * screen_grid_state.cell_size.y;

        const float center_x = (screen_bound_.x_min + screen_bound_.x_max) / 2.0f;
        const float center_y = (screen_bound_.y_min + screen_bound_.y_max) / 2.0f;

        screen_bound_margin_.x_min = std::min(screen_bound_.x_min + 20.0f, center_x - 5.0f);
        screen_bound_margin_.x_max = std::max(screen_bound_.x_max - 20.0f, center_x + 5.0f);
        screen_bound_margin_.y_min = std::min(screen_bound_.y_min + 20.0f, center_y - 5.0f);
        screen_bound_margin_.y_max = std::max(screen_bound_.y_max - 20.0f, center_y + 5.0f);

        gl_bound_.x_min = -1.0f + pos_.x * gl_grid_state.cell_size.x;
        gl_bound_.x_max = -1.0f + (pos_.x + size_.width) * gl_grid_state.cell_size.x;
        gl_bound_.y_min = -1.0f + pos_.y * gl_grid_state.cell_size.y;
        gl_bound_.y_max = -1.0f + (pos_.y + size_.height) * gl_grid_state.cell_size.y;
    }

    Square() = default;
    Square(const float x_, const float y_, const float width_, const float height_) : pos_(x_, y_), size_(width_, height_) {}
};

class PrototypeView : public wxGLCanvas
{
private:
    wxGLContext* m_context;
    wxSize panel_size_;

    int args[9];

    bool left_button_pressed_;
    bool is_inside_square_;
    bool is_editing_;

    GridState screen_grid_state_;
    GridState gl_grid_state_;

    Vec2Df grid_pos_pressed_;

    Vec2Df pos_of_pressed_sq_;
    Size2Df size_of_pressed_;

    Bound2Df gl_bounds_;

    CursorSquareState cursor_square_state;

    int* getArgsPtr();

    Vec2Df num_grid_cells_;

    std::vector<Square> squares_;
    int idx_of_selected_square_;

    void updateGridStates();

public:
    PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size_);
    virtual ~PrototypeView();

    void setPosAndSize(const wxPoint pos_, const wxSize size_);
    void render(wxPaintEvent& evt);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseMoved(wxMouseEvent& event);

    void changeNumCellsX(const float change);
    void changeNumCellsY(const float change);
};


#endif
