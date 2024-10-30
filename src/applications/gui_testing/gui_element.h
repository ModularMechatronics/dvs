#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include <wx/wx.h>

#include <string>

#include "rgbtriplet.h"
#include "vertex_buffer.h"

enum class ChangeDirection : uint8_t
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    LEFT_UP,
    LEFT_DOWN,
    RIGHT_UP,
    RIGHT_DOWN,
    MIDDLE,
    NONE
};

class GuiElement
{
protected:
    float x_;
    float y_;
    float width_;
    float height_;

    uint64_t id_;

    std::string name_;

    float parent_width_;
    float parent_height_;
    uint16_t z_order_;

    VertexBuffer vertex_buffer_;
    RGBTripletf color_;

public:
    GuiElement() = default;
    GuiElement(const float x,
               const float y,
               const float width,
               const float height,
               const std::string& name,
               const uint16_t z_order,
               const RGBTripletf& color);

    void UpdateSizeFromParent(const wxSize new_size);
    bool PointIsWithin(const wxPoint pt) const;

    void render() const;

    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    void mouseExited(wxPoint& exit_point);
    void mouseMoved(wxMouseEvent& event, const wxPoint& delta_vec);
    void mouseDragged(wxMouseEvent& event, const wxPoint& delta_vec);
    void mousePressed(wxMouseEvent& event);
    void mouseReleased(wxMouseEvent& event);
    ChangeDirection GetDirectionFromMouse(const wxPoint pt) const;
    void ChangePositionOrSize(const wxPoint delta_vec, const ChangeDirection change_direction);

    RGBTripletf getColor() const
    {
        return color_;
    }

    void bringToFront()
    {
        // z_order_ = 0;
    }

    void sendToBack()
    {
        // z_order_ = 1000;
    }

    uint16_t zOrder() const
    {
        return z_order_;
    }

    void ChangeSize(const float delta_width, const float delta_height, const ChangeDirection direction);
    void ChangePosition(const float delta_x, const float delta_y);
};

#endif
