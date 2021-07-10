#ifndef PROTOTYPE_VIEW_H_
#define PROTOTYPE_VIEW_H_

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
#include "project_file.h"

struct Bound2Df
{
    float x_min;
    float x_max;
    float y_min;
    float y_max;

    Bound2Df() = default;
};

inline Bound2Df operator+(const Bound2Df& bnd, const Vec2Df& offset)
{
    Bound2Df new_bnd;
    new_bnd.x_min = bnd.x_min + offset.x;
    new_bnd.x_max = bnd.x_max + offset.x;
    new_bnd.y_min = bnd.y_min + offset.y;
    new_bnd.y_max = bnd.y_max + offset.y;

    return new_bnd;
}


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

inline void drawCircle(const Vec2Df& pos, const float r)
{
    const int num_elements = 20;

    float t = 0.0f;
    const float inc = (2.0f * M_PI) / static_cast<float>(num_elements - 1);
    glBegin(GL_POLYGON);
    for(size_t k = 0; k < num_elements; k++)
    {
        const float x = r * std::cos(t) + pos.x;
        const float y = r * std::sin(t) + pos.y;
        glVertex2f(x, y);
        t = t + inc;
    }
    glEnd();
}

inline void drawCircularSegment(const Vec2Df& pos, const float r, const float from_angle, const float to_angle)
{
    const int num_elements = 10;

    float t = from_angle;
    const float inc = (to_angle - from_angle) / static_cast<float>(num_elements-1);
    glBegin(GL_POLYGON);
    glVertex2f(pos.x, pos.y);
    for(size_t k = 0; k < num_elements; k++)
    {
        const float x = r * std::cos(t) + pos.x;
        const float y = r * std::sin(t) + pos.y;
        glVertex2f(x, y);
        t = t + inc;
    }

    glEnd();
}

inline void drawRoundedRectangle(const Bound2Df& bounds, const float r)
{
    const Vec2Df start(bounds.x_min + r, bounds.y_min + r);
    Vec2Df vec = start;
    const float y_dist = (bounds.y_max - bounds.y_min) - 2.0f * r;
    const float x_dist = (bounds.x_max - bounds.x_min) - 2.0f * r;

    glBegin(GL_POLYGON);
    glVertex2f(vec.x, vec.y);
    vec.x -= r;
    glVertex2f(vec.x, vec.y);
    vec.y += y_dist;
    glVertex2f(vec.x, vec.y);
    vec.x += r;
    glVertex2f(vec.x, vec.y);
    vec.y += r;
    glVertex2f(vec.x, vec.y);
    vec.x += x_dist;
    glVertex2f(vec.x, vec.y);
    vec.y -= r;
    glVertex2f(vec.x, vec.y);
    vec.x += r;
    glVertex2f(vec.x, vec.y);
    vec.y -= y_dist;
    glVertex2f(vec.x, vec.y);
    vec.x -= r;
    glVertex2f(vec.x, vec.y);
    vec.y -= r;
    glVertex2f(vec.x, vec.y);
    vec.x -= x_dist;
    glVertex2f(vec.x, vec.y);
    glVertex2f(start.x, start.y);
    glEnd();

    drawCircularSegment(Vec2Df(bounds.x_min, bounds.y_min) + r, r, M_PI, 3.0f * M_PI / 2.0f);
    drawCircularSegment(Vec2Df(bounds.x_max, bounds.y_max) - r, r, 0, M_PI / 2.0f);
    drawCircularSegment(Vec2Df(bounds.x_min + r, bounds.y_max - r), r, M_PI / 2.0f, M_PI);
    drawCircularSegment(Vec2Df(bounds.x_max - r, bounds.y_min + r), r, 3.0f * M_PI / 2.0f, 2.0f * M_PI);
}

class Square
{
private:
    Bound2Df screen_bound_;
    Bound2Df screen_bound_margin_;
    Bound2Df gl_bound_;

    Vec2Df pos_;
    Size2Df size_;
    RGBTripletf color_;

    bool is_selected_;

    wxTextCtrl* name_field_;
    wxNotebookPage* parent_;

public:

    Square() = delete;
    Square(wxNotebookPage* parent, const float x_, const float y_, const float width_, const float height_, const RGBTripletf& color)
        : pos_(x_, y_), size_(width_, height_)
    {
        parent_ = parent;
        name_field_ = new wxTextCtrl(parent, wxID_ANY, "<no name>");
        color_ = color;
        is_selected_ = false;
    }

