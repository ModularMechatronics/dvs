#include "plot_pane.h"

#include <wx/event.h>
#include <wx/glcanvas.h>

#include "axes/axes.h"
#include "axes/axes_side_configuration.h"
#include "dvs/enumerations.h"
#include "dvs/math/math.h"
#include "events.h"
#include "mouse_state.h"

using namespace dvs::internal;

CursorSquareState getCursorSquareState(const Bound2D bound, const Bound2D bound_margin, const Vec2f mouse_pos)
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

void PlotPane::raise()
{
    Raise();
}

void PlotPane::lower()
{
    Lower();
}

void PlotPane::initShaders()
{
    const std::string v_path = "../main_application/axes/shaders/plot_box_shader.vs";
    const std::string f_path = "../main_application/axes/shaders/plot_box_shader.fs";
    shader_collection_.plot_box_shader = ShaderBase(v_path, f_path, ShaderSource::FILE);

    const std::string v_path_text = "../main_application/axes/shaders/text.vs";
    const std::string f_path_text = "../main_application/axes/shaders/text.fs";
    shader_collection_.text_shader = ShaderBase(v_path_text, f_path_text, ShaderSource::FILE);

    const std::string v_path_basic_plot_shader = "../main_application/axes/shaders/basic_plot_shader.vs";
    const std::string f_path_basic_plot_shader = "../main_application/axes/shaders/basic_plot_shader.fs";
    shader_collection_.basic_plot_shader =
        ShaderBase(v_path_basic_plot_shader, f_path_basic_plot_shader, ShaderSource::FILE);

    const std::string v_path_plot_2d_shader = "../main_application/axes/shaders/plot_2d_shader.vs";
    const std::string f_path_plot_2d_shader = "../main_application/axes/shaders/plot_2d_shader.fs";
    shader_collection_.plot_2d_shader = Plot2DShader(v_path_plot_2d_shader, f_path_plot_2d_shader, ShaderSource::FILE);

    const std::string v_path_plot_3d_shader = "../main_application/axes/shaders/plot_3d_shader.vs";
    const std::string f_path_plot_3d_shader = "../main_application/axes/shaders/plot_3d_shader.fs";
    shader_collection_.plot_3d_shader = ShaderBase(v_path_plot_3d_shader, f_path_plot_3d_shader, ShaderSource::FILE);

    const std::string v_path_img_plot_shader = "../main_application/axes/shaders/img.vs";
    const std::string f_path_img_plot_shader = "../main_application/axes/shaders/img.fs";
    shader_collection_.img_plot_shader = ShaderBase(v_path_img_plot_shader, f_path_img_plot_shader, ShaderSource::FILE);

    const std::string v_path_scatter_shader = "../main_application/axes/shaders/scatter_shader.vs";
    const std::string f_path_scatter_shader = "../main_application/axes/shaders/scatter_shader.fs";
    shader_collection_.scatter_shader = ShaderBase(v_path_scatter_shader, f_path_scatter_shader, ShaderSource::FILE);

    const std::string v_path_draw_mesh_shader = "../main_application/axes/shaders/draw_mesh_shader.vs";
    const std::string f_path_draw_mesh_shader = "../main_application/axes/shaders/draw_mesh_shader.fs";
    shader_collection_.draw_mesh_shader =
        ShaderBase(v_path_draw_mesh_shader, f_path_draw_mesh_shader, ShaderSource::FILE);

    const std::string v_path_legend_shader = "../main_application/axes/shaders/legend_shader.vs";
    const std::string f_path_legend_shader = "../main_application/axes/shaders/legend_shader.fs";
    shader_collection_.legend_shader = ShaderBase(v_path_legend_shader, f_path_legend_shader, ShaderSource::FILE);
}

