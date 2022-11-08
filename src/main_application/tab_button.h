#ifndef MAIN_APPLICATION_TAB_BUTTON_H_
#define MAIN_APPLICATION_TAB_BUTTON_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <functional>

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

class TabButton : public wxPanel
{
public:
    TabButton() = delete;
    TabButton(wxFrame* parent,
              const std::string& button_label,
              const std::function<void(std::string)> button_pressed_callback,
              const int id,
              const wxPoint& pos,
              const wxSize& size,
              const std::function<void(const wxPoint pos, const std::string& item_name)>&
                  notify_parent_window_right_mouse_pressed);
    int getId() const;
    void setSelected();
    void setDeselected();
    bool isSelected() const;
    std::string getButtonLabel() const;
    void setButtonLabel(const std::string& new_label);
    std::function<void(const wxPoint pos, const std::string& item_name)> notify_parent_window_right_mouse_pressed_;

private:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseRightPressed(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    void OnEnteredTimer(wxTimerEvent& event);
    void OnExitedTimer(wxTimerEvent& event);

    void drawWithCircle();
    void drawNormal();
    void drawPressed();

    bool button_pressed_;
    bool is_selected_;

    int id_;
    std::string button_label_;
    std::function<void(std::string)> button_pressed_callback_;
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

    wxPen selected_pen;
    wxBrush selected_brush;

    wxPen shadow_pen;
    wxBrush shadow_brush;
    wxPen circle_pen;
    wxBrush circle_brush;
    wxPen pressed_pen;
    wxBrush pressed_brush;
    wxRect button_rect;
    // wxRect shadow_rect;
    int circle_alpha_;

    wxTimer entered_timer_;
    wxTimer exited_timer_;
};

#endif  // MAIN_APPLICATION_TAB_BUTTON_H_
