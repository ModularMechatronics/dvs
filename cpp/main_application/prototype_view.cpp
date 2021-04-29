#include <wx/glcanvas.h>
#include <wx/wx.h>

#include "prototype_view.h"

#include <cmath>

BEGIN_EVENT_TABLE(PrototypeView, wxGLCanvas)
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

void PrototypeView::mouseLeftPressed(wxMouseEvent& event)
{
    const Vec2Df mouse_pos(event.GetX(), panel_size_.GetHeight() - event.GetY());

    left_button_pressed_ = true;

    if(is_inside_square_)
    {
        pos_of_pressed_sq_ = squares_[idx_of_selected_square_].getPos();

        size_of_pressed_ = squares_[idx_of_selected_square_].getSize();

        grid_pos_pressed_.x = std::floor(mouse_pos.x / screen_grid_state_.cell_size.x);
        grid_pos_pressed_.y = std::floor(mouse_pos.y / screen_grid_state_.cell_size.y);

        is_editing_ = true;
    }
}

void PrototypeView::mouseLeftReleased(wxMouseEvent& event)
{
    left_button_pressed_ = false;
    is_editing_ = false;
    idx_of_selected_square_ = -1;
}

void PrototypeView::mouseMoved(wxMouseEvent& event)
{
    const Vec2Df mouse_pos(event.GetX(), panel_size_.GetHeight() - event.GetY());

    if(is_editing_)
    {
        if(idx_of_selected_square_ != -1)
        {
            const float trunc_x = std::floor(mouse_pos.x / screen_grid_state_.cell_size.x);
            const float trunc_y = std::floor(mouse_pos.y / screen_grid_state_.cell_size.y);
            const float diff_x = grid_pos_pressed_.x - trunc_x;
            const float diff_y = grid_pos_pressed_.y - trunc_y;

            const float new_pos_x = pos_of_pressed_sq_.x - diff_x;
            const float new_pos_y = pos_of_pressed_sq_.y - diff_y;

            const float new_width = std::max(1.0f, size_of_pressed_.width - diff_x);
            const float new_height = std::max(1.0f, size_of_pressed_.height - diff_y);

            const float new_width_positive = std::max(1.0f, size_of_pressed_.width + diff_x);
            const float new_height_positive = std::max(1.0f, size_of_pressed_.height + diff_y);

            switch(cursor_square_state)
            {
                case CursorSquareState::LEFT:
                    squares_[idx_of_selected_square_].setWidthAndX(new_width_positive, new_pos_x);
                    break;
                case CursorSquareState::RIGHT:
                    squares_[idx_of_selected_square_].setWidth(new_width);
                    break;
                case CursorSquareState::TOP:
                    squares_[idx_of_selected_square_].setHeight(new_height);
                    break;
                case CursorSquareState::BOTTOM:
                    squares_[idx_of_selected_square_].setHeightAndY(new_height_positive, new_pos_y);
                    break;
                case CursorSquareState::BOTTOM_RIGHT:
                    squares_[idx_of_selected_square_].setHeightAndY(new_height_positive, new_pos_y);
                    squares_[idx_of_selected_square_].setWidth(new_width);
                    break;
                case CursorSquareState::BOTTOM_LEFT:
                    squares_[idx_of_selected_square_].setHeightAndY(new_height_positive, new_pos_y);
                    squares_[idx_of_selected_square_].setWidthAndX(new_width_positive, new_pos_x);
                    break;
                case CursorSquareState::TOP_RIGHT:
                    squares_[idx_of_selected_square_].setWidth(new_width);
                    squares_[idx_of_selected_square_].setHeight(new_height);
                    break;
                case CursorSquareState::TOP_LEFT:
                    squares_[idx_of_selected_square_].setHeight(new_height);
                    squares_[idx_of_selected_square_].setWidthAndX(new_width_positive, new_pos_x);
                    break;
                case CursorSquareState::INSIDE:
                    squares_[idx_of_selected_square_].setPosition(new_pos_x, new_pos_y);
                    break;
                default:
                    std::cout << "Do nothing..." << std::endl;
            }

            updateGridStates();

            Refresh();
        }
        else
        {
            std::cout << "Got editing with negative square index!" << std::endl;
        }
    }
    else
    {
        is_inside_square_ = false;
        cursor_square_state = CursorSquareState::OUTSIDE;

        for(size_t k = 0; k < squares_.size(); k++)
        {
            cursor_square_state = squares_[k].mouseState(mouse_pos);
            if(cursor_square_state != CursorSquareState::OUTSIDE)
            {
                is_inside_square_ = true;
                idx_of_selected_square_ = k;
                break;
            }
        }

        if(is_inside_square_)
        {
            switch(cursor_square_state)
            {
                case CursorSquareState::LEFT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
                    break;
                case CursorSquareState::RIGHT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZEWE));
                    break;
                case CursorSquareState::TOP:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENS));
                    break;
                case CursorSquareState::BOTTOM:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENS));
                    break;
                case CursorSquareState::BOTTOM_RIGHT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
                    break;
                case CursorSquareState::BOTTOM_LEFT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
                    break;
                case CursorSquareState::TOP_RIGHT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENESW));
                    break;
                case CursorSquareState::TOP_LEFT:
                    wxSetCursor(wxCursor(wxCURSOR_SIZENWSE));
                    break;
                case CursorSquareState::INSIDE:
                    wxSetCursor(wxCursor(wxCURSOR_HAND));
                    break;
                default:
                    wxSetCursor(wxCursor(wxCURSOR_HAND));
            }
        }
        else
        {
            wxSetCursor(wxCursor(wxCURSOR_ARROW));
        }
    }
}