PlotPane::PlotPane(wxWindow* parent,
                   const ElementSettings& element_settings,
                   const RGBTripletf& tab_background_color,
                   const std::function<void(const char key)>& notify_main_window_key_pressed,
                   const std::function<void(const char key)>& notify_main_window_key_released,
                   const std::function<void(const wxPoint pos, const std::string& elem_name)>&
                       notify_parent_window_right_mouse_pressed,
                   const std::function<void()>& notify_main_window_about_modification)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE),
      GuiElement(element_settings,
                 notify_main_window_key_pressed,
                 notify_main_window_key_released,
                 notify_parent_window_right_mouse_pressed,
                 notify_main_window_about_modification),
      m_context(getContext()),
      axes_interactor_(axes_settings_, getWidth(), getHeight())
{
    pending_clear_ = false;
    parent_size_ = parent->GetSize();
    edit_size_margin_ = 20.0f;
    minimum_x_pos_ = 70;
    minimum_y_pos_ = 30;
    perspective_projection_ =
        (element_settings.projection_mode == ElementSettings::ProjectionMode::PERSPECTIVE) ? true : false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    wait_for_flush_ = false;
    shift_pressed_at_mouse_press_ = false;
    control_pressed_at_mouse_press_ = false;
    left_mouse_pressed_ = false;

    bindCallbacks();

    wxGLCanvas::SetCurrent(*m_context);
    initShaders();

    axes_renderer_ = new AxesRenderer(shader_collection_, element_settings, tab_background_color);
    plot_data_handler_ = new PlotDataHandler(shader_collection_);

    axes_set_ = false;
    view_set_ = false;
    axes_from_min_max_disabled_ = false;
    axes_interactor_.setOverriddenMouseInteractionType(MouseInteractionType::UNCHANGED);

    Bind(wxEVT_RIGHT_DOWN, &PlotPane::mouseRightPressed, this);
    Bind(wxEVT_MIDDLE_DOWN, &PlotPane::mouseMiddlePressed, this);

    Bind(wxEVT_MIDDLE_UP, &PlotPane::mouseMiddleReleased, this);
    Bind(wxEVT_RIGHT_UP, &PlotPane::mouseRightReleased, this);

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    current_mouse_interaction_axis_ = MouseInteractionAxis::ALL;
}

void PlotPane::updateSizeFromParent(const wxSize& parent_size)
{
    parent_size_ = parent_size;

    setElementPositionAndSize();
}

void PlotPane::setMinimumXPos(const int new_min_x_pos)
{
    minimum_x_pos_ = new_min_x_pos;
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
    Bind(wxEVT_ENTER_WINDOW, &PlotPane::mouseEntered, this);
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
    delete plot_data_handler_;
    delete m_context;
}

