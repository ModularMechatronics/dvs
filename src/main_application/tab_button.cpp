#include "tab_button.h"

TabButton::TabButton(wxFrame* parent,
                     const std::string& button_label,
                     const std::function<void(std::string)> button_pressed_callback,
                     const int id,
                     const wxPoint& pos,
                     const wxSize& size,
                     const std::function<void(const wxPoint pos)>& notify_parent_window_right_mouse_pressed)
    : wxPanel(parent, wxID_ANY, pos, size),
      notify_parent_window_right_mouse_pressed_{notify_parent_window_right_mouse_pressed}
{
    button_pressed_callback_ = button_pressed_callback;
    button_label_ = button_label;
    id_ = id;
    num_timer_iterations_clicked_ = 0;
    num_timer_iterations_entered_ = 0;

    size_ = size;
    pos_ = pos;
    circle_radius_ = kInitialCircleRadius;
    button_size_ = wxSize(size.GetWidth(), size.GetHeight());

    original_color_ = wxColor(30, 174, 152);
    button_color_ = original_color_;

    pen = wxPen(button_color_, 1);
    brush = wxBrush(button_color_, wxBRUSHSTYLE_SOLID);
    shadow_pen = wxPen(wxColor(0, 0, 0, 0), 1);
    shadow_brush = wxBrush(wxColor(0, 0, 0, 127), wxBRUSHSTYLE_SOLID);
    circle_pen = wxPen(wxColor(255, 255, 255, 0), 1);
    circle_brush = wxBrush(wxColor(255, 255, 255, 127), wxBRUSHSTYLE_SOLID);

    pressed_pen = wxPen(wxColor(220, 154, 132), 255);
    pressed_brush = wxBrush(wxColor(220, 154, 132, 255), wxBRUSHSTYLE_SOLID);

    selected_pen = wxPen(wxColor(120, 154, 232), 255);
    selected_brush = wxBrush(wxColor(120, 154, 232, 255), wxBRUSHSTYLE_SOLID);

    button_rect = wxRect(0, 0, button_size_.GetWidth(), button_size_.GetHeight());
    // shadow_rect = wxRect(kShadowMargin, kShadowMargin, button_size_.GetWidth(), button_size_.GetHeight());
    draw_circle_ = false;
    circle_alpha_ = kInitialAlpha;
    button_pressed_ = false;
    is_selected_ = false;

    mouse_click_pos_ = wxPoint(30, 30);
    color_inc_ = -kColorInc;

    // wxStaticText* staticText = new wxStaticText(this, wxID_ANY, button_label_);

    Bind(wxEVT_PAINT, &TabButton::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &TabButton::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &TabButton::mouseLeftReleased, this);
    Bind(wxEVT_ENTER_WINDOW, &TabButton::mouseEntered, this);
    Bind(wxEVT_LEAVE_WINDOW, &TabButton::mouseExited, this);
    Bind(wxEVT_RIGHT_DOWN, &TabButton::mouseRightPressed, this);
    // Bind(wxEVT_SIZE, &TabButton::OnSize, this);
    entered_timer_.Bind(wxEVT_TIMER, &TabButton::OnEnteredTimer, this);
    exited_timer_.Bind(wxEVT_TIMER, &TabButton::OnExitedTimer, this);
}

void TabButton::drawNormal()
{
    wxPaintDC dc(this);

    if (is_selected_)
    {
        dc.SetPen(selected_pen);
        dc.SetBrush(selected_brush);
    }
    else
    {
        dc.SetPen(pen);
        dc.SetBrush(brush);
    }

    dc.DrawRoundedRectangle(button_rect, 5.0);

    dc.DrawText(button_label_, wxPoint(0, 0));
}

void TabButton::drawPressed()
{
    wxPaintDC dc(this);

    dc.SetPen(pressed_pen);
    dc.SetBrush(pressed_brush);

    dc.DrawRoundedRectangle(button_rect, 5.0);

    dc.DrawText(button_label_, wxPoint(0, 0));
}

void TabButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    if (button_pressed_)
    {
        drawPressed();
    }
    else
    {
        drawNormal();
    }
}

void TabButton::mouseEntered(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_HAND));

    /*exited_timer_.Stop();
    while (exited_timer_.IsRunning())
    {
        wxMilliSleep(1);
    }*/
    // if(!entered_timer_.IsRunning())
    // {
    // color_inc_ = -kColorInc;
    // entered_timer_.Start(kEnteredPeriod);
    // }
}

void TabButton::mouseExited(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_ARROW));

    /*entered_timer_.Stop();
    while (entered_timer_.IsRunning())
    {
        wxMilliSleep(1);
    }

    // draw_circle_ = false;

    // num_timer_iterations_entered_ = 0;
    color_inc_ = kColorInc;
    exited_timer_.Start(kEnteredPeriod);
    Refresh();*/
}

int TabButton::getId() const
{
    return id_;
}

void TabButton::setSelected()
{
    is_selected_ = true;
    Refresh();
}

void TabButton::setDeselected()
{
    is_selected_ = false;
    Refresh();
}

void TabButton::mouseRightPressed(wxMouseEvent& event)
{
    notify_parent_window_right_mouse_pressed_(event.GetPosition());
}

void TabButton::mouseLeftReleased(wxMouseEvent& event)
{
    std::cout << "Released..." << std::endl;
    button_pressed_ = false;
    Refresh();
}

void TabButton::mouseLeftPressed(wxMouseEvent& event)
{
    // is_selected_ = true;
    button_pressed_ = true;
    Refresh();
    std::cout << "Clicked!" << std::endl;
    button_pressed_callback_(button_label_);
    /*if (click_timer_.IsRunning())
    {
        click_timer_.Stop();
        num_timer_iterations_clicked_ = 0;
        circle_alpha_ = kInitialAlpha;
        draw_circle_ = false;
    }

    circle_radius_ = kInitialCircleRadius;
    mouse_click_pos_ = event.GetPosition();
    click_timer_.Start(kCirclePeriod);*/
}

void TabButton::OnExitedTimer(wxTimerEvent& WXUNUSED(event))
{
    button_color_ = wxColour(
        button_color_.Red() + color_inc_, button_color_.Green() + color_inc_, button_color_.Blue() + color_inc_);
    pen = wxPen(button_color_, 1);
    brush = wxBrush(button_color_, wxBRUSHSTYLE_SOLID);

    Refresh();

    if (num_timer_iterations_entered_ <= 0)
    {
        exited_timer_.Stop();
        num_timer_iterations_entered_ = 0;
        button_color_ = original_color_;
    }
    else
    {
        num_timer_iterations_entered_--;
    }
}

void TabButton::OnEnteredTimer(wxTimerEvent& WXUNUSED(event))
{
    button_color_ = wxColour(
        button_color_.Red() + color_inc_, button_color_.Green() + color_inc_, button_color_.Blue() + color_inc_);
    pen = wxPen(button_color_, 1);
    brush = wxBrush(button_color_, wxBRUSHSTYLE_SOLID);

    Refresh();

    if (num_timer_iterations_entered_ >= (kNumEnteredIterationsMax - 1))
    {
        entered_timer_.Stop();
        // num_timer_iterations_entered_ = 0;
    }
    else
    {
        num_timer_iterations_entered_++;
    }
}

void TabButton::OnSize(wxSizeEvent& WXUNUSED(event))
{
    // this->Set
}
