#include "plot_window_gl_pane.h"


#include <wx/event.h>

#include "math/math.h"
#include "axes/axes.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_low_level.h"

#include "enumerations.h"

using namespace dvs::internal;


PlotWindowGLPane::PlotWindowGLPane(wxNotebookPage* parent, const Element& element_settings, const wxPoint& position, const wxSize& size)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE), GuiElement(element_settings)
{
    m_context = new wxGLContext(this);
    is_editing_ = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    const float min_x = -1.0;
    const float max_x = 1.0;
    const float min_y = -1.0;
    const float max_y = 1.0;
    const float min_z = -1.0;
    const float max_z = 1.0;

    const AxesSettings axes_settings({min_x, min_y, min_z}, {max_x, max_y, max_z});

    bindCallbacks();

    axes_interactor_ = new AxesInteractor(axes_settings);
    axes_painter_ = new AxesPainter(axes_settings);

    hold_on_ = false;
    axes_set_ = false;

    glEnable(GL_MULTISAMPLE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}

void PlotWindowGLPane::updateSize(const Vec2Df& tab_cell_size)
{
    const wxSize size(element_settings_.width * tab_cell_size.x, element_settings_.height * tab_cell_size.y);
    const wxPoint pos(element_settings_.cell_idx_x * tab_cell_size.x, element_settings_.cell_idx_y * tab_cell_size.y);

    this->SetPosition(pos);
    this->SetSize(size);
}

void PlotWindowGLPane::show()
{
    this->Show();
}

void PlotWindowGLPane::hide()
{
    this->Hide();
}

void PlotWindowGLPane::bindCallbacks()
{
    Bind(wxEVT_MOTION, &PlotWindowGLPane::mouseMoved, this);
    Bind(wxEVT_LEFT_DOWN, &PlotWindowGLPane::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &PlotWindowGLPane::mouseLeftReleased, this);
    Bind(wxEVT_KEY_DOWN, &PlotWindowGLPane::keyPressed, this);
    Bind(wxEVT_KEY_UP, &PlotWindowGLPane::keyReleased, this);
    Bind(wxEVT_PAINT, &PlotWindowGLPane::render, this);
}

void PlotWindowGLPane::unbindCallbacks()
{
    Unbind(wxEVT_MOTION, &PlotWindowGLPane::mouseMoved, this);
    Unbind(wxEVT_LEFT_DOWN, &PlotWindowGLPane::mouseLeftPressed, this);
    Unbind(wxEVT_LEFT_UP, &PlotWindowGLPane::mouseLeftReleased, this);
    Unbind(wxEVT_KEY_DOWN, &PlotWindowGLPane::keyPressed, this);
    Unbind(wxEVT_KEY_UP, &PlotWindowGLPane::keyReleased, this);
    Unbind(wxEVT_PAINT, &PlotWindowGLPane::render, this);
}

void PlotWindowGLPane::setPosAndSize(const wxPoint& pos, const wxSize& size)
{
    this->SetPosition(pos);
    this->SetSize(size);
}

int* PlotWindowGLPane::getArgsPtr()
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

PlotWindowGLPane::~PlotWindowGLPane()
{
    delete m_context;
}

void PlotWindowGLPane::addData(std::unique_ptr<const ReceivedData> received_data, const dvs::internal::FunctionHeader& hdr)
{
    const internal::Function fcn = hdr.getObjectAtIdx(0).getAs<internal::Function>();

    if (fcn == Function::HOLD_ON)
    {
        hold_on_ = true;
    }
    else if (fcn == Function::AXES_2D)
    {
        axes_set_ = true;

        const AxesBounds axes_bnd = hdr.getObjectFromType(FunctionHeaderObjectType::AXIS_MIN_MAX_VEC).getAs<AxesBounds>();
        axes_interactor_->setAxesLimits(Vec2Dd(axes_bnd.lower.x, axes_bnd.lower.y),
                                        Vec2Dd(axes_bnd.upper.x, axes_bnd.upper.y));
    }
    else if (fcn == Function::AXES_3D)
    {
        axes_set_ = true;

        const AxesBounds axes_bnd = hdr.getObjectFromType(FunctionHeaderObjectType::AXIS_MIN_MAX_VEC).getAs<AxesBounds>();
        axes_interactor_->setAxesLimits(Vec3Dd(axes_bnd.lower.x, axes_bnd.lower.y, axes_bnd.lower.z),
                                        Vec3Dd(axes_bnd.upper.x, axes_bnd.upper.y, axes_bnd.upper.z));
    }
    else if (fcn == Function::VIEW)
    {
        const float azimuth = hdr.getObjectFromType(internal::FunctionHeaderObjectType::AZIMUTH).getAs<float>();
        const float elevation = hdr.getObjectFromType(internal::FunctionHeaderObjectType::ELEVATION).getAs<float>();

        axes_interactor_->setViewAngles(azimuth, elevation);
    }
    else if (fcn == Function::CLEAR)
    {
        axes_set_ = false;
        hold_on_ = false;
        plot_data_handler_.clear();
    }
    else if (fcn == Function::SOFT_CLEAR)
    {
        plot_data_handler_.softClear();
    }
    else
    {
        if (!hold_on_)
        {
            plot_data_handler_.clear();
        }
        plot_data_handler_.addData(std::move(received_data), hdr);
        std::cout << "Adding data..." << std::endl;

        /*if (!axes_set_)
        {
            const std::pair<Vec3Dd, Vec3Dd> min_max =
                // plot_data_handler_.getMinMaxVectors();
            axes_interactor_->setAxesLimits(min_max.first, min_max.second);
        }*/
    }

    // TODO: Add "holdClear" that only clears when new data comes in, to avoid flashing

    Refresh();
}

void PlotWindowGLPane::mouseLeftPressed(wxMouseEvent& event)
{
    const wxPoint current_point = event.GetPosition();
    pos_at_press_ = Vec2Df(current_point.x, current_point.y);

    left_mouse_button_.setIsPressed(current_point.x, current_point.y);
    Refresh();
}

void PlotWindowGLPane::mouseLeftReleased(wxMouseEvent& event)
{
    left_mouse_button_.setIsReleased();
    Refresh();
}

void PlotWindowGLPane::mouseMoved(wxMouseEvent& event)
{
    if (left_mouse_button_.isPressed())
    {
        const wxPoint current_point = event.GetPosition();

        if(is_editing_)
        {
            const Vec2Df mouse_pos = Vec2Df(current_point.x, current_point.y);
            const Vec2Df delta = mouse_pos - pos_at_press_;
            const Vec2Df current_pos(this->GetPosition().x, this->GetPosition().y);
            const Vec2Df changed_pos = current_pos + delta;
            const wxSize size_now = this->GetSize();

            setPosAndSize(wxPoint(changed_pos.x, changed_pos.y), size_now);
        }
        else
        {
            left_mouse_button_.updateOnMotion(current_point.x, current_point.y);
            axes_interactor_->registerMouseDragInput(left_mouse_button_.getDeltaPos().x,
                                                     left_mouse_button_.getDeltaPos().y);
        }

        Refresh();
    }
}

void PlotWindowGLPane::keyPressed(wxKeyEvent& event)
{
    const int key_code = event.GetKeyCode();

    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key_code))
    {
        keyboard_state_.keyGotPressed(key_code);
    }
    Refresh();
}