void PlotPane::addSettingsData(const ReceivedData& received_data,
                               const PlotObjectAttributes& plot_object_attributes,
                               const PropertiesData& properties_data)
{
    const dvs::internal::CommunicationHeader& hdr = received_data.getCommunicationHeader();
    const Function fcn = hdr.getFunction();

    if (fcn == Function::AXES_2D)
    {
        axes_set_ = true;

        const std::pair<Vec3d, Vec3d> axes_bnd =
            hdr.get(CommunicationHeaderObjectType::AXIS_MIN_MAX_VEC).as<std::pair<Vec3d, Vec3d>>();
        axes_interactor_.setAxesLimits(Vec2d(axes_bnd.first.x, axes_bnd.first.y),
                                       Vec2d(axes_bnd.second.x, axes_bnd.second.y));
    }
    else if (fcn == Function::AXES_3D)
    {
        axes_set_ = true;

        const std::pair<Vec3d, Vec3d> axes_bnd =
            hdr.get(CommunicationHeaderObjectType::AXIS_MIN_MAX_VEC).as<std::pair<Vec3d, Vec3d>>();
        axes_interactor_.setAxesLimits(Vec3d(axes_bnd.first.x, axes_bnd.first.y, axes_bnd.first.z),
                                       Vec3d(axes_bnd.second.x, axes_bnd.second.y, axes_bnd.second.z));
    }
    else if (fcn == Function::PROPERTIES_EXTENSION)
    {
        plot_data_handler_->propertiesExtension(plot_object_attributes.id, properties_data);
    }
    else if (fcn == Function::GLOBAL_ILLUMINATION)
    {
        axes_renderer_->activateGlobalIllumination(hdr.get(CommunicationHeaderObjectType::VEC3).as<Vec3d>());
    }
    else if (fcn == Function::VIEW)
    {
        view_set_ = true;
        const float azimuth = hdr.get(internal::CommunicationHeaderObjectType::AZIMUTH).as<float>();
        const float elevation = hdr.get(internal::CommunicationHeaderObjectType::ELEVATION).as<float>();

        const float azimuth_rad = azimuth * M_PI / 180.0f;
        const float elevation_rad = elevation * M_PI / 180.0f;

        axes_interactor_.setViewAngles(azimuth_rad, elevation_rad);
    }
    else if (fcn == Function::CLEAR)
    {
        clearPane();
    }
    else if (fcn == Function::SHOW_LEGEND)
    {
        axes_interactor_.showLegend(true);
    }
    else if (fcn == Function::WAIT_FOR_FLUSH)
    {
        wait_for_flush_ = true;
    }
    else if (fcn == Function::FLUSH_ELEMENT)
    {
        // Do nothing...
    }
    else if (fcn == Function::SOFT_CLEAR)
    {
        plot_data_handler_->softClear();
    }
    else if (fcn == Function::DISABLE_AXES_FROM_MIN_MAX)
    {
        axes_from_min_max_disabled_ = true;
    }
    else if (fcn == Function::SET_AXES_BOX_SCALE_FACTOR)
    {
        const Vec3d scale_vec = hdr.get(CommunicationHeaderObjectType::VEC3).as<Vec3d>() * 3.0;
        axes_renderer_->setAxesBoxScaleFactor(scale_vec);
    }
    else if (fcn == Function::SET_OBJECT_TRANSFORM)
    {
        const Vec3d translation_vec = hdr.get(CommunicationHeaderObjectType::TRANSLATION_VECTOR).as<Vec3d>();
        const MatrixFixed<double, 3, 3> rotation_mat =
            hdr.get(CommunicationHeaderObjectType::ROTATION_MATRIX).as<MatrixFixed<double, 3, 3>>();

        const ItemId id = hdr.get(CommunicationHeaderObjectType::ITEM_ID).as<internal::ItemId>();

        const MatrixFixed<double, 3, 3> scale =
            hdr.get(CommunicationHeaderObjectType::SCALE_MATRIX).as<MatrixFixed<double, 3, 3>>();

        const MatrixFixed<double, 3, 3> inv_rotation_mat = rotation_mat.transposed();
        plot_data_handler_->setTransform(id, inv_rotation_mat, translation_vec, scale);
    }
    else
    {
        throw std::runtime_error("Invalid function!");
    }
}

void PlotPane::pushQueue(std::queue<std::unique_ptr<InputData>>& new_queue)
{
    const size_t queue_size = new_queue.size();

    for (size_t k = 0; k < queue_size; k++)
    {
        queued_data_.push(std::move(new_queue.front()));
        new_queue.pop();
    }
}

void PlotPane::update()
{
    if (!queued_data_.empty())
    {
        Refresh();
    }
}

bool PlotPane::isImageFunction(const Function fcn)
{
    return fcn == Function::IM_SHOW;
}

bool PlotPane::is3DFunction(const Function fcn)
{
    return (fcn == Function::DRAW_LINE3D) || (fcn == Function::PLANE_XY) || (fcn == Function::PLANE_XZ) ||
           (fcn == Function::PLANE_YZ) || (fcn == Function::DRAW_MESH) ||
           (fcn == Function::DRAW_MESH_SEPARATE_VECTORS) || (fcn == Function::DRAW_TRIANGLES_3D) ||
           (fcn == Function::DRAW_TRIANGLE_3D) || (fcn == Function::SURF) || (fcn == Function::PLOT3) ||
           (fcn == Function::SCATTER3) || (fcn == Function::DRAW_LINE_BETWEEN_POINTS_3D) ||
           (fcn == Function::POLYGON_FROM_4_POINTS) || (fcn == Function::DRAW_TILES) ||
           (fcn == Function::PLOT_COLLECTION3) || (fcn == Function::FAST_PLOT3) || (fcn == Function::LINE_COLLECTION3);
}

void PlotPane::waitForFlush()
{
    wait_for_flush_ = true;
}

void PlotPane::toggleProjectionMode()
{
    perspective_projection_ = !perspective_projection_;
    element_settings_.projection_mode = perspective_projection_ ? ElementSettings::ProjectionMode::PERSPECTIVE
                                                                : ElementSettings::ProjectionMode::ORTHOGRAPHIC;
    Refresh();
}

