#include "graphic_window.h"

wxBEGIN_EVENT_TABLE(ShapedFrame, wxFrame)
    EVT_LEFT_DCLICK(ShapedFrame::OnDoubleClick)
    EVT_LEFT_DOWN(ShapedFrame::OnLeftDown)
    EVT_LEFT_UP(ShapedFrame::OnLeftUp)
    EVT_MOTION(ShapedFrame::OnMouseMove)
    EVT_RIGHT_UP(ShapedFrame::OnExit)
    EVT_PAINT(ShapedFrame::OnPaint)
wxEND_EVENT_TABLE()


// frame constructor
ShapedFrame::ShapedFrame(wxFrame *parent)
       : wxFrame(parent, wxID_ANY, wxEmptyString,
                  wxDefaultPosition, wxSize(50, 50),
                  0
                  | wxFRAME_SHAPED
                  | wxSIMPLE_BORDER
                  | wxFRAME_NO_TASKBAR
                  | wxSTAY_ON_TOP
            )
{
    m_shapeKind = Shape_Star;
    m_bmp = wxBitmap("/Users/danielpi/work/dvs/src/main_application/star.png", wxBITMAP_TYPE_PNG);
    SetSize(wxSize(m_bmp.GetWidth(), m_bmp.GetHeight()));
    SetWindowShape();
}

void ShapedFrame::SetWindowShape()
{
    switch ( m_shapeKind )
    {
        case Shape_None:
            SetShape(wxRegion());
            break;

        case Shape_Star:
            SetShape(wxRegion(m_bmp, *wxWHITE));
            break;

#if wxUSE_GRAPHICS_CONTEXT
        case Shape_Circle:
            {
                wxGraphicsPath
                    path = wxGraphicsRenderer::GetDefaultRenderer()->CreatePath();
                // path.AddCircle(m_bmp.GetWidth()/2, m_bmp.GetHeight()/2, 30);
                path.AddRoundedRectangle(0, 0, m_bmp.GetWidth(), m_bmp.GetHeight(), 10);
                SetShape(path);
            }
            break;
#endif // wxUSE_GRAPHICS_CONTEXT

        case Shape_Max:
            wxFAIL_MSG( "invalid shape kind" );
            break;
    }
}

void ShapedFrame::OnDoubleClick(wxMouseEvent& WXUNUSED(evt))
{
    m_shapeKind = static_cast<ShapeKind>((m_shapeKind + 1) % Shape_Max);
    SetWindowShape();
}

void ShapedFrame::OnLeftDown(wxMouseEvent& evt)
{
    CaptureMouse();
    wxPoint pos = ClientToScreen(evt.GetPosition());
    wxPoint origin = GetPosition();
    int dx =  pos.x - origin.x;
    int dy = pos.y - origin.y;
    m_delta = wxPoint(dx, dy);
}

void ShapedFrame::OnLeftUp(wxMouseEvent& WXUNUSED(evt))
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void ShapedFrame::OnMouseMove(wxMouseEvent& evt)
{
    wxPoint pt = evt.GetPosition();
    if (evt.Dragging() && evt.LeftIsDown())
    {
        wxPoint pos = ClientToScreen(pt);
        Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
    }
}

void ShapedFrame::OnExit(wxMouseEvent& WXUNUSED(evt))
{
    Close();
}

void ShapedFrame::OnPaint(wxPaintEvent& WXUNUSED(evt))
{
    wxPaintDC dc(this);
    dc.DrawBitmap(m_bmp, 0, 0, true);
}
