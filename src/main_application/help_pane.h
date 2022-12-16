#ifndef MAIN_APPLICATION_HELP_PANE_H_
#define MAIN_APPLICATION_HELP_PANE_H_

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/wx.h>

#include <functional>

class HelpPane : public wxPanel
{
public:
    HelpPane() = delete;
    HelpPane(wxFrame* parent, const wxPoint& pos, const wxSize& size, const wxSize& parent_size);

    void show();
    void hide();
    void updateParentSize(const wxSize& parent_size);

private:
    void OnPaint(wxPaintEvent& event);
    void OnSize(wxSizeEvent& event);
    void mouseLeftPressed(wxMouseEvent& event);
    void mouseLeftReleased(wxMouseEvent& event);
    void mouseRightPressed(wxMouseEvent& event);
    void mouseEntered(wxMouseEvent& event);
    void mouseExited(wxMouseEvent& event);
    wxSize parent_size_;

    void drawNormal();

    wxSize size_;
    wxPoint pos_;
    wxColour original_color_;

    wxPen pen_;
    wxBrush brush_;
    wxRect rect_;
};

#endif  // MAIN_APPLICATION_HELP_PANE_H_