/*void PlotPane::showLegend(const bool show_legend)
{
    axes_interactor_.showLegend(show_legend);
    if (!wait_for_flush_)
    {
        Refresh();
    }
}*/

void PlotPane::destroy()
{
    this->Destroy();
}

void PlotPane::setName(const std::string& new_name)
{
    element_settings_.name = new_name;
    Refresh();
}

void PlotPane::mouseLeftPressed(wxMouseEvent& event)
{
    const wxPoint current_mouse_position = event.GetPosition();
    pos_at_press_ = this->GetPosition();
    size_at_press_ = this->GetSize();

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

    cursor_state_at_press_ =
        getCursorSquareState(bnd, bnd_margin, Vec2f(current_mouse_position.x, current_mouse_position.y));

    mouse_pos_at_press_ = Vec2f(current_mouse_position.x, current_mouse_position.y);

    mouse_state_.setCurrentPos(current_mouse_position.x, current_mouse_position.y);

    axes_interactor_.registerMousePressed(
        mouse_pos_at_press_.elementWiseDivide(Vec2f(size_at_press_.x, size_at_press_.y)));

    if (wxGetKeyState(WXK_CONTROL))
    {
        control_pressed_at_mouse_press_ = true;
    }
    else if (wxGetKeyState(WXK_SHIFT))
    {
        shift_pressed_at_mouse_press_ = true;
        axes_interactor_.setOverriddenMouseInteractionType(MouseInteractionType::ROTATE);
    }

    left_mouse_pressed_ = true;

    Refresh();
}

void PlotPane::mouseLeftReleased(wxMouseEvent& event)
{
    const wxPoint mouse_pos{event.GetPosition()};
    const wxSize size_now = this->GetSize();
    const Vec2f size_now_vec(size_now.x, size_now.y);
    axes_interactor_.registerMouseReleased(Vec2f(mouse_pos.x, mouse_pos.y).elementWiseDivide(size_now_vec));

    control_pressed_at_mouse_press_ = false;
    left_mouse_pressed_ = false;

    if (shift_pressed_at_mouse_press_)
    {
        shift_pressed_at_mouse_press_ = false;
        axes_interactor_.setOverriddenMouseInteractionType(MouseInteractionType::UNCHANGED);
    }

    Refresh();
}

void PlotPane::mouseMiddlePressed(wxMouseEvent& event)
{
    const wxPoint current_point = event.GetPosition();

    mouse_state_.setCurrentPos(current_point.x, current_point.y);

    if (wxGetKeyState(WXK_SHIFT))
    {
        shift_pressed_at_mouse_press_ = true;
        axes_interactor_.setOverriddenMouseInteractionType(MouseInteractionType::PAN);
    }
}

void PlotPane::mouseMiddleReleased(wxMouseEvent& event)
{
    if (shift_pressed_at_mouse_press_)
    {
        shift_pressed_at_mouse_press_ = false;
        axes_interactor_.setOverriddenMouseInteractionType(MouseInteractionType::UNCHANGED);
    }

    Refresh();
}

void PlotPane::mouseRightPressed(wxMouseEvent& event)
{
    const wxPoint current_point = event.GetPosition();

    mouse_state_.setCurrentPos(current_point.x, current_point.y);

    if (wxGetKeyState(WXK_SHIFT))
    {
        shift_pressed_at_mouse_press_ = true;
        axes_interactor_.setOverriddenMouseInteractionType(MouseInteractionType::ZOOM);
    }
    else
    {
        notify_parent_window_right_mouse_pressed_(this->GetPosition() + event.GetPosition(), element_settings_.name);
    }
}

void PlotPane::mouseRightReleased(wxMouseEvent& event)
{
    if (event.RightIsDown())
    {
        if (shift_pressed_at_mouse_press_)
        {
            shift_pressed_at_mouse_press_ = false;
            axes_interactor_.setOverriddenMouseInteractionType(MouseInteractionType::UNCHANGED);
        }

        Refresh();
    }
}