    Square(const Square& other) = delete;

    void show()
    {
        name_field_->Show();
    }

    void hide()
    {
        name_field_->Hide();
    }

    std::string getText() const
    {
        return std::string(name_field_->GetValue().mb_str());
    }

    ~Square()
    {
        delete name_field_;
    }

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

    bool isSelected() const
    {
        return is_selected_;
    }

    void setIsSelected(bool is_selected)
    {
        is_selected_ = is_selected;
    }

    void render() const
    {
        const Bound2Df shadow_bnd = gl_bound_ + Vec2Df(0.01f, -0.01f);

        glColor4f(0.1f, 0.1f, 0.1f, 0.4f);
        drawRoundedRectangle(shadow_bnd, 0.02f);

        glColor3f(color_.red, color_.green, color_.blue);
        drawRoundedRectangle(gl_bound_, 0.01f);

        if(is_selected_)
        {
            glLineWidth(3.0f);
            glColor3f(1.0f, 0.0f, 1.0f);

            glBegin(GL_LINES);
            glVertex2f(gl_bound_.x_min, gl_bound_.y_min);
            glVertex2f(gl_bound_.x_min, gl_bound_.y_max);
            glEnd();

            glBegin(GL_LINES);
            glVertex2f(gl_bound_.x_min, gl_bound_.y_max);
            glVertex2f(gl_bound_.x_max, gl_bound_.y_max);
            glEnd();

            glBegin(GL_LINES);
            glVertex2f(gl_bound_.x_max, gl_bound_.y_max);
            glVertex2f(gl_bound_.x_max, gl_bound_.y_min);
            glEnd();

            glBegin(GL_LINES);
            glVertex2f(gl_bound_.x_max, gl_bound_.y_min);
            glVertex2f(gl_bound_.x_min, gl_bound_.y_min);
            glEnd();
        }
    }

    void updateInternals(const GridState& screen_grid_state, const GridState& gl_grid_state)
    {
        screen_bound_.x_min = pos_.x * screen_grid_state.cell_size.x;
        screen_bound_.x_max = screen_bound_.x_min + size_.width * screen_grid_state.cell_size.x;
        screen_bound_.y_min = pos_.y * screen_grid_state.cell_size.y;
        screen_bound_.y_max = screen_bound_.y_min + size_.height * screen_grid_state.cell_size.y;

        const float center_x = (screen_bound_.x_min + screen_bound_.x_max) / 2.0f;
        const float center_y = (screen_bound_.y_min + screen_bound_.y_max) / 2.0f;

        const float margin_offset = 10.0f;

        screen_bound_margin_.x_min = std::min(screen_bound_.x_min + margin_offset, center_x - 5.0f);
        screen_bound_margin_.x_max = std::max(screen_bound_.x_max - margin_offset, center_x + 5.0f);
        screen_bound_margin_.y_min = std::min(screen_bound_.y_min + margin_offset, center_y - 5.0f);
        screen_bound_margin_.y_max = std::max(screen_bound_.y_max - margin_offset, center_y + 5.0f);

        gl_bound_.x_min = -1.0f + pos_.x * gl_grid_state.cell_size.x;
        gl_bound_.x_max = -1.0f + (pos_.x + size_.width) * gl_grid_state.cell_size.x;
        gl_bound_.y_min = -1.0f + pos_.y * gl_grid_state.cell_size.y;
        gl_bound_.y_max = -1.0f + (pos_.y + size_.height) * gl_grid_state.cell_size.y;

        name_field_->SetPosition(wxPoint(screen_bound_.x_min, screen_grid_state.grid_size.y - screen_bound_.y_min));
    }
};

class PrototypeView : public wxGLCanvas
{
private:
    wxGLContext* m_context;
    wxNotebookPage* parent_;
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

    std::vector<Square*> squares_;
    int idx_of_selected_square_;

    void updateGridStates();

    int current_color_index_;

    std::vector<RGBTripletf> colors_;

public:
    PrototypeView(wxNotebookPage* parent,
                  const wxPoint& position,
                  const wxSize& size,
                  const Vec2Df& num_grid_cells,
                  const std::vector<project_file::ElementSettings>& elements);
    virtual ~PrototypeView();

    void setSize(const wxSize size_);
    void render(wxPaintEvent& evt);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseMoved(wxMouseEvent& event);

    void changeNumCellsX(const float change);
    void changeNumCellsY(const float change);
    void newElement();
    void deleteElement();

    void show();
    void hide();
};


#endif
