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

#include "project_state/project_settings.h"

wxColor RGBTripletfToWxColour(const RGBTripletf& c);

struct ColorPair
{
    wxColour base_color;
    wxColour final_color;

    ColorPair() = default;
    ColorPair(const wxColour& base_color_, const float inc)
    {
        base_color = base_color_;
        const float r = std::min(static_cast<float>(base_color.Red()) * (1.0f + inc), 255.0f);
        const float g = std::min(static_cast<float>(base_color.Green()) * (1.0f + inc), 255.0f);
        const float b = std::min(static_cast<float>(base_color.Blue()) * (1.0f + inc), 255.0f);
        final_color = wxColour(r, g, b);
    }

    wxColour operator()(const size_t idx)
    {
        const size_t idx_in_interaval = std::min(idx, static_cast<size_t>(100U));

        const float idxf = static_cast<float>(idx_in_interaval) / 100.0f;
        const int dr = idxf * static_cast<float>(final_color.Red() - base_color.Red());
        const int dg = idxf * static_cast<float>(final_color.Green() - base_color.Green());
        const int db = idxf * static_cast<float>(final_color.Blue() - base_color.Blue());

        return wxColour(base_color.Red() + dr, base_color.Green() + dg, base_color.Blue() + db);
    }
};

class TabButton : public wxPanel
{
public:
    TabButton() = delete;
    TabButton(wxFrame* parent,
              const TabSettings& tab_settings,
              const wxPoint& pos,
              const wxSize& size,
              const int id,
              const std::function<void(std::string)> button_pressed_callback,
              const std::function<void(const wxPoint pos, const std::string& item_name)>&
                  notify_parent_window_right_mouse_pressed);
    int getId() const;
    void setSelected();
    void setDeselected();
    bool isSelected() const;
    std::string getButtonLabel() const;
    void setButtonLabel(const std::string& new_label);
    void show();
    void hide();

private:
    void OnPaint(wxPaintEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseRightPressed(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    void OnEnteredTimer(wxTimerEvent& event);

    TabSettings tab_settings_;
    std::string button_label_;
    int id_;
    std::function<void(std::string)> button_pressed_callback_;
    std::function<void(const wxPoint pos, const std::string& item_name)> notify_parent_window_right_mouse_pressed_;

    bool button_pressed_;
    bool is_selected_;

    int num_timer_iterations_entered_;
    bool entered_timer_running_;

    ColorPair unselected_color_pair_;
    ColorPair selected_color_pair_;
    ColorPair pressed_color_pair_;

    wxColour unselected_color_;
    wxColour selected_color_;
    wxColour pressed_color_;

    wxPen current_pen_;
    wxBrush current_brush_;

    wxRect button_rect_;

    wxTimer entered_timer_;
};

#endif  // MAIN_APPLICATION_TAB_BUTTON_H_
