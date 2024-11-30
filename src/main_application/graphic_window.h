#ifndef MAIN_APPLICATION_GRAPHIC_WINDOW_H
#define MAIN_APPLICATION_GRAPHIC_WINDOW_H

#include <wx/wx.h>

#ifndef WX_PRECOMP
    #include <wx/app.h>
    #include <wx/log.h>
    #include <wx/frame.h>
    #include <wx/panel.h>
    #include <wx/stattext.h>
    #include <wx/menu.h>
    #include <wx/layout.h>
    #include <wx/msgdlg.h>
    #include <wx/image.h>
#endif

#include <wx/dcclient.h>
#include <wx/graphics.h>
#include <wx/image.h>


// Define a new frame type: this is going to the frame showing the
// effect of wxFRAME_SHAPED
class ShapedFrame : public wxFrame
{
public:
    // ctor(s)
    ShapedFrame(wxFrame *parent);
    void SetWindowShape();

    // event handlers (these functions should _not_ be virtual)
    void OnDoubleClick(wxMouseEvent& evt);
    void OnLeftDown(wxMouseEvent& evt);
    void OnLeftUp(wxMouseEvent& evt);
    void OnMouseMove(wxMouseEvent& evt);
    void OnExit(wxMouseEvent& evt);
    void OnPaint(wxPaintEvent& evt);

private:
    enum ShapeKind
    {
        Shape_None,
        Shape_Star,
#if wxUSE_GRAPHICS_CONTEXT
        Shape_Circle,
#endif // wxUSE_GRAPHICS_CONTEXT
        Shape_Max
    } m_shapeKind;

    wxBitmap m_bmp;
    wxPoint  m_delta;

    // any class wishing to process wxWidgets events must use this macro
    wxDECLARE_EVENT_TABLE();
};


#endif