void PlotPane::mouseLeftWindow(wxMouseEvent& event)
{
    // if (!(event.LeftIsDown() && control_pressed_at_mouse_press_))
    {
        wxSetCursor(wxCursor(wxCURSOR_ARROW));
    }
}

void PlotPane::mouseEntered(wxMouseEvent& event)
{
    const wxPoint current_mouse_point = event.GetPosition();
    const wxPoint current_position = this->GetPosition();
    previous_mouse_pos_ = Vec2f(current_position.x + current_mouse_point.x, current_position.y + current_mouse_point.y);
}

void PlotPane::adjustPaneSizeOnMouseMoved()
{
    const Vec2f delta = current_mouse_pos_ - previous_mouse_pos_;

    const Vec2f current_pos(this->GetPosition().x, this->GetPosition().y);

    const wxSize size_now = this->GetSize();

    wxPoint new_position = this->GetPosition();
    wxSize new_size = this->GetSize();

    switch (cursor_state_at_press_)
    {
        case CursorSquareState::LEFT:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y);
            new_size = wxSize(size_now.GetWidth() - delta.x, size_now.GetHeight());
            break;
        case CursorSquareState::RIGHT:
            new_size = wxSize(size_now.GetWidth() + delta.x, size_now.GetHeight());
            break;
        case CursorSquareState::TOP:
            new_position = wxPoint(current_pos.x, current_pos.y + delta.y);
            new_size = wxSize(size_now.GetWidth(), size_now.GetHeight() - delta.y);
            break;
        case CursorSquareState::BOTTOM:
            new_size = wxSize(size_now.GetWidth(), size_now.GetHeight() + delta.y);
            break;
        case CursorSquareState::INSIDE:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y + delta.y);
            new_size = size_at_press_;
            break;
        case CursorSquareState::BOTTOM_RIGHT:
            new_size = wxSize(size_now.GetWidth() + delta.x, size_now.GetHeight() + delta.y);
            break;
        case CursorSquareState::BOTTOM_LEFT:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y);
            new_size = wxSize(size_now.GetWidth() - delta.x, size_now.GetHeight() + delta.y);
            break;
        case CursorSquareState::TOP_RIGHT:
            new_position = wxPoint(current_pos.x, current_pos.y + delta.y);
            new_size = wxSize(size_now.GetWidth() + delta.x, size_now.GetHeight() - delta.y);
            break;
        case CursorSquareState::TOP_LEFT:
            new_position = wxPoint(current_pos.x + delta.x, current_pos.y + delta.y);
            new_size = wxSize(size_now.GetWidth() - delta.x, size_now.GetHeight() - delta.y);
            break;
        default:
            std::cout << "Invalid cursor state!" << std::endl;
    }
    if (new_size.GetWidth() < 50)
    {
        new_size.SetWidth(50);
        new_position.x = current_pos.x;
    }
    if (new_size.GetHeight() < 50)
    {
        new_size.SetHeight(50);
        new_position.y = current_pos.y;
    }

    const float px = parent_size_.GetWidth();
    const float py = parent_size_.GetHeight();

    if (new_position.x < minimum_x_pos_)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.x = current_pos.x;
        }
        else
        {
            new_size.SetWidth(size_now.GetWidth());
            new_position.x = current_pos.x;
        }
    }
    else if ((new_position.x + new_size.GetWidth()) > px)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.x = current_pos.x;
        }
        else
        {
            new_size.SetWidth(size_now.GetWidth());
        }
    }

    if (new_position.y < minimum_y_pos_)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.y = current_pos.y;
        }
        else
        {
            new_size.SetHeight(size_now.GetHeight());
            new_position.y = current_pos.y;
        }
    }
    else if ((new_position.y + new_size.GetHeight()) > py)
    {
        if (cursor_state_at_press_ == CursorSquareState::INSIDE)
        {
            new_position.y = current_pos.y;
        }
        else
        {
            new_size.SetHeight(size_now.GetHeight());
        }
    }

    if ((this->GetPosition().x != new_position.x) || (this->GetPosition().y != new_position.y) ||
        (new_size.GetWidth() != this->GetSize().GetWidth()) || (new_size.GetHeight() != this->GetSize().GetHeight()))
    {
        const float ratio_x = 1.0f - static_cast<float>(minimum_x_pos_) / px;
        const float ratio_y = 1.0f - static_cast<float>(minimum_y_pos_) / py;

        element_settings_.width = static_cast<float>(new_size.GetWidth()) / (px * ratio_x);
        element_settings_.height = static_cast<float>(new_size.GetHeight()) / (py * ratio_y);
        element_settings_.x = static_cast<float>(new_position.x - minimum_x_pos_) / (px * ratio_x);
        element_settings_.y = static_cast<float>(new_position.y - minimum_y_pos_) / (py * ratio_y);

        Unbind(wxEVT_MOTION, &PlotPane::mouseMoved, this);  // TODO: Needed?
        notify_main_window_about_modification_();
        setElementPositionAndSize();
        Bind(wxEVT_MOTION, &PlotPane::mouseMoved, this);
    }
}

