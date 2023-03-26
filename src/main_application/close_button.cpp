#include "close_button.h"

CloseButton::CloseButton(wxFrame* parent,
                         const wxPoint& pos,
                         const wxSize& size,
                         const wxColour& color,
                         const std::function<void()>& button_pressed_callback,
                         const std::function<void(wxPaintDC& dc, const int button_size)>& icon_draw_function)
    : wxPanel(parent, wxID_ANY, pos, size),
      button_pressed_callback_{button_pressed_callback},
      icon_draw_function_{icon_draw_function}
{
    button_rect_ = wxRect(0, 0, size.GetWidth(), size.GetHeight());

    button_pressed_ = false;

    pressed_color_ = color;

    Bind(wxEVT_PAINT, &CloseButton::OnPaint, this);
    Bind(wxEVT_LEFT_UP, &CloseButton::mouseLeftReleased, this);
    Bind(wxEVT_ENTER_WINDOW, &CloseButton::mouseEntered, this);
    Bind(wxEVT_LEAVE_WINDOW, &CloseButton::mouseExited, this);
}

void CloseButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    current_pen_.SetColour(pressed_color_);
    current_brush_.SetColour(pressed_color_);

    dc.SetPen(current_pen_);
    dc.SetBrush(current_brush_);

    dc.DrawRectangle(button_rect_);

    current_pen_.SetColour(wxColour(217, 162, 179));
    current_brush_.SetColour(wxColour(217, 162, 179));

    dc.SetPen(current_pen_);
    dc.SetBrush(current_brush_);

    icon_draw_function_(dc, button_rect_.GetWidth());
}

void CloseButton::mouseEntered(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_HAND));
}

void CloseButton::mouseExited(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_ARROW));
    Refresh();
}

void CloseButton::mouseLeftReleased(wxMouseEvent& event)
{
    // Refresh();
    button_pressed_callback_();
}

void CloseButton::mouseLeftPressed(wxMouseEvent& event)
{
    // button_pressed_ = true;

    Refresh();
}
