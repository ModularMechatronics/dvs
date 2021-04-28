#include <wx/glcanvas.h>
#include <wx/wx.h>

#include "prototype_view.h"

#include <cmath>

BEGIN_EVENT_TABLE(PrototypeView, wxGLCanvas)
EVT_LEFT_DOWN(PrototypeView::mouseLeftPressed)
EVT_PAINT(PrototypeView::render)
END_EVENT_TABLE()

int* PrototypeView::getArgsPtr()
{
    args[0] = WX_GL_RGBA;
    args[1] = WX_GL_DOUBLEBUFFER;
    args[2] = WX_GL_DEPTH_SIZE;
    args[3] = 16;
    args[4] = WX_GL_SAMPLES;
    args[5] = 4;
    args[6] = WX_GL_SAMPLE_BUFFERS;
    args[7] = 1;
    args[8] = 0;

    return args;
}

PrototypeView::PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size)
     : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

    size_ = size;

    grid_settings_.num_cells_x = 10;
    grid_settings_.num_cells_y = 10;
    grid_settings_.margin_x = 20;
    grid_settings_.margin_y = 20;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    glEnable(GL_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}

PrototypeView::~PrototypeView()
{
    delete m_context;
}

void PrototypeView::setPosAndSize(const wxPoint pos, const wxSize size)
{
    this->SetSize(size);
    size_ = size;
}

void PrototypeView::setParameters(const GridSettings& grid_settings)
{
    grid_settings_ = grid_settings;
}

void PrototypeView::changeNumGridCellsX(const int change)
{
    grid_settings_.num_cells_x = std::min(200, std::max(2, grid_settings_.num_cells_x + change));
    Refresh();
}

void PrototypeView::changeNumGridCellsY(const int change)
{
    grid_settings_.num_cells_y = std::min(200, std::max(2, grid_settings_.num_cells_y + change));
    Refresh();
}

void PrototypeView::changeMarginX(const int change)
{
    grid_settings_.margin_x = std::min(200, std::max(0, grid_settings_.margin_x + change));
    Refresh();
}

void PrototypeView::changeMarginY(const int change)
{
    grid_settings_.margin_y = std::min(200, std::max(0, grid_settings_.margin_y + change));
    Refresh();
}

void PrototypeView::render(wxPaintEvent& evt)
{
    (void)evt;
    if (!IsShown())
        return;

    grid_settings_.num_cells_x = std::min(200, std::max(grid_settings_.num_cells_x, 0));
    grid_settings_.num_cells_y = std::min(200, std::max(grid_settings_.num_cells_y, 0));

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);

    glEnable(GL_MULTISAMPLE);

    const float bg_color = 245.0f;

    glClearColor(bg_color / 255.0f, bg_color / 255.0f, bg_color / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(0.1f);
    glColor3f(0.0f, 0.0f, 0.0f);

    const float normalized_offset_x = grid_settings_.margin_x / static_cast<float>(size_.GetWidth());
    const float normalized_offset_y = grid_settings_.margin_y / static_cast<float>(size_.GetHeight());

    const float window_x = 2.0f - 2.0f * normalized_offset_x;
    const float window_y = 2.0f - 2.0f * normalized_offset_y;

    const float dx = window_x / static_cast<float>(grid_settings_.num_cells_x);
    const float dy = window_y / static_cast<float>(grid_settings_.num_cells_y);

    for(int idx_x = 0; idx_x < (grid_settings_.num_cells_x + 1); idx_x++)
    {
        const float x0 = static_cast<float>(idx_x) * dx - 1.0f + normalized_offset_x;
        const float x1 = x0;
        const float y0 = -1.0f + normalized_offset_y;
        const float y1 = 1.0f - normalized_offset_y;
        glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glEnd();
    }

    for(int idx_y = 0; idx_y < (grid_settings_.num_cells_y + 1); idx_y++)
    {
        const float x0 = -1.0f + normalized_offset_x;
        const float x1 = 1.0f - normalized_offset_x;
        const float y0 = static_cast<float>(idx_y) * dy - 1.0f + normalized_offset_y;
        const float y1 = y0;
        glBegin(GL_LINES);
        glVertex2f(x0, y0);
        glVertex2f(x1, y1);
        glEnd();
    }

    SwapBuffers();
}

void PrototypeView::mouseLeftPressed(wxMouseEvent& event)
{
    Refresh();
}
