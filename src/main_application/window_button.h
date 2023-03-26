#ifndef MAIN_APPLICATION_WINDOW_BUTTON_H_
#define MAIN_APPLICATION_WINDOW_BUTTON_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <functional>

#include "project_state/project_settings.h"
#include "tab_button.h"

class WindowButton : public wxPanel
{
public:
    WindowButton() = delete;
    WindowButton(wxFrame* parent,
                 const TabSettings& tab_settings,
                 const wxPoint& pos,
                 const wxSize& size,
                 const int id,
                 const std::function<void(std::string)> button_pressed_callback);
    int getId() const;
    std::string getButtonLabel() const;
    void setButtonLabel(const std::string& new_label);

private:
    void OnPaint(wxPaintEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    void OnEnteredTimer(wxTimerEvent& event);

    TabSettings tab_settings_;
    std::string button_label_;
    int id_;
    std::function<void(std::string)> button_pressed_callback_;

    bool button_pressed_;

    int num_timer_iterations_entered_;
    bool entered_timer_running_;

    ColorPair unselected_color_pair_;
    ColorPair pressed_color_pair_;

    wxColour unselected_color_;
    wxColour pressed_color_;

    wxPen current_pen_;
    wxBrush current_brush_;

    wxRect button_rect_;

    wxTimer entered_timer_;
};

#endif  // MAIN_APPLICATION_WINDOW_BUTTON_H_
