#include "plot_pane.h"

#include <wx/event.h>
#include <wx/glcanvas.h>

#include "axes/axes.h"
#include "dvs/enumerations.h"
#include "dvs/math/math.h"
#include "events.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_low_level.h"

using namespace dvs::internal;

CursorSquareState mouseState(const Bound2D bound, const Bound2D bound_margin, const Vec2f mouse_pos)
{
    if ((bound.x_min <= mouse_pos.x) && (mouse_pos.x <= bound.x_max) && (bound.y_min <= mouse_pos.y) &&
        (mouse_pos.y <= bound.y_max))
    {
        if (mouse_pos.x <= bound_margin.x_min)
        {
            if (mouse_pos.y <= bound_margin.y_min)
            {
                return CursorSquareState::TOP_LEFT;
            }
            else if (bound_margin.y_max <= mouse_pos.y)
            {
                return CursorSquareState::BOTTOM_LEFT;
            }
            else
            {
                return CursorSquareState::LEFT;
            }
        }
        else if (bound_margin.x_max <= mouse_pos.x)
        {
            if (mouse_pos.y <= bound_margin.y_min)
            {
                return CursorSquareState::TOP_RIGHT;
            }
            else if (bound_margin.y_max <= mouse_pos.y)
            {
                return CursorSquareState::BOTTOM_RIGHT;
            }
            else
            {
                return CursorSquareState::RIGHT;
            }
        }
        else if (mouse_pos.y <= bound_margin.y_min)
        {
            return CursorSquareState::TOP;
        }
        else if (bound_margin.y_max <= mouse_pos.y)
        {
            return CursorSquareState::BOTTOM;
        }
        else
        {
            return CursorSquareState::INSIDE;
        }
    }
    else
    {
        return CursorSquareState::OUTSIDE;
    }
}

wxGLContext* PlotPane::getContext()
{
#ifdef PLATFORM_APPLE_M
    wxGLContextAttrs cxtAttrs;
    cxtAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 2).EndList();
    // https://stackoverflow.com/questions/41145024/wxwidgets-and-modern-opengl-3-3
    return new wxGLContext(this, NULL, &cxtAttrs);
#endif

#ifdef PLATFORM_LINUX_M
    return new wxGLContext(this);
#endif
}

void PlotPane::initShaders()
{
    const std::string v_path = "../main_application/axes/shaders/basic.vs";
    const std::string f_path = "../main_application/axes/shaders/basic.fs";
    shader_collection_.plot_box_shader = Shader::createFromFiles(v_path, f_path);

    const std::string v_path_text = "../main_application/axes/shaders/text.vs";
    const std::string f_path_text = "../main_application/axes/shaders/text.fs";
    shader_collection_.text_shader = Shader::createFromFiles(v_path_text, f_path_text);

    const std::string v_path_basic_plot_shader = "../main_application/axes/shaders/basic_plot_shader.vs";
    const std::string f_path_basic_plot_shader = "../main_application/axes/shaders/basic_plot_shader.fs";
    shader_collection_.basic_plot_shader = Shader::createFromFiles(v_path_basic_plot_shader, f_path_basic_plot_shader);

    const std::string v_path_plot_2d_shader = "../main_application/axes/shaders/plot_2d_shader.vs";
    const std::string f_path_plot_2d_shader = "../main_application/axes/shaders/plot_2d_shader.fs";
    shader_collection_.plot_2d_shader = Shader::createFromFiles(v_path_plot_2d_shader, f_path_plot_2d_shader);

    const std::string v_path_plot_3d_shader = "../main_application/axes/shaders/plot_3d_shader.vs";
    const std::string f_path_plot_3d_shader = "../main_application/axes/shaders/plot_3d_shader.fs";
    shader_collection_.plot_3d_shader = Shader::createFromFiles(v_path_plot_3d_shader, f_path_plot_3d_shader);

    const std::string v_path_img_plot_shader = "../main_application/axes/shaders/img.vs";
    const std::string f_path_img_plot_shader = "../main_application/axes/shaders/img.fs";
    shader_collection_.img_plot_shader = Shader::createFromFiles(v_path_img_plot_shader, f_path_img_plot_shader);

    const std::string v_path_surf_shader = "../main_application/axes/shaders/surf.vs";
    const std::string f_path_surf_shader = "../main_application/axes/shaders/surf.fs";
    shader_collection_.surf_shader = Shader::createFromFiles(v_path_surf_shader, f_path_surf_shader);

    const std::string v_path_scatter_shader = "../main_application/axes/shaders/scatter_shader.vs";
    const std::string f_path_scatter_shader = "../main_application/axes/shaders/scatter_shader.fs";
    shader_collection_.scatter_shader = Shader::createFromFiles(v_path_scatter_shader, f_path_scatter_shader);
}