void PlotPane::setCursorDependingOnMousePos(const wxPoint& current_mouse_position)
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
    const CursorSquareState cms =
        getCursorSquareState(bnd, bnd_margin, Vec2f(current_mouse_position.x, current_mouse_position.y));
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

void PlotPane::mouseMoved(wxMouseEvent& event)
{
    const wxPoint current_mouse_position = event.GetPosition();
    const wxPoint current_pane_position = this->GetPosition();

    current_mouse_pos_ =
        Vec2f(current_pane_position.x + current_mouse_position.x, current_pane_position.y + current_mouse_position.y);

    if ((event.LeftIsDown() || event.MiddleIsDown() || event.RightIsDown()))
    {
        if (event.LeftIsDown() && control_pressed_at_mouse_press_)
        {
            adjustPaneSizeOnMouseMoved();
        }
        else if (shift_pressed_at_mouse_press_)
        {
            mouse_state_.updateOnMotion(current_mouse_position.x, current_mouse_position.y);

            axes_interactor_.registerMouseDragInput(
                current_mouse_interaction_axis_, mouse_state_.getDeltaPos().x, mouse_state_.getDeltaPos().y);
        }
        else if (event.LeftIsDown())
        {
            mouse_state_.updateOnMotion(current_mouse_position.x, current_mouse_position.y);

            axes_interactor_.registerMouseDragInput(
                current_mouse_interaction_axis_, mouse_state_.getDeltaPos().x, mouse_state_.getDeltaPos().y);
        }

        Refresh();
    }
    else
    {
        if (wxGetKeyState(WXK_COMMAND))
        {
            setCursorDependingOnMousePos(current_mouse_position);
        }
    }

    previous_mouse_pos_ = current_mouse_pos_;
}

void PlotPane::keyPressed(const char key)
{
    if (wxGetKeyState(static_cast<wxKeyCode>('1')) && wxGetKeyState(static_cast<wxKeyCode>('2')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::XY;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('2')) && wxGetKeyState(static_cast<wxKeyCode>('3')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::YZ;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('1')) && wxGetKeyState(static_cast<wxKeyCode>('3')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::XZ;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('1')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::X;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('2')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::Y;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('3')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::Z;
    }

    if (wxGetKeyState(static_cast<wxKeyCode>('r')) || wxGetKeyState(static_cast<wxKeyCode>('R')))
    {
        axes_interactor_.setMouseInteractionType(MouseInteractionType::ROTATE);
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('z')) || wxGetKeyState(static_cast<wxKeyCode>('Z')))
    {
        axes_interactor_.setMouseInteractionType(MouseInteractionType::ZOOM);
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('p')) || wxGetKeyState(static_cast<wxKeyCode>('P')))
    {
        axes_interactor_.setMouseInteractionType(MouseInteractionType::PAN);
    }

    Refresh();
}

