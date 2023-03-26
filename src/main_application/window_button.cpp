#include "window_button.h"

constexpr int kEnteredPeriod = 15;
constexpr int kNumEnteredIterationsMax = 10;

WindowButton::WindowButton(wxFrame* parent,
                           const TabSettings& tab_settings,
                           const wxPoint& pos,
                           const wxSize& size,
                           const int id,
                           const std::function<void(std::string)> button_pressed_callback)
    : wxPanel(parent, wxID_ANY, pos, size),
      tab_settings_{tab_settings},
      button_label_{tab_settings.name},
      id_{id},
      button_pressed_callback_{button_pressed_callback}
{
    unselected_color_pair_ = ColorPair(RGBTripletfToWxColour(tab_settings_.button_normal_color), 0.7f);
    pressed_color_pair_ = ColorPair(RGBTripletfToWxColour(tab_settings_.button_clicked_color), 0.7f);

    pressed_color_ = pressed_color_pair_.base_color;
    unselected_color_ = unselected_color_pair_.base_color;

    button_rect_ = wxRect(0, 0, size.GetWidth(), size.GetHeight());

    num_timer_iterations_entered_ = 0;

    entered_timer_running_ = false;
    button_pressed_ = false;

    entered_timer_.Bind(wxEVT_TIMER, &WindowButton::OnEnteredTimer, this);

    Bind(wxEVT_PAINT, &WindowButton::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &WindowButton::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &WindowButton::mouseLeftReleased, this);
    Bind(wxEVT_ENTER_WINDOW, &WindowButton::mouseEntered, this);
    Bind(wxEVT_LEAVE_WINDOW, &WindowButton::mouseExited, this);
}

void WindowButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    if (button_pressed_)
    {
        current_pen_.SetColour(pressed_color_);
        current_brush_.SetColour(pressed_color_);
    }
    else
    {
        current_pen_.SetColour(unselected_color_);
        current_brush_.SetColour(unselected_color_);
    }
    dc.SetPen(current_pen_);
    dc.SetBrush(current_brush_);

    const int text_height = dc.GetCharHeight();
    const int text_width = dc.GetCharWidth() * button_label_.length();

    dc.DrawRectangle(button_rect_);
    dc.DrawText(button_label_,
                wxPoint(button_rect_.width / 2 - text_width / 2, button_rect_.height / 2 - text_height / 2 - 3));
}

void WindowButton::mouseEntered(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_HAND));

    entered_timer_running_ = true;
    entered_timer_.Start(kEnteredPeriod);
}

void WindowButton::mouseExited(wxMouseEvent& WXUNUSED(event))
{
    entered_timer_running_ = false;

    wxSetCursor(wxCursor(wxCURSOR_ARROW));

    if (entered_timer_.IsRunning())
    {
        entered_timer_.Stop();
        while (entered_timer_.IsRunning())
        {
            wxMilliSleep(1);
        }
    }

    unselected_color_ = unselected_color_pair_.base_color;

    Refresh();
}

void WindowButton::mouseLeftReleased(wxMouseEvent& event)
{
    button_pressed_ = false;
    Refresh();
}

void WindowButton::mouseLeftPressed(wxMouseEvent& event)
{
    button_pressed_ = true;
    button_pressed_callback_(button_label_);
    Refresh();
}

void WindowButton::OnEnteredTimer(wxTimerEvent& WXUNUSED(event))
{
    if (!entered_timer_running_)
    {
        return;
    }

    if (num_timer_iterations_entered_ >= (kNumEnteredIterationsMax - 1))
    {
        entered_timer_running_ = false;
        entered_timer_.Stop();
        num_timer_iterations_entered_ = 0;
    }
    else
    {
        unselected_color_ = unselected_color_pair_(num_timer_iterations_entered_ * 6);
        num_timer_iterations_entered_++;
    }

    Refresh();
}

std::string WindowButton::getButtonLabel() const
{
    return button_label_;
}

void WindowButton::setButtonLabel(const std::string& new_label)
{
    button_label_ = new_label;
    Refresh();
}

int WindowButton::getId() const
{
    return id_;
}
