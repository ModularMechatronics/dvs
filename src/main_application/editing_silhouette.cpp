#include "editing_silhouette.h"

EditingSilhouette::EditingSilhouette(wxFrame* parent, const wxPoint& pos, const wxSize& size)
    : wxPanel(parent, wxID_ANY, pos, size)
{
    Bind(wxEVT_PAINT, &EditingSilhouette::OnPaint, this);

    wxColour col_pen{0, 0, 0};
    wxColour col_brush{0, 0, 0, 0};

    pen_.SetColour(col_pen);
    brush_.SetColour(col_brush);

    Hide();
}

void EditingSilhouette::setPosAndSize(const wxPoint& pos, const wxSize& size)
{
    pos_ = pos;
    size_ = size;

    this->SetSize(size_);
    this->SetPosition(pos_);

    Refresh();
}

void EditingSilhouette::OnPaint(wxPaintEvent& WXUNUSED(event))
{
    wxPaintDC dc(this);

    const wxRect rect{wxRect(0, 0, size_.GetWidth(), size_.GetHeight())};

    dc.SetPen(pen_);
    dc.SetBrush(brush_);

    dc.DrawRectangle(rect);
}
