#ifndef MAIN_APPLICATION_CLOSE_BUTTONS_H
#define MAIN_APPLICATION_CLOSE_BUTTONS_H

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <functional>

class CloseButton : public wxPanel
{
private:
    std::function<void()> button_pressed_callback_;

    bool button_pressed_;

    wxColour pressed_color_;

    wxPen current_pen_;
    wxBrush current_brush_;

    wxRect button_rect_;

    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);

    std::function<void(wxPaintDC& dc, const int button_size)> icon_draw_function_;

public:
    CloseButton() = delete;
    CloseButton(wxFrame* parent,
                const wxPoint& pos,
                const wxSize& size,
                const wxColour& color,
                const std::function<void()>& button_pressed_callback,
                const std::function<void(wxPaintDC& dc, const int button_size)>& icon_draw_function);
};

#endif  // MAIN_APPLICATION_CLOSE_BUTTONS_H
