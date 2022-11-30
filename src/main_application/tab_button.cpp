#include "tab_button.h"

constexpr int kEnteredPeriod = 15;
constexpr int kNumEnteredIterationsMax = 10;

wxColor RGBTripletfToWxColour(const RGBTripletf& c)
{
    return wxColour(c.red * 255.0f, c.green * 255.0f, c.blue * 255.0f);
}

TabButton::TabButton(wxFrame* parent,
                     const TabSettings& tab_settings,
                     const wxPoint& pos,
                     const wxSize& size,
                     const int id,
                     const std::function<void(std::string)> button_pressed_callback,
                     const std::function<void(const wxPoint pos, const std::string& item_name)>&
                         notify_parent_window_right_mouse_pressed)
    : wxPanel(parent, wxID_ANY, pos, size),
      tab_settings_{tab_settings},
      button_label_{tab_settings.name},
      id_{id},
      button_pressed_callback_{button_pressed_callback},
      notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed}
{
    unselected_color_pair_ = ColorPair(RGBTripletfToWxColour(tab_settings_.button_normal_color), 0.7f);
    pressed_color_pair_ = ColorPair(RGBTripletfToWxColour(tab_settings_.button_clicked_color), 0.7f);
    selected_color_pair_ = ColorPair(RGBTripletfToWxColour(tab_settings_.button_selected_color), 0.7f);

    pressed_color_ = pressed_color_pair_.base_color;
    selected_color_ = selected_color_pair_.base_color;
    unselected_color_ = unselected_color_pair_.base_color;

    button_rect_ = wxRect(0, 0, size.GetWidth(), size.GetHeight());

    num_timer_iterations_entered_ = 0;

    entered_timer_running_ = false;
    button_pressed_ = false;
    is_selected_ = false;

    entered_timer_.Bind(wxEVT_TIMER, &TabButton::OnEnteredTimer, this);

    Bind(wxEVT_PAINT, &TabButton::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &TabButton::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &TabButton::mouseLeftReleased, this);
    Bind(wxEVT_ENTER_WINDOW, &TabButton::mouseEntered, this);
    Bind(wxEVT_LEAVE_WINDOW, &TabButton::mouseExited, this);
    Bind(wxEVT_RIGHT_DOWN, &TabButton::mouseRightPressed, this);
}

void TabButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    if (button_pressed_)
    {
        current_pen_.SetColour(pressed_color_);
        current_brush_.SetColour(pressed_color_);
    }
    else
    {
        if (is_selected_)
        {
            current_pen_.SetColour(selected_color_);
            current_brush_.SetColour(selected_color_);
        }
        else
        {
            current_pen_.SetColour(unselected_color_);
            current_brush_.SetColour(unselected_color_);
        }
    }
    dc.SetPen(current_pen_);
    dc.SetBrush(current_brush_);

    const int text_height = dc.GetCharHeight();

    dc.DrawRectangle(button_rect_);
    dc.DrawText(button_label_, wxPoint(5, button_rect_.height / 2 - text_height / 2 - 3));
}

void TabButton::mouseEntered(wxMouseEvent& WXUNUSED(event))
{
    if (!is_selected_)
    {
        wxSetCursor(wxCursor(wxCURSOR_HAND));

        entered_timer_running_ = true;
        entered_timer_.Start(kEnteredPeriod);
    }
}

void TabButton::mouseExited(wxMouseEvent& WXUNUSED(event))
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

    selected_color_ = selected_color_pair_.base_color;
    unselected_color_ = unselected_color_pair_.base_color;

    Refresh();
}

void TabButton::mouseRightPressed(wxMouseEvent& event)
{
    notify_parent_window_right_mouse_pressed_(this->GetPosition() + event.GetPosition(), button_label_);
}

void TabButton::mouseLeftReleased(wxMouseEvent& event)
{
    button_pressed_ = false;
    selected_color_ = selected_color_pair_.base_color;
    unselected_color_ = unselected_color_pair_.base_color;
    wxSetCursor(wxCursor(wxCURSOR_ARROW));
    Refresh();
}

void TabButton::mouseLeftPressed(wxMouseEvent& event)
{
    if (!is_selected_)
    {
        button_pressed_ = true;
        button_pressed_callback_(button_label_);
        Refresh();
    }
}

void TabButton::OnEnteredTimer(wxTimerEvent& WXUNUSED(event))
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
        selected_color_ = selected_color_pair_(num_timer_iterations_entered_ * 6);
        unselected_color_ = unselected_color_pair_(num_timer_iterations_entered_ * 6);
        num_timer_iterations_entered_++;
    }

    Refresh();
}

std::string TabButton::getButtonLabel() const
{
    return button_label_;
}

void TabButton::setButtonLabel(const std::string& new_label)
{
    button_label_ = new_label;
    Refresh();
}

bool TabButton::isSelected() const
{
    return is_selected_;
}

void TabButton::show()
{
    this->Show();
}
void TabButton::hide()
{
    this->Hide();
}

int TabButton::getId() const
{
    return id_;
}

void TabButton::setSelected()
{
    is_selected_ = true;
}

void TabButton::setDeselected()
{
    is_selected_ = false;
    Refresh();
}