PlotPane::PlotPane(wxWindow* parent,
    const ElementSettings& element_settings,
    const float grid_size,
    const std::function<void(const char key)>& notify_main_window_key_pressed,
    const std::function<void(const char key)>& notify_main_window_key_released)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
      GuiElement(element_settings, notify_main_window_key_pressed,
        notify_main_window_key_released), m_context(getContext()), axes_interactor_(axes_settings_, getWidth(), getHeight())
{
    is_editing_ = false;
    parent_size_ = parent->GetSize();
    grid_size_ = grid_size;
    edit_size_margin_ = 20.0f;
    perspective_projection_ = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    view_parent_ = dynamic_cast<SuperBase*>(parent);

    is_selected_ = false;

    bindCallbacks();

    wxGLCanvas::SetCurrent(*m_context);
    initShaders();

    axes_renderer_ = new AxesRenderer(shader_collection_);
    plot_data_handler_ = new PlotDataHandler(shader_collection_);

    hold_on_ = true;
    axes_set_ = false;
    view_set_ = false;

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}

void PlotPane::updateSizeFromParent(const wxSize& parent_size)
{
    parent_size_ = parent_size;
    const float px = parent_size_.GetWidth();
    const float py = parent_size_.GetHeight();

    const float xpos = px * element_settings_.x;
    const float ypos = py * element_settings_.y;

    const float width = px * element_settings_.width;
    const float height = py * element_settings_.height;

    this->setPosition(wxPoint(xpos, ypos));
    this->setSize(wxSize(width, height));
}

void PlotPane::setPosition(const wxPoint& new_pos)
{
    this->SetPosition(new_pos);
}

void PlotPane::setSize(const wxSize& new_size)
{
    // axes_renderer_->setWindowSize(new_size.GetWidth(), new_size.GetHeight());
#ifdef PLATFORM_LINUX_M
    // This seems to be needed on linux platforms
    glViewport(0, 0, new_size.GetWidth(), new_size.GetHeight());
#endif
    this->SetSize(new_size);
}

void PlotPane::show()
{
    this->Show();
}

void PlotPane::hide()
{
    this->Hide();
}

void PlotPane::bindCallbacks()
{
    Bind(wxEVT_MOTION, &PlotPane::mouseMoved, this);
    Bind(wxEVT_LEFT_DOWN, &PlotPane::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &PlotPane::mouseLeftReleased, this);
    Bind(wxEVT_KEY_DOWN, &PlotPane::keyPressedCallback, this);
    Bind(wxEVT_KEY_UP, &PlotPane::keyReleasedCallback, this);
    Bind(wxEVT_PAINT, &PlotPane::render, this);
    Bind(wxEVT_LEAVE_WINDOW, &PlotPane::mouseLeftWindow, this);
}

int* PlotPane::getArgsPtr()
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

PlotPane::~PlotPane()
{
    delete m_context;
}