PrototypeView::PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size)
     : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

    squares_.push_back(Square(20, 13, 15, 15));
    squares_.push_back(Square(2, 2, 5, 5));
    squares_.push_back(Square(10, 2, 7, 7));

    grid_pos_pressed_.x = 0.0f;
    grid_pos_pressed_.y = 0.0f;

    panel_size_ = size;
    left_button_pressed_ = false;
    is_editing_ = false;

    idx_of_selected_square_ = -1;

    gl_bounds_.x_min = -1.0f;
    gl_bounds_.x_max = 1.0f;
    gl_bounds_.y_min = -1.0f;
    gl_bounds_.y_max = 1.0f;

    grid_settings_.num_cells.x = 100;
    grid_settings_.num_cells.y = 100;

    updateGridStates();

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    Bind(wxEVT_MOTION, &PrototypeView::mouseMoved, this);
    Bind(wxEVT_LEFT_DOWN, &PrototypeView::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &PrototypeView::mouseLeftReleased, this);

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
    panel_size_ = size;
    updateGridStates();
}

void PrototypeView::changeNumCellsX(const float change)
{
    grid_settings_.num_cells.x = std::min(200.0f, std::max(1.0f, grid_settings_.num_cells.x + change));
    updateGridStates();
    Refresh();
}

void PrototypeView::changeNumCellsY(const float change)
{
    grid_settings_.num_cells.y = std::min(200.0f, std::max(1.0f, grid_settings_.num_cells.y + change));
    updateGridStates();
    Refresh();
}

void PrototypeView::updateGridStates()
{
    // Screen
    screen_grid_state_.grid_size = Vec2Df(panel_size_.GetWidth(), panel_size_.GetHeight());

    screen_grid_state_.cell_size = Vec2Df(screen_grid_state_.grid_size.x / grid_settings_.num_cells.x,
                                          screen_grid_state_.grid_size.y / grid_settings_.num_cells.y);

    // GL
    gl_grid_state_.grid_size = Vec2Df(2.0f, 2.0f);

    gl_grid_state_.cell_size = Vec2Df(gl_grid_state_.grid_size.x / grid_settings_.num_cells.x,
                                      gl_grid_state_.grid_size.y / grid_settings_.num_cells.y);

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
    for(int idx_x = 0; idx_x < (static_cast<int>(grid_settings_.num_cells.x) + 1); idx_x++)
    {
        const float x_val = gl_bounds_.x_min + static_cast<float>(idx_x) * gl_grid_state_.cell_size.x;
        glBegin(GL_LINES);
        glVertex2f(x_val, gl_bounds_.y_min);
        glVertex2f(x_val, gl_bounds_.y_max);
        glEnd();
    }

    // Drawing horizontal lines
    for(int idx_y = 0; idx_y < (static_cast<int>(grid_settings_.num_cells.y) + 1); idx_y++)
    {
        const float y_val = gl_bounds_.y_max - static_cast<float>(idx_y) * gl_grid_state_.cell_size.y;
        glBegin(GL_LINES);
        glVertex2f(gl_bounds_.x_min, y_val);
        glVertex2f(gl_bounds_.x_max, y_val);
        glEnd();
    }

    for(size_t k = 0; k < squares_.size(); k++)
    {
        squares_[k].render();
    }

    SwapBuffers();
}
