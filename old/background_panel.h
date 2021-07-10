#ifndef DVS_BACKGROUND_PANEL_H_
#define DVS_BACKGROUND_PANEL_H_

#include <wx/wxprec.h>
#include <wx/wfstream.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/notebook.h>
#include <wx/textctrl.h>
#include <wx/timer.h>
#include <wx/menu.h>
#include <wx/wx.h>

class BackgroundHalfPanel : public wxPanel
{
public:
    BackgroundHalfPanel() = delete;
    BackgroundHalfPanel(wxFrame* parent,
                        const wxPoint& pos,
                        const wxSize& size,
                        const wxColor& bottom_color,
                        const wxColor& top_color) : 
        wxPanel(parent, wxID_ANY, pos, size)
    {
        bottom_color_ = bottom_color;
        top_color_ = top_color;
        Bind(wxEVT_PAINT, &BackgroundHalfPanel::OnPaint, this);
        this->Show();
    }

    void setSizeAndPos(const wxPoint& new_pos, const wxSize& new_size)
    {
        this->SetPosition(new_pos);
        this->SetSize(new_size);
    }

private:
    wxColor bottom_color_;
    wxColor top_color_;

    void OnPaint(wxPaintEvent& WXUNUSED(event))
    {
        wxPaintDC dc(this);
        wxPen pen = wxPen(wxColor(0, 0, 0, 255), 1);
        wxBrush brush = wxBrush(wxColor(0, 0, 0, 127), wxBRUSHSTYLE_SOLID);

        dc.SetPen(pen);
        dc.SetBrush(brush);
        const wxSize current_size = this->GetSize();
        wxRect button_rect = wxRect(0, 0, current_size.GetWidth(), current_size.GetHeight());

        dc.GradientFillLinear(button_rect, top_color_, bottom_color_, wxDOWN);
    }

};

class BackgroundPanel
{
private:
    BackgroundHalfPanel* top_;
    BackgroundHalfPanel* bottom_;
public:
    BackgroundPanel() = delete;
    BackgroundPanel(wxFrame* parent, const wxSize& size, const wxColor& middle_color, const wxColor& outer_color)
    {
        top_ = new BackgroundHalfPanel(parent, wxPoint(0, 0), size, middle_color, outer_color);
        bottom_ = new BackgroundHalfPanel(parent, wxPoint(0, 0), size, outer_color, middle_color);
        resize(size);
    }

    void resize(const wxSize& new_size)
    {
        const float height = new_size.GetHeight();
        const int half_height = std::round(height / 2.0f);
        const wxSize half_size = wxSize(new_size.GetWidth(), half_height);

        top_->setSizeAndPos(wxPoint(0, 0), half_size);
        bottom_->setSizeAndPos(wxPoint(0, half_height), half_size);
    }

};


#endif