void PlotPane::addData(std::unique_ptr<const ReceivedData> received_data,
                               const dvs::internal::TransmissionHeader& hdr)
{
    const internal::Function fcn = hdr.getObjectAtIdx(0).as<internal::Function>();

    wxGLCanvas::SetCurrent(*m_context);

    if (fcn == Function::HOLD_ON)
    {
        hold_on_ = true;
    }
    else if (fcn == Function::HOLD_OFF)
    {
        hold_on_ = false;
    }
    else if (fcn == Function::AXES_2D)
    {
        axes_set_ = true;

        const std::pair<Vec3d, Vec3d> axes_bnd =
            hdr.get(TransmissionHeaderObjectType::AXIS_MIN_MAX_VEC).as<std::pair<Vec3d, Vec3d>>();
        axes_interactor_.setAxesLimits(Vec2d(axes_bnd.first.x, axes_bnd.first.y),
                                        Vec2d(axes_bnd.second.x, axes_bnd.second.y));
    }
    else if (fcn == Function::AXES_3D)
    {
        axes_set_ = true;

        const std::pair<Vec3d, Vec3d> axes_bnd =
            hdr.get(TransmissionHeaderObjectType::AXIS_MIN_MAX_VEC).as<std::pair<Vec3d, Vec3d>>();
        axes_interactor_.setAxesLimits(Vec3d(axes_bnd.first.x, axes_bnd.first.y, axes_bnd.first.z),
                                        Vec3d(axes_bnd.second.x, axes_bnd.second.y, axes_bnd.second.z));
    }
    else if (fcn == Function::VIEW)
    {
        view_set_ = true;
        const float azimuth = hdr.get(internal::TransmissionHeaderObjectType::AZIMUTH).as<float>();
        const float elevation = hdr.get(internal::TransmissionHeaderObjectType::ELEVATION).as<float>();

        const float azimuth_rad = azimuth * M_PI / 180.0f;
        const float elevation_rad = elevation * M_PI / 180.0f;

        axes_interactor_.setViewAngles(azimuth_rad, elevation_rad);
    }
    else if (fcn == Function::CLEAR)
    {
        axes_set_ = false;
        hold_on_ = true;
        view_set_ = false;

        plot_data_handler_->clear();
        axes_interactor_.setViewAngles(0, M_PI);
        axes_interactor_.setAxesLimits(Vec3d(-1.0, -1.0, -1.0), Vec3d(1.0, 1.0, 1.0));
    }
    else if (fcn == Function::SOFT_CLEAR)
    {
        plot_data_handler_->softClear();
    }
    else
    {
        if (!hold_on_)
        {
            plot_data_handler_->clear();
        }
        plot_data_handler_->addData(std::move(received_data), hdr);

        if (!axes_set_)
        {
            const std::pair<Vec3d, Vec3d> min_max = plot_data_handler_->getMinMaxVectors();
            const Vec3d mean_vec = (min_max.second + min_max.first) / 2.0;

            const Vec3d min_vec = (min_max.first - mean_vec) * 1.001 + mean_vec;
            const Vec3d max_vec = (min_max.second - mean_vec) * 1.001 + mean_vec;

            axes_interactor_.setAxesLimits(min_vec, max_vec);
        }
        if (!view_set_)
        {
            if (is3DFunction(fcn))
            {
                const float azimuth = -64.0f * M_PI / 180.0f;
                const float elevation = 34.0f * M_PI / 180.0f;

                axes_interactor_.setViewAngles(azimuth, elevation);
            }
            else if (isImageFunction(fcn))
            {
                const float azimuth = 0.0f * M_PI / 180.0f;
                const float elevation = -90.0f * M_PI / 180.0f;

                axes_interactor_.setViewAngles(azimuth, elevation);
                view_set_ = true;  // Let imshow be dominant once used
            }
            else
            {
                const float azimuth = 0.0f * M_PI / 180.0f;
                const float elevation = 90.0f * M_PI / 180.0f;

                axes_interactor_.setViewAngles(azimuth, elevation);
            }
        }
    }

    Refresh();
}

bool PlotPane::isImageFunction(const Function fcn)
{
    return fcn == Function::IM_SHOW;
}

bool PlotPane::is3DFunction(const Function fcn)
{
    return (fcn == Function::DRAW_LINE3D) || (fcn == Function::PLANE_XY) || (fcn == Function::PLANE_XZ) ||
           (fcn == Function::PLANE_YZ) || (fcn == Function::DRAW_MESH) || (fcn == Function::DRAW_TRIANGLES_3D) ||
           (fcn == Function::DRAW_TRIANGLE_3D) || (fcn == Function::SURF) || (fcn == Function::PLOT3) ||
           (fcn == Function::SCATTER3) || (fcn == Function::DRAW_LINE_BETWEEN_POINTS_3D) ||
           (fcn == Function::POLYGON_FROM_4_POINTS) || (fcn == Function::DRAW_TILES) ||
           (fcn == Function::PLOT_COLLECTION3) || (fcn == Function::FAST_PLOT3) || (fcn == Function::LINE_COLLECTION3);
}

