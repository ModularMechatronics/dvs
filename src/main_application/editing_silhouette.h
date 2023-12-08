#ifndef EDITING_SILHOUETTE_H
#define EDITING_SILHOUETTE_H

#include <wx/button.h>
#include <wx/frame.h>
#include <wx/wx.h>

#include <functional>

class EditingSilhouette : public wxPanel
{
public:
    EditingSilhouette() = delete;
    EditingSilhouette(wxFrame* parent, const wxPoint& pos, const wxSize& size);

    void setPosAndSize(const wxPoint& pos, const wxSize& size);

private:
    void OnPaint(wxPaintEvent& event);

    wxSize size_;
    wxPoint pos_;

    wxPen pen_;
    wxBrush brush_;
};

#endif  // EDITING_SILHOUETTE_H
