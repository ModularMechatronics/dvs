#ifndef MAIN_APPLICATION_CUSTOM_BUTTON_H_
#define MAIN_APPLICATION_CUSTOM_BUTTON_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

constexpr int kCirclePeriod = 20;
constexpr int kEnteredPeriod = 10;
constexpr int kNumCircleIterationsMax = 20;
constexpr int kNumEnteredIterationsMax = 10;
constexpr int kInitialAlpha = 127;
constexpr int kShadowMargin = 5;
constexpr int kColorInc = 2;
constexpr int kCircleAlphaDec = 10;
constexpr double kCircleRadiusInc = 4.0;
constexpr double kInitialCircleRadius = 10.0;

class CustomButton : public wxPanel
{
public:
    CustomButton() = delete;
    CustomButton(wxFrame* parent, const wxPoint& pos, const wxSize& size) : wxPanel(parent, wxID_ANY, pos, size)
    {
        num_timer_iterations_clicked_ = 0;
        num_timer_iterations_entered_ = 0;

        size_ = size;
        pos_ = pos;
        circle_radius_ = kInitialCircleRadius;
        button_size_ = wxSize(size.GetWidth() - kShadowMargin, size.GetHeight() - kShadowMargin);

        original_color_ = wxColor(30, 174, 152);
        button_color_ = original_color_;

        pen = wxPen(button_color_, 1);
        brush = wxBrush(button_color_, wxBRUSHSTYLE_SOLID);
        shadow_pen = wxPen(wxColor(0, 0, 0, 0), 1);
        shadow_brush = wxBrush(wxColor(0, 0, 0, 127), wxBRUSHSTYLE_SOLID);
        circle_pen = wxPen(wxColor(255, 255, 255, 0), 1);
        circle_brush = wxBrush(wxColor(255, 255, 255, 127), wxBRUSHSTYLE_SOLID);
        button_rect = wxRect(0, 0, button_size_.GetWidth(), button_size_.GetHeight());
        shadow_rect = wxRect(kShadowMargin, kShadowMargin, button_size_.GetWidth(), button_size_.GetHeight());
        draw_circle_ = false;
        circle_alpha_ = kInitialAlpha;

        mouse_click_pos_ = wxPoint(30, 30);
        color_inc_ = -kColorInc;

        Bind(wxEVT_PAINT, &CustomButton::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &CustomButton::mouseLeftPressed, this);
        Bind(wxEVT_ENTER_WINDOW, &CustomButton::mouseEntered, this);
        Bind(wxEVT_LEAVE_WINDOW, &CustomButton::mouseExited, this);
        // Bind(wxEVT_SIZE, &CustomButton::OnSize, this);
        click_timer_.Bind(wxEVT_TIMER, &CustomButton::OnClickedTimer, this);
        entered_timer_.Bind(wxEVT_TIMER, &CustomButton::OnEnteredTimer, this);
        exited_timer_.Bind(wxEVT_TIMER, &CustomButton::OnExitedTimer, this);
    }

private:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    void OnClickedTimer(wxTimerEvent& event);
    void OnEnteredTimer(wxTimerEvent& event);
    void OnExitedTimer(wxTimerEvent& event);

    void drawWithCircle();
    void drawNormal();

    wxPoint mouse_click_pos_;
    wxSize size_;
    wxPoint pos_;
    wxSize button_size_;
    double circle_radius_;
    int num_timer_iterations_clicked_;
    int num_timer_iterations_entered_;
    bool draw_circle_;
    wxColour original_color_;
    wxColour button_color_;
    int color_inc_;

    wxPen pen;
    wxBrush brush;
    wxPen shadow_pen;
    wxBrush shadow_brush;
    wxPen circle_pen;
    wxBrush circle_brush;
    wxRect button_rect;
    wxRect shadow_rect;
    int circle_alpha_;

    wxTimer click_timer_;
    wxTimer entered_timer_;
    wxTimer exited_timer_;
};

inline void CustomButton::drawNormal()
{
    wxPaintDC dc(this);

    dc.SetPen(shadow_pen);
    dc.SetBrush(shadow_brush);

    dc.DrawRoundedRectangle(shadow_rect, 5.0);

    dc.SetPen(pen);
    dc.SetBrush(brush);

    dc.DrawRoundedRectangle(button_rect, 5.0);

    // dc.DrawText("Some text", wxPoint(0, 0));

    if (draw_circle_)
    {
        circle_brush.SetColour(wxColour(255, 255, 255, circle_alpha_));
        dc.SetPen(circle_pen);
        dc.SetBrush(circle_brush);
        dc.DrawCircle(mouse_click_pos_.x, mouse_click_pos_.y, circle_radius_);
    }
}

inline void CustomButton::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    drawNormal();
}

inline void CustomButton::mouseEntered(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_HAND));

    exited_timer_.Stop();
    while (exited_timer_.IsRunning())
    {
        wxMilliSleep(1);
    }
    // if(!entered_timer_.IsRunning())
    // {
    color_inc_ = -kColorInc;
    entered_timer_.Start(kEnteredPeriod);
    // }
}

inline void CustomButton::mouseExited(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_ARROW));

    entered_timer_.Stop();
    while (entered_timer_.IsRunning())
    {
        wxMilliSleep(1);
    }

    // draw_circle_ = false;
    std::cout << num_timer_iterations_entered_ << std::endl;

    // num_timer_iterations_entered_ = 0;
    color_inc_ = kColorInc;
    exited_timer_.Start(kEnteredPeriod);
    Refresh();
}

inline void CustomButton::mouseLeftPressed(wxMouseEvent& event)
{
    if (click_timer_.IsRunning())
    {
        click_timer_.Stop();
        num_timer_iterations_clicked_ = 0;
        circle_alpha_ = kInitialAlpha;
        draw_circle_ = false;
    }

    circle_radius_ = kInitialCircleRadius;
    mouse_click_pos_ = event.GetPosition();
    click_timer_.Start(kCirclePeriod);
}

inline void CustomButton::OnClickedTimer(wxTimerEvent& WXUNUSED(event))
{
    if (num_timer_iterations_clicked_ > kNumCircleIterationsMax)
    {
        click_timer_.Stop();
        num_timer_iterations_clicked_ = 0;
        circle_alpha_ = kInitialAlpha;
        draw_circle_ = false;
    }
    else
    {
        draw_circle_ = true;
        num_timer_iterations_clicked_++;
    }

    Refresh();
    circle_radius_ += kCircleRadiusInc;
    circle_alpha_ = std::max(0, circle_alpha_ - kCircleAlphaDec);
}

inline void CustomButton::OnExitedTimer(wxTimerEvent& WXUNUSED(event))
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

inline void CustomButton::OnEnteredTimer(wxTimerEvent& WXUNUSED(event))
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

inline void CustomButton::OnSize(wxSizeEvent& WXUNUSED(event))
{
    // this->Set
}

#endif // MAIN_APPLICATION_CUSTOM_BUTTON_H_