void PlotPane::showLegend(const bool show_legend)
{
    axes_interactor_.showLegend(show_legend);
    Refresh();
}

void PlotPane::setSelection()
{
    is_selected_ = true;
}

void PlotPane::resetSelection()
{
    is_selected_ = false;
}

void PlotPane::destroy()
{
    this->Destroy();
}

void PlotPane::mouseLeftPressed(wxMouseEvent& event)
{
    view_parent_->resetSelectionForAllChildren();
    wxCommandEvent evt(MY_EVENT, GetId());
    evt.SetEventObject(this);
    evt.SetString(element_settings_.name);
    ProcessWindowEvent(evt);

    is_selected_ = true;

    const wxPoint current_point = event.GetPosition();

    Bound2D bnd;
    bnd.x_min = 0.0f;
    bnd.x_max = this->GetSize().GetWidth();
    bnd.y_min = 0.0f;
    bnd.y_max = this->GetSize().GetHeight();

    Bound2D bnd_margin;
    bnd_margin.x_min = bnd.x_min + edit_size_margin_;
    bnd_margin.x_max = bnd.x_max - edit_size_margin_;
    bnd_margin.y_min = bnd.y_min + edit_size_margin_;
    bnd_margin.y_max = bnd.y_max - edit_size_margin_;

    cursor_state_at_press_ = mouseState(bnd, bnd_margin, Vec2f(current_point.x, current_point.y));

    mouse_pos_at_press_ = Vec2f(current_point.x, current_point.y);
    pos_at_press_ = this->GetPosition();
    size_at_press_ = this->GetSize();

    left_mouse_button_.setIsPressed(current_point.x, current_point.y);
    const wxSize size_now = this->GetSize();
    const Vec2f size_now_vec(size_now.x, size_now.y);
    axes_interactor_.registerMousePressed(mouse_pos_at_press_.elementWiseDivide(size_now_vec));
    
    Refresh();
}

void PlotPane::mouseLeftReleased(wxMouseEvent& event)
{
    const wxPoint mouse_pos{event.GetPosition()};
    const wxSize size_now = this->GetSize();
    const Vec2f size_now_vec(size_now.x, size_now.y);
    axes_interactor_.registerMouseReleased(Vec2f(mouse_pos.x, mouse_pos.y).elementWiseDivide(size_now_vec));
    left_mouse_button_.setIsReleased();
    Refresh();
}

void PlotPane::mouseLeftWindow(wxMouseEvent& WXUNUSED(event))
{
    wxSetCursor(wxCursor(wxCURSOR_ARROW));
}

void PlotPane::notifyParentAboutModification()
{
    wxCommandEvent parent_event(GUI_ELEMENT_CHANGED_EVENT);
    wxPostEvent(GetParent(), parent_event);
}

