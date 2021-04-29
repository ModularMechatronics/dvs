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

void PrototypeView::mouseMoved(wxMouseEvent& event)
{
    const float mouse_x = event.GetX();
    const float mouse_y = size_.GetHeight() - event.GetY();

    bool is_inside = false;

    for(size_t k = 0; k < squares_.size(); k++)
    {
        if(squares_[k].mouseState(mouse_x, mouse_y) != CursorSquareState::OUTSIDE)
        {
            is_inside = true;
            break;
        }
    }

    std::cout << "Mouse: " << mouse_x << ", " << mouse_y << std::endl;

    if(is_inside)
    {
        wxSetCursor(wxCursor(wxCURSOR_HAND));
    }
    else
    {
        wxSetCursor(wxCursor(wxCURSOR_ARROW));
    }
}

PrototypeView::PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size)
     : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

    squares_.push_back(Square(20, 13, 15, 15));
    squares_.push_back(Square(2, 2, 5, 5));
    squares_.push_back(Square(10, 2, 7, 7));

    size_ = size;

    grid_settings_.num_cells_x = 100;
    grid_settings_.num_cells_y = 100;
    grid_settings_.margin_x = 30;
    grid_settings_.margin_y = 30;

    updateGridStates();

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    Bind(wxEVT_MOTION, &PrototypeView::mouseMoved, this);

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
    updateGridStates();
}

void PrototypeView::changeNumCellsX(const float change)
{
    grid_settings_.num_cells_x = std::min(200.0f, std::max(1.0f, grid_settings_.num_cells_x + change));
    updateGridStates();
    Refresh();
}

void PrototypeView::changeNumCellsY(const float change)
{
    grid_settings_.num_cells_y = std::min(200.0f, std::max(1.0f, grid_settings_.num_cells_y + change));
    updateGridStates();
    Refresh();
}

void PrototypeView::changeMarginX(const float change)
{
    grid_settings_.margin_x = std::min(200.0f, std::max(0.0f, grid_settings_.margin_x + change));
    updateGridStates();
    Refresh();
}

void PrototypeView::changeMarginY(const float change)
{
    grid_settings_.margin_y = std::min(200.0f, std::max(0.0f, grid_settings_.margin_y + change));
    updateGridStates();
    Refresh();
}

void PrototypeView::updateGridStates()
{
    // Screen
    screen_grid_state_.offset_x = grid_settings_.margin_x;
    screen_grid_state_.offset_y = grid_settings_.margin_y;

    screen_grid_state_.grid_size_x = static_cast<float>(size_.GetWidth()) - 2.0f * grid_settings_.margin_x;
    screen_grid_state_.grid_size_y = static_cast<float>(size_.GetHeight()) - 2.0f * grid_settings_.margin_y;

    screen_grid_state_.cell_size_x = screen_grid_state_.grid_size_x / static_cast<float>(grid_settings_.num_cells_x);
    screen_grid_state_.cell_size_y = screen_grid_state_.grid_size_y / static_cast<float>(grid_settings_.num_cells_y);

    // GL
    gl_grid_state_.offset_x = 2.0f * static_cast<float>(grid_settings_.margin_x) / static_cast<float>(size_.GetWidth());
    gl_grid_state_.offset_y = 2.0f * static_cast<float>(grid_settings_.margin_y) / static_cast<float>(size_.GetHeight());

    gl_grid_state_.grid_size_x = 2.0f - 2.0f * gl_grid_state_.offset_x;
    gl_grid_state_.grid_size_y = 2.0f - 2.0f * gl_grid_state_.offset_y;

    gl_grid_state_.cell_size_x = gl_grid_state_.grid_size_x / static_cast<float>(grid_settings_.num_cells_x);
    gl_grid_state_.cell_size_y = gl_grid_state_.grid_size_y / static_cast<float>(grid_settings_.num_cells_y);

    gl_grid_state_.x_min = -1.0f + gl_grid_state_.offset_x;
    gl_grid_state_.x_max = 1.0f - gl_grid_state_.offset_x;
    gl_grid_state_.y_min = -1.0f + gl_grid_state_.offset_y;
    gl_grid_state_.y_max = 1.0f - gl_grid_state_.offset_y;

    for(size_t k = 0; k < squares_.size(); k++)
    {
        squares_[k].updateInternals(grid_settings_, screen_grid_state_, gl_grid_state_);
    }
}

void PrototypeView::render(wxPaintEvent& evt)
{
    (void)evt;
    if (!IsShown())
        return;

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);

    glEnable(GL_MULTISAMPLE);

    const float bg_color = 245.0f;

    glClearColor(bg_color / 255.0f, bg_color / 255.0f, bg_color / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLineWidth(0.1f);
    glColor3f(0.0f, 0.0f, 0.0f);

    // Drawing vertical lines
    for(int idx_x = 0; idx_x < (grid_settings_.num_cells_x + 1); idx_x++)
    {
        const float x_val = gl_grid_state_.x_min + static_cast<float>(idx_x) * gl_grid_state_.cell_size_x;
        glBegin(GL_LINES);
        glVertex2f(x_val, gl_grid_state_.y_min);
        glVertex2f(x_val, gl_grid_state_.y_max);
        glEnd();
    }

    // Drawing horizontal lines
    for(int idx_y = 0; idx_y < (grid_settings_.num_cells_y + 1); idx_y++)
    {
        const float y_val = gl_grid_state_.y_max - static_cast<float>(idx_y) * gl_grid_state_.cell_size_y;
        glBegin(GL_LINES);
        glVertex2f(gl_grid_state_.x_min, y_val);
        glVertex2f(gl_grid_state_.x_max, y_val);
        glEnd();
    }

    for(size_t k = 0; k < squares_.size(); k++)
    {
        squares_[k].render();
    }

    SwapBuffers();
}

void PrototypeView::mouseLeftPressed(wxMouseEvent& event)
{
    Refresh();
}
