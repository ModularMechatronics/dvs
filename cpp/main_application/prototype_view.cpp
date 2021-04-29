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
    mouse_x = event.GetX();
    mouse_y = size_.GetHeight() - event.GetY();

    left_button_pressed_ = true;

    if(is_inside_square_)
    {
        pos_of_pressed_x_ = squares_[idx_of_selected_square_].getX();
        pos_of_pressed_y_ = squares_[idx_of_selected_square_].getY();

        size_of_pressed_width_ = squares_[idx_of_selected_square_].getWidth();
        size_of_pressed_height_ = squares_[idx_of_selected_square_].getHeight();

        grid_pos_pressed_x_ = std::floor(mouse_x / screen_grid_state_.cell_size_x);
        grid_pos_pressed_y_ = std::floor(mouse_y / screen_grid_state_.cell_size_y);

        is_editing_ = true;
    }
    
    

    std::cout << "Left pushed!" << std::endl;
}

void PrototypeView::mouseLeftReleased(wxMouseEvent& event)
{
    left_button_pressed_ = false;
    is_editing_ = false;
    idx_of_selected_square_ = -1;
    std::cout << "Left released!" << std::endl;
    // (void)event;
    // left_mouse_button_.setIsReleased();
    // Refresh();
}

void PrototypeView::mouseMoved(wxMouseEvent& event)
{
    mouse_x = event.GetX();
    mouse_y = size_.GetHeight() - event.GetY();

    if(is_editing_)
    {
        if(idx_of_selected_square_ != -1)
        {
            const float trunc_x = std::floor(mouse_x / screen_grid_state_.cell_size_x);
            const float trunc_y = std::floor(mouse_y / screen_grid_state_.cell_size_y);
            const float diff_x = grid_pos_pressed_x_ - trunc_x;
            const float diff_y = grid_pos_pressed_y_ - trunc_y;

            const float new_pos_x = pos_of_pressed_x_ - diff_x;
            const float new_pos_y = pos_of_pressed_y_ - diff_y;

            const float new_width = std::max(1.0f, size_of_pressed_width_ - diff_x);
            const float new_height = std::max(1.0f, size_of_pressed_height_ - diff_y);

            const float new_width_positive = std::max(1.0f, size_of_pressed_width_ + diff_x);
            const float new_height_positive = std::max(1.0f, size_of_pressed_height_ + diff_y);

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
            cursor_square_state = squares_[k].mouseState(mouse_x, mouse_y);
            if(cursor_square_state != CursorSquareState::OUTSIDE)
            {
                is_inside_square_ = true;
                idx_of_selected_square_ = k;
                break;
            }
        }

        // std::cout << "Mouse: " << mouse_x << ", " << mouse_y << std::endl;

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

    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_x;
}

PrototypeView::PrototypeView(wxPanel* parent, const wxPoint& position, const wxSize& size)
     : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

    squares_.push_back(Square(20, 13, 15, 15));
    squares_.push_back(Square(2, 2, 5, 5));
    squares_.push_back(Square(10, 2, 7, 7));

    grid_pos_pressed_x_ = 0.0f;
    grid_pos_pressed_y_ = 0.0f;

    size_ = size;
    left_button_pressed_ = false;
    is_editing_ = false;
    mouse_x = 0.0f;
    mouse_y = 0.0f;
    prev_mouse_x = 0.0f;
    prev_mouse_y = 0.0f;
    idx_of_selected_square_ = -1;

    grid_settings_.num_cells_x = 100;
    grid_settings_.num_cells_y = 100;

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

void PrototypeView::updateGridStates()
{
    // Screen
    screen_grid_state_.grid_size_x = static_cast<float>(size_.GetWidth());
    screen_grid_state_.grid_size_y = static_cast<float>(size_.GetHeight());

    screen_grid_state_.cell_size_x = screen_grid_state_.grid_size_x / static_cast<float>(grid_settings_.num_cells_x);
    screen_grid_state_.cell_size_y = screen_grid_state_.grid_size_y / static_cast<float>(grid_settings_.num_cells_y);

    // GL
    gl_grid_state_.grid_size_x = 2.0f;
    gl_grid_state_.grid_size_y = 2.0f;

    gl_grid_state_.cell_size_x = gl_grid_state_.grid_size_x / static_cast<float>(grid_settings_.num_cells_x);
    gl_grid_state_.cell_size_y = gl_grid_state_.grid_size_y / static_cast<float>(grid_settings_.num_cells_y);

    gl_grid_state_.x_min = -1.0f;
    gl_grid_state_.x_max = 1.0f;
    gl_grid_state_.y_min = -1.0f;
    gl_grid_state_.y_max = 1.0f;

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