void PlotPane::keyReleased(const char key)
{
    current_mouse_interaction_axis_ = MouseInteractionAxis::ALL;

    if (wxGetKeyState(static_cast<wxKeyCode>('1')) && wxGetKeyState(static_cast<wxKeyCode>('2')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::XY;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('2')) && wxGetKeyState(static_cast<wxKeyCode>('3')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::YZ;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('1')) && wxGetKeyState(static_cast<wxKeyCode>('3')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::XZ;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('1')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::X;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('2')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::Y;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('3')))
    {
        current_mouse_interaction_axis_ = MouseInteractionAxis::Z;
    }

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

void PlotPane::setElementPositionAndSize()
{
    const float px = parent_size_.GetWidth();
    const float py = parent_size_.GetHeight();

    wxPoint new_pos;
    wxSize new_size;

    const float ratio_x = 1.0f - static_cast<float>(minimum_x_pos_) / px;
    const float ratio_y = 1.0f - static_cast<float>(minimum_y_pos_) / py;

    new_size.SetWidth(element_settings_.width * px * ratio_x);
    new_size.SetHeight(element_settings_.height * py * ratio_y);
    new_pos.x = minimum_x_pos_ + element_settings_.x * px * ratio_x;
    new_pos.y = minimum_y_pos_ + element_settings_.y * py * ratio_y;

    SetPosition(new_pos);
    setSize(new_size);
}

void PlotPane::setMinXPos(const int min_x_pos)
{
    minimum_x_pos_ = min_x_pos;
    setElementPositionAndSize();
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

MouseInteractionType getMouseInteractionType()
{
    if (wxGetKeyState(static_cast<wxKeyCode>('p')) || wxGetKeyState(static_cast<wxKeyCode>('P')))
    {
        return MouseInteractionType::PAN;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('r')) || wxGetKeyState(static_cast<wxKeyCode>('R')))
    {
        return MouseInteractionType::ROTATE;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('z')) || wxGetKeyState(static_cast<wxKeyCode>('Z')))
    {
        return MouseInteractionType::ZOOM;
    }
    else
    {
        return MouseInteractionType::UNCHANGED;
    }
}

