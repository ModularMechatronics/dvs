#include "plot_window_gl_pane.h"


#include <wx/event.h>

#include "math/math.h"
#include "axes/axes.h"
#include "io_devices/io_devices.h"
// #include "plot_data.h"
#include "opengl_low_level/opengl_low_level.h"
// #include "plot_functions/plot_functions.h"

#include "enumerations.h"

using namespace dvs::internal;

BEGIN_EVENT_TABLE(PlotWindowGLPane, wxGLCanvas)
EVT_LEFT_DOWN(PlotWindowGLPane::mouseLeftPressed)
EVT_LEFT_UP(PlotWindowGLPane::mouseLeftReleased)
EVT_SIZE(PlotWindowGLPane::resized)
EVT_KEY_DOWN(PlotWindowGLPane::keyPressed)
EVT_KEY_UP(PlotWindowGLPane::keyReleased)
EVT_PAINT(PlotWindowGLPane::render)
END_EVENT_TABLE()

PlotWindowGLPane::PlotWindowGLPane(wxNotebookPage* parent, const wxPoint& position, const wxSize& size)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    const float min_x = -1.0;
    const float max_x = 1.0;
    const float min_y = -1.0;
    const float max_y = 1.0;
    const float min_z = -1.0;
    const float max_z = 1.0;

    const AxesSettings axes_settings({min_x, min_y, min_z}, {max_x, max_y, max_z});

    Bind(wxEVT_MOTION, &PlotWindowGLPane::mouseMoved, this);

    axes_interactor_ = new AxesInteractor(axes_settings);
    axes_painter_ = new AxesPainter(axes_settings);

    hold_on_ = false;
    axes_set_ = false;

    glEnable(GL_MULTISAMPLE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}

PlotWindowGLPane::PlotWindowGLPane(wxPanel* parent, const wxPoint& position, const wxSize& size)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    const float min_x = -1.0;
    const float max_x = 1.0;
    const float min_y = -1.0;
    const float max_y = 1.0;
    const float min_z = -1.0;
    const float max_z = 1.0;

    const AxesSettings axes_settings({min_x, min_y, min_z}, {max_x, max_y, max_z});

    Bind(wxEVT_MOTION, &PlotWindowGLPane::mouseMoved, this);

    axes_interactor_ = new AxesInteractor(axes_settings);
    axes_painter_ = new AxesPainter(axes_settings);

    hold_on_ = false;
    axes_set_ = false;

    glEnable(GL_MULTISAMPLE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}

PlotWindowGLPane::PlotWindowGLPane(wxFrame* parent, const wxPoint& position, const wxSize& size)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), position, size, wxFULL_REPAINT_ON_RESIZE)
{
    m_context = new wxGLContext(this);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    const float min_x = -1.0;
    const float max_x = 1.0;
    const float min_y = -1.0;
    const float max_y = 1.0;
    const float min_z = -1.0;
    const float max_z = 1.0;

    const AxesSettings axes_settings({min_x, min_y, min_z}, {max_x, max_y, max_z});

    Bind(wxEVT_MOTION, &PlotWindowGLPane::mouseMoved, this);

    axes_interactor_ = new AxesInteractor(axes_settings);
    axes_painter_ = new AxesPainter(axes_settings);

    hold_on_ = false;
    axes_set_ = false;

    glEnable(GL_MULTISAMPLE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
}

void PlotWindowGLPane::setPosAndSize(const wxPoint pos, const wxSize size)
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

    left_mouse_button_.setIsPressed(current_point.x, current_point.y);
    Refresh();
}

void PlotWindowGLPane::mouseLeftReleased(wxMouseEvent& event)
{
    (void)event;

    left_mouse_button_.setIsReleased();
    Refresh();
}

void PlotWindowGLPane::mouseMoved(wxMouseEvent& event)
{
    if (left_mouse_button_.isPressed())
    {
        const wxPoint current_point = event.GetPosition();
        left_mouse_button_.updateOnMotion(current_point.x, current_point.y);
        axes_interactor_->registerMouseDragInput(left_mouse_button_.getDeltaPos().x,
                                                 left_mouse_button_.getDeltaPos().y);

        Refresh();
    }
}

void PlotWindowGLPane::keyPressed(wxKeyEvent& event)
{
    const int key_code = event.GetKeyCode();

    /*if(key_code == 68)
    {
        wxSize s = this->GetSize();
        wxPoint p = this->GetPosition();
        p.x = p.x + 1;
        
        s.DecBy(1, 1);
        this->SetPosition(p);
        this->SetSize(s);
        std::cout << p.x << std::endl;
    }*/

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

void PlotWindowGLPane::resized(wxSizeEvent& evt)
{
    this->SetSize(evt.GetSize());
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
