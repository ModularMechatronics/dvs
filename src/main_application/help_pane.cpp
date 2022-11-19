#include "help_pane.h"

constexpr int kPaneWidth = 250;

HelpPane::HelpPane(wxFrame* parent, const wxPoint& pos, const wxSize& size, const wxSize& parent_size)
    : wxPanel(parent, wxID_ANY, wxPoint(150, 150), wxSize(kPaneWidth, 120))
{
    size_ = size;
    pos_ = pos;

    original_color_ = wxColor(0, 0, 0, 150);

    pen_ = wxPen(original_color_, 1);
    brush_ = wxBrush(original_color_, wxBRUSHSTYLE_SOLID);

    rect_ = wxRect(0, 0, kPaneWidth, 120);

    Bind(wxEVT_PAINT, &HelpPane::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &HelpPane::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &HelpPane::mouseLeftReleased, this);
    Bind(wxEVT_ENTER_WINDOW, &HelpPane::mouseEntered, this);
    Bind(wxEVT_LEAVE_WINDOW, &HelpPane::mouseExited, this);
    Bind(wxEVT_RIGHT_DOWN, &HelpPane::mouseRightPressed, this);

    const int text_color = 220;
    SetForegroundColour(wxColor(text_color, text_color, text_color));
    hide();
}

void HelpPane::updateParentSize(const wxSize& parent_size)
{
    parent_size_ = parent_size;
    Refresh();
}

void HelpPane::drawNormal()
{
    wxPaintDC dc(this);

    dc.SetPen(*wxTRANSPARENT_PEN);
    dc.SetBrush(brush_);

    this->SetPosition(wxPoint(parent_size_.GetWidth() / 2 - kPaneWidth / 2, parent_size_.GetHeight() / 2 - 120 / 2));

    dc.DrawRoundedRectangle(rect_, 5.0);

    const int dy = 20;
    const int y_offset = 10;
    dc.DrawText("h = Show help", wxPoint(10, y_offset));
    dc.DrawText("r = Rotate", wxPoint(10, y_offset + dy));
    dc.DrawText("t = Translate/Pan", wxPoint(10, y_offset + 2 * dy));
    dc.DrawText("z = Zoom", wxPoint(10, y_offset + 3 * dy));
    dc.DrawText("1/2/3 = Lock interaction to z/y/z axis", wxPoint(10, y_offset + 4 * dy));
}

void HelpPane::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    drawNormal();
}

void HelpPane::mouseEntered(wxMouseEvent& WXUNUSED(event))
{
    // wxSetCursor(wxCursor(wxCURSOR_HAND));

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

void HelpPane::mouseExited(wxMouseEvent& WXUNUSED(event))
{
    // wxSetCursor(wxCursor(wxCURSOR_ARROW));

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

void HelpPane::mouseRightPressed(wxMouseEvent& event) {}

void HelpPane::show()
{
    Raise();
    this->Show();
}
void HelpPane::hide()
{
    this->Hide();
}

void HelpPane::mouseLeftReleased(wxMouseEvent& event) {}

void HelpPane::mouseLeftPressed(wxMouseEvent& event) {}