bool viewShouldBeReset()
{
    if (wxGetKeyState(static_cast<wxKeyCode>('c')))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void PlotPane::addPlotData(ReceivedData& received_data,
                           const PlotObjectAttributes& plot_object_attributes,
                           const PropertiesData& properties_data,
                           std::unique_ptr<const ConvertedDataBase>& converted_data)
{
    const CommunicationHeader& hdr = received_data.getCommunicationHeader();

    plot_data_handler_->addData(hdr, plot_object_attributes, properties_data, received_data, converted_data);

    internal::Function fcn = hdr.getFunction();

    // TODO: The fundamental design of this min/max needs to be changed.
    // For some types of plot data, it shouldn't be executed if axes is set,
    // but for some it should always be executed, for drawMesh for example,
    // to correctly set the color map min max values
    // And if it has already been executed once for a plot data, it shouldn't need
    // to be executed again. Also, if there are multiple color map plot datas
    // they should get their color map min max values from a global min/max of
    // all the plot datas
    // The whole getMinMaxVectors logic in plot_datas should probably also be redesigned
    const std::pair<Vec3d, Vec3d> min_max = plot_data_handler_->getMinMaxVectors();

    if ((!axes_set_) && (!axes_from_min_max_disabled_))
    {
        const Vec3d mean_vec = (min_max.second + min_max.first) / 2.0;

        const Vec3d min_vec = (min_max.first - mean_vec) * 1.001 + mean_vec;
        const Vec3d max_vec = (min_max.second - mean_vec) * 1.001 + mean_vec;

        axes_interactor_.setAxesLimits(min_vec, max_vec);
    }
    if ((!view_set_) && (!axes_from_min_max_disabled_))
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

void PlotPane::clearPane()
{
    plot_data_handler_->clear();
    pending_clear_ = true;
    axes_set_ = false;
    view_set_ = false;
    wait_for_flush_ = false;
    axes_from_min_max_disabled_ = false;

    axes_interactor_.setViewAngles(0, M_PI);
    axes_interactor_.setAxesLimits(Vec3d(-1.0, -1.0, -1.0), Vec3d(1.0, 1.0, 1.0));
    axes_interactor_.showLegend(false);
    axes_renderer_->resetGlobalIllumination();
    axes_renderer_->setAxesBoxScaleFactor(Vec3d{2.5, 2.5, 2.5});
}

void PlotPane::processActionQueue()
{
    while (!queued_data_.empty())
    {
        const internal::Function fcn = queued_data_.front()->getFunction();

        if (wait_for_flush_)
        {
            if (fcn == Function::FLUSH_ELEMENT)
            {
                while (!flush_queue_.empty())
                {
                    const internal::Function inner_fcn = flush_queue_.front()->getFunction();

                    if (isPlotDataFunction(inner_fcn))
                    {
                        auto [received_data, plot_object_attributes, properties_data, converted_data] =
                            flush_queue_.front()->moveAllData();
                        addPlotData(received_data, plot_object_attributes, properties_data, converted_data);
                    }
                    else
                    {
                        auto [received_data, plot_object_attributes, properties_data] =
                            flush_queue_.front()->moveAllDataButConvertedData();
                        addSettingsData(received_data, plot_object_attributes, properties_data);
                    }

                    flush_queue_.pop();
                }
                queued_data_.pop();
                break;
            }
            else if (fcn == Function::CLEAR)
            {
                clearPane();
                while (!flush_queue_.empty())
                {
                    flush_queue_.pop();
                }
                queued_data_.pop();
            }
            else
            {
                flush_queue_.push(std::move(queued_data_.front()));
                queued_data_.pop();
            }
        }
        else
        {
            if (isPlotDataFunction(fcn))
            {
                auto [received_data, plot_object_attributes, properties_data, converted_data] =
                    queued_data_.front()->moveAllData();

                addPlotData(received_data, plot_object_attributes, properties_data, converted_data);
            }
            else
            {
                auto [received_data, plot_object_attributes, properties_data] =
                    queued_data_.front()->moveAllDataButConvertedData();
                addSettingsData(received_data, plot_object_attributes, properties_data);
            }

            queued_data_.pop();

            if (fcn == Function::SOFT_CLEAR)
            {
                break;
            }
        }
    }
}

void PlotPane::refresh()
{
    Refresh();
}

void PlotPane::render(wxPaintEvent& WXUNUSED(evt))
{
    if (!IsShown())
    {
        return;
    }

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);  // TODO: Can be removed?

    processActionQueue();

    /*if (pending_clear_)
    {
        pending_clear_ = false;
        plot_data_handler_->clear();
    }*/

    glEnable(GL_MULTISAMPLE);

    const RGBTripletf color_vec{element_settings_.background_color};
    glClearColor(color_vec.red, color_vec.green, color_vec.blue, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (viewShouldBeReset())
    {
        axes_interactor_.resetView();
    }

    axes_interactor_.updateWindowSize(getWidth(), getHeight());

    if (wxGetKeyState(static_cast<wxKeyCode>('y')) || wxGetKeyState(static_cast<wxKeyCode>('Y')))
    {
        legend_scale_factor_ -= 0.05;
    }
    else if (wxGetKeyState(static_cast<wxKeyCode>('u')) || wxGetKeyState(static_cast<wxKeyCode>('U')))
    {
        legend_scale_factor_ += 0.05;
    }

    const Vec2f pane_size(GetSize().x, GetSize().y);

    const AxesSideConfiguration axes_side_configuration{axes_interactor_.getViewAngles(), perspective_projection_};

    axes_renderer_->updateStates(axes_interactor_.getAxesLimits(),
                                 axes_interactor_.getViewAngles(),
                                 axes_interactor_.generateGridVectors(),
                                 axes_side_configuration,
                                 perspective_projection_,
                                 getWidth(),
                                 getHeight(),
                                 mouse_pos_at_press_.elementWiseDivide(pane_size),
                                 current_mouse_pos_.elementWiseDivide(pane_size),
                                 axes_interactor_.getMouseInteractionType(),
                                 left_mouse_pressed_,
                                 axes_interactor_.shouldDrawZoomRect(),
                                 axes_interactor_.getShowLegend(),
                                 legend_scale_factor_,
                                 plot_data_handler_->getLegendStrings(),
                                 element_settings_.name,
                                 current_mouse_interaction_axis_);

    axes_renderer_->render();
    glEnable(GL_DEPTH_TEST);  // TODO: Put in "plotBegin" and "plotEnd"?
    axes_renderer_->plotBegin();

    plot_data_handler_->render();

    axes_renderer_->plotEnd();
    glDisable(GL_DEPTH_TEST);

    // glFlush();
    SwapBuffers();
}