void PlotPane::mouseMoved(wxMouseEvent& event)
{
    const wxPoint current_point = event.GetPosition();
    current_mouse_pos_ = Vec2f(current_point.x, current_point.y);

    if (left_mouse_button_.isPressed())
    {
        if (is_editing_)
        {
            wxPoint pos_now = this->GetPosition();
            const Vec2f mouse_pos = Vec2f(current_point.x + pos_now.x, current_point.y + pos_now.y);
            Vec2f delta = mouse_pos - mouse_pos_at_press_;

            delta =
                Vec2f(std::round(delta.x / grid_size_) * grid_size_, std::round(delta.y / grid_size_) * grid_size_);
            const Vec2f current_pos(this->GetPosition().x, this->GetPosition().y);
            const Vec2f changed_pos = delta;

            const wxSize size_now = this->GetSize();

            wxPoint new_position = this->GetPosition();
            wxSize new_size = this->GetSize();

            switch (cursor_state_at_press_)
            {
                case CursorSquareState::LEFT:
                    new_position = wxPoint(changed_pos.x, pos_at_press_.y);
                    new_size = wxSize(size_now.GetWidth() - delta.x, size_now.GetHeight());
                    break;
                case CursorSquareState::RIGHT:
                    new_size = wxSize(size_at_press_.GetWidth() + delta.x, size_at_press_.GetHeight());
                    break;
                case CursorSquareState::TOP:
                    new_position = wxPoint(pos_at_press_.x, changed_pos.y);
                    new_size = wxSize(size_now.GetWidth(), size_now.GetHeight() - delta.y);
                    break;
                case CursorSquareState::BOTTOM:
                    new_size = wxSize(size_at_press_.GetWidth(), size_at_press_.GetHeight() + delta.y);
                    break;
                case CursorSquareState::BOTTOM_RIGHT:
                    new_size = wxSize(size_at_press_.GetWidth() + delta.x, size_at_press_.GetHeight() + delta.y);
                    break;
                case CursorSquareState::BOTTOM_LEFT:
                    new_position = wxPoint(changed_pos.x, pos_at_press_.y);
                    new_size = wxSize(size_now.GetWidth() - delta.x, size_at_press_.GetHeight() + delta.y);
                    break;
                case CursorSquareState::TOP_RIGHT:
                    new_position = wxPoint(pos_at_press_.x, changed_pos.y);
                    new_size = wxSize(size_at_press_.GetWidth() + delta.x, size_now.GetHeight() - delta.y);
                    break;
                case CursorSquareState::TOP_LEFT:
                    new_position = wxPoint(changed_pos.x, changed_pos.y);
                    new_size = wxSize(size_now.GetWidth() - delta.x, size_now.GetHeight() - delta.y);
                    break;
                case CursorSquareState::INSIDE:
                    new_position = wxPoint(changed_pos.x, changed_pos.y);
                    new_size = size_at_press_;
                    break;
                default:
                    std::cout << "Do nothing..." << std::endl;
            }
            new_size.SetWidth(std::max(50, new_size.GetWidth()));
            new_size.SetHeight(std::max(50, new_size.GetHeight()));

            const float px = parent_size_.GetWidth();
            const float py = parent_size_.GetHeight();

            element_settings_.width = static_cast<float>(new_size.GetWidth()) / px;
            element_settings_.height = static_cast<float>(new_size.GetHeight()) / py;
            element_settings_.x = static_cast<float>(new_position.x) / px;
            element_settings_.y = static_cast<float>(new_position.y) / py;

            if ((this->GetPosition().x != new_position.x) || (this->GetPosition().y != new_position.y) ||
                (new_size.GetWidth() != this->GetSize().GetWidth()) ||
                (new_size.GetHeight() != this->GetSize().GetHeight()))
            {
                notifyParentAboutModification();
                this->setPosition(new_position);
                this->setSize(new_size);
            }
        }
        else
        {
            left_mouse_button_.updateOnMotion(current_point.x, current_point.y);

            if (keyboard_state_.isPressed())
            {
                if (keyboard_state_.keyIsPressed('1'))
                {
                    current_mouse_interaction_axis_ = MouseInteractionAxis::X;
                }
                else if (keyboard_state_.keyIsPressed('2'))
                {
                    current_mouse_interaction_axis_ = MouseInteractionAxis::Y;
                }
                else if (keyboard_state_.keyIsPressed('3'))
                {
                    current_mouse_interaction_axis_ = MouseInteractionAxis::Z;
                }
            }
            else
            {
                current_mouse_interaction_axis_ = MouseInteractionAxis::ALL;
            }
            axes_interactor_.registerMouseDragInput(current_mouse_interaction_axis_,
                                                     left_mouse_button_.getDeltaPos().x,
                                                     left_mouse_button_.getDeltaPos().y);
        }

        Refresh();
    }
    else
    {
        if (is_editing_)
        {
            Bound2D bnd;
            bnd.x_min = 0.0f;
            bnd.x_max = this->GetSize().GetWidth();
            bnd.y_min = 0.0f;
            bnd.y_max = this->GetSize().GetHeight();

            Bound2D bnd_margin;
            bnd_margin.x_min = bnd.x_min + edit_size_margin_;
            bnd_margin.x_max = bnd.x_max - edit_size_margin_;
            bnd_margin.y_min = bnd.y_min + edit_size_margin_;
            bnd_margin.y_max = bnd.y_max - edit_size_margin_;
            const CursorSquareState cms = mouseState(bnd, bnd_margin, Vec2f(current_point.x, current_point.y));
            switch (cms)
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
    }
}

void PlotPane::keyPressed(const char key)
{
    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key))
    {
        keyboard_state_.keyGotPressed(key);
    }
    
    Refresh();
}