void PlotWindowGLPane::keyReleased(wxKeyEvent& event)
{
    const int key_code = event.GetKeyCode();

    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key_code))
    {
        keyboard_state_.keyGotReleased(key_code);
    }
    Refresh();
}

// Returns window width in pixels
int PlotWindowGLPane::getWidth()
{
    return GetSize().x;
}

// Returns window height in pixels
int PlotWindowGLPane::getHeight()
{
    return GetSize().y;
}

InteractionType keyboardStateToInteractionType(const KeyboardState& keyboard_state)
{
    if (keyboard_state.keyIsPressed('C'))
    {
        return InteractionType::RESET;
    }
    else if (keyboard_state.keyIsPressed('P'))
    {
        return InteractionType::PAN;
    }
    else if (keyboard_state.keyIsPressed('R'))
    {
        return InteractionType::ROTATE;
    }
    else if (keyboard_state.keyIsPressed('Z'))
    {
        return InteractionType::ZOOM;
    }
    else
    {
        return InteractionType::UNCHANGED;
    }
}

void PlotWindowGLPane::render(wxPaintEvent& evt)
{
    (void)evt;
    if (!IsShown())
        return;

    // clang-format off
    // const double m[] = {1, 0, 0, 0, 
    //                     0, 0, 1, 0, 
    //                     0, 1, 0, 0, 
    //                     0, 0, 0, 1};
    // // clang-format on
    // glLoadMatrixd(m);

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(
        this);

    glEnable(GL_MULTISAMPLE);

    const float bg_color = 190.0f;

    glClearColor(bg_color / 255.0f, bg_color / 255.0f, bg_color / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    axes_interactor_->update(
        keyboardStateToInteractionType(keyboard_state_), getWidth(), getHeight());

    axes_painter_->paint(axes_interactor_->getAxesLimits(),
                         axes_interactor_->getViewAngles(),
                         axes_interactor_->generateGridVectors(),
                         axes_interactor_->getCoordConverter());

    glEnable(GL_DEPTH_TEST);  // TODO: Put in "plotBegin" and "plotEnd"?
    axes_painter_->plotBegin();

    plot_data_handler_.visualize();

    axes_painter_->plotEnd();
    glDisable(GL_DEPTH_TEST);

    // glFlush();
    SwapBuffers();
}
