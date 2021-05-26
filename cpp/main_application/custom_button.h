#ifndef CUSTOM_BUTTON_DVS_H_
#define CUSTOM_BUTTON_DVS_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/wx.h>

class CustomButton : public wxPanel
{
public:
    CustomButton() = delete;
    CustomButton(wxFrame* parent, const wxPoint& pos, const wxSize& size) : 
        wxPanel(parent, wxID_ANY, pos, size)
    {
        num_timer_iterations_clicked_ = 0;
        num_timer_iterations_entered_ = 0;

        size_ = size;
        pos_ = pos;
        shadow_margin_ = 5;
        circle_radius_ = 10.0;
        button_size_ = wxSize(size.GetWidth() - shadow_margin_, size.GetHeight() - shadow_margin_);

        original_color_ = wxColor(30, 174, 152);
        button_color_ = original_color_;

        pen = wxPen(button_color_, 1);
        brush = wxBrush(button_color_, wxBRUSHSTYLE_SOLID);
        shadow_pen = wxPen(wxColor(0, 0, 0, 0), 1);
        shadow_brush = wxBrush(wxColor(0, 0, 0, 127), wxBRUSHSTYLE_SOLID);
        circle_pen = wxPen(wxColor(255, 255, 255, 0), 1);
        circle_brush = wxBrush(wxColor(255, 255, 255, 127), wxBRUSHSTYLE_SOLID);
        button_rect = wxRect(0, 0, button_size_.GetWidth(), button_size_.GetHeight());
        shadow_rect = wxRect(shadow_margin_, shadow_margin_, button_size_.GetWidth(), button_size_.GetHeight());
        draw_circle_ = false;
        circle_alpha_ = 127;

        mouse_click_pos_ = wxPoint(30, 30);
        color_inc_ = -2;

        Bind(wxEVT_PAINT, &CustomButton::OnPaint, this);
        Bind(wxEVT_LEFT_DOWN, &CustomButton::mouseLeftPressed, this);
        Bind(wxEVT_ENTER_WINDOW, &CustomButton::mouseEntered, this);
        Bind(wxEVT_LEAVE_WINDOW, &CustomButton::mouseExited, this);
        // Bind(wxEVT_SIZE, &CustomButton::OnSize, this);
        click_timer_.Bind(wxEVT_TIMER, &CustomButton::OnClickedTimer, this);
        entered_timer_.Bind(wxEVT_TIMER, &CustomButton::OnEnteredTimer, this);
        
    }

private:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    void OnClickedTimer(wxTimerEvent& event);
    void OnEnteredTimer(wxTimerEvent& event);

    void drawWithCircle();
    void drawNormal();

    wxPoint mouse_click_pos_;
    wxSize size_;
    wxPoint pos_;
    wxSize button_size_;
    double circle_radius_;
    int num_timer_iterations_clicked_;
    int num_timer_iterations_entered_;
    int shadow_margin_;
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

    if(draw_circle_)
    {
        circle_brush.SetColour(wxColour(255, 255, 255, circle_alpha_));
        dc.SetPen(circle_pen);
        dc.SetBrush(circle_brush);
        dc.DrawCircle(mouse_click_pos_.x, mouse_click_pos_.y, circle_radius_);
    }
}

inline void CustomButton::OnPaint(wxPaintEvent& event)
{
    drawNormal();
}

inline void CustomButton::mouseEntered(wxMouseEvent& event)
{
    wxSetCursor(wxCursor(wxCURSOR_HAND));
    if(!entered_timer_.IsRunning())
    {
        color_inc_ = -2;
        entered_timer_.Start(10);
    }
}

inline void CustomButton::mouseExited(wxMouseEvent& event)
{
    wxSetCursor(wxCursor(wxCURSOR_ARROW));
    click_timer_.Stop();
    entered_timer_.Stop();
    draw_circle_ = false;
    num_timer_iterations_entered_ = 0;
    color_inc_ = 2;
    entered_timer_.Start(10);
    Refresh();
}

inline void CustomButton::mouseLeftPressed(wxMouseEvent& event)
{
    if(!click_timer_.IsRunning())
    {
        circle_radius_ = 10.0;
        mouse_click_pos_ = event.GetPosition();
        click_timer_.Start(20);
    }
}

inline void CustomButton::OnClickedTimer(wxTimerEvent& event)
{
    if(num_timer_iterations_clicked_ > 20)
    {
        click_timer_.Stop();
        num_timer_iterations_clicked_ = 0;
        circle_alpha_ = 127;
        draw_circle_ = false;
    }
    else
    {
        draw_circle_ = true;
        num_timer_iterations_clicked_++;
    }
    Refresh();
    circle_radius_ += 4.0;
    circle_alpha_ = std::max(0, circle_alpha_ - 10);
    
}

inline void CustomButton::OnEnteredTimer(wxTimerEvent& event)
{
    if(num_timer_iterations_entered_ > 10)
    {
        entered_timer_.Stop();
        num_timer_iterations_entered_ = 0;
    }
    else
    {
        num_timer_iterations_entered_++;
    }
    button_color_ = wxColour(button_color_.Red() + color_inc_,
                             button_color_.Green() + color_inc_,
                             button_color_.Blue() + color_inc_);
    pen = wxPen(button_color_, 1);
    brush = wxBrush(button_color_, wxBRUSHSTYLE_SOLID);

    Refresh();
}

inline void CustomButton::OnSize(wxSizeEvent& event)
{
    // this->Set
}

#endif