void PlotPane::keyReleased(const char key)
{
    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key))
    {
        keyboard_state_.keyGotReleased(key);
    }
    current_mouse_interaction_axis_ = MouseInteractionAxis::ALL;
    Refresh();
}

void PlotPane::keyPressedCallback(wxKeyEvent& evt)
{
    const int key = evt.GetUnicodeKey();
    notify_main_window_key_pressed_(key);
}

void PlotPane::keyReleasedCallback(wxKeyEvent& evt)
{
    const int key = evt.GetUnicodeKey();
    notify_main_window_key_released_(key);
}

// Returns window width in pixels
int PlotPane::getWidth()
{
    return GetSize().x;
}

// Returns window height in pixels
int PlotPane::getHeight()
{
    return GetSize().y;
}

InteractionType keyboardStateToInteractionType(const KeyboardState& keyboard_state)
{
    if (wxGetKeyState(static_cast<wxKeyCode>('c')))
    {
        return InteractionType::RESET;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('t')) || wxGetKeyState(static_cast<wxKeyCode>('T')))
    {
        return InteractionType::PAN;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('r')) || wxGetKeyState(static_cast<wxKeyCode>('R')))
    {
        return InteractionType::ROTATE;
    }
    else if(wxGetKeyState(static_cast<wxKeyCode>('z')) || wxGetKeyState(static_cast<wxKeyCode>('Z')))
    {
        return InteractionType::ZOOM;
    }
    else
    {
        return InteractionType::UNCHANGED;
    }
}

void PlotPane::refresh()
{
    Refresh();
}

void PlotPane::render(wxPaintEvent& evt)
{
    (void)evt;
    if (!IsShown())
        return;

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);

    glEnable(GL_MULTISAMPLE);

    const float bg_color = 240.0f;

    glClearColor(bg_color / 255.0f, bg_color / 255.0f, bg_color / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // TODO: Move to keypressed? And deal with key hold...
    if(keyboard_state_.keyIsPressed('p') || keyboard_state_.keyIsPressed('P'))
    {
        perspective_projection_ = !perspective_projection_;
    }

    axes_interactor_.update(
        keyboardStateToInteractionType(keyboard_state_), getWidth(), getHeight());

    if(wxGetKeyState(static_cast<wxKeyCode>('y')) || wxGetKeyState(static_cast<wxKeyCode>('Y')))
    {
        legend_scale_factor_ -= 0.05;
    }
    else if(wxGetKeyState(static_cast<wxKeyCode>('u')) || wxGetKeyState(static_cast<wxKeyCode>('U')))
    {
        legend_scale_factor_ += 0.05;
    }

    const bool draw_selected_bb = is_selected_ && is_editing_;

    const Vec2f pane_size(GetSize().x, GetSize().y);

    axes_renderer_->updateStates(axes_interactor_.getAxesLimits(),
                         axes_interactor_.getViewAngles(),
                         axes_interactor_.generateGridVectors(),
                         perspective_projection_,
                         getWidth(),
                         getHeight(),
                         mouse_pos_at_press_.elementWiseDivide(pane_size),
                         current_mouse_pos_.elementWiseDivide(pane_size),
                         axes_interactor_.getCurrentMouseActivity(),
                         left_mouse_button_.isPressed(),
                         axes_interactor_.shouldDrawZoomRect(),
                         axes_interactor_.getShowLegend(),
                         legend_scale_factor_,
                         plot_data_handler_->getLegendStrings());

    axes_renderer_->render();
    glEnable(GL_DEPTH_TEST);  // TODO: Put in "plotBegin" and "plotEnd"?
    axes_renderer_->plotBegin();

    plot_data_handler_->render();

    axes_renderer_->plotEnd();
    glDisable(GL_DEPTH_TEST);

    // glFlush();
    SwapBuffers();
}
