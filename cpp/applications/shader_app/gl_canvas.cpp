#include "gl_canvas.h"

#include <stdlib.h>

#include <wx/event.h>
#include <wx/glcanvas.h>

#include "axes/axes.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_low_level.h"

GlCanvas::~GlCanvas()
{

}

GlCanvas::GlCanvas(wxWindow* parent)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), wxPoint(0, 0), wxSize(600, 600), wxFULL_REPAINT_ON_RESIZE)
{
// #ifdef PLATFORM_APPLE_M
    wxGLContextAttrs cxtAttrs;
    cxtAttrs.PlatformDefaults().CompatibilityProfile().CoreProfile().OGLVersion(3, 3).EndList();
    // cxtAttrs.PlatformDefaults().EndList();
    // https://stackoverflow.com/questions/41145024/wxwidgets-and-modern-opengl-3-3
    m_context = new wxGLContext(this, NULL, &cxtAttrs);
// #endif

#ifdef PLATFORM_LINUX_M
    // m_context = new wxGLContext(this);
#endif
    if ( !m_context->IsOK() )
    {
        std::cout << "Not ok!!" << std::endl;
    }

    wxGLCanvas::SetCurrent(*m_context);

    const std::string v_path = "../applications/shader_app/shaders/basic.vertex";
    const std::string f_path = "../applications/shader_app/shaders/basic.fragment";
    shader_ = Shader::createFromFiles(v_path,f_path);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    glEnable(GL_MULTISAMPLE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    Bind(wxEVT_PAINT, &GlCanvas::render, this);
    Bind(wxEVT_MOTION, &GlCanvas::mouseMoved, this);
    Bind(wxEVT_LEFT_DOWN, &GlCanvas::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &GlCanvas::mouseLeftReleased, this);
    Bind(wxEVT_KEY_DOWN, &GlCanvas::keyPressed, this);
    Bind(wxEVT_KEY_UP, &GlCanvas::keyReleased, this);

    axes_settings_ = AxesSettings({-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f});
    axes_interactor_ = new AxesInteractor(axes_settings_, getWidth(), getHeight());
    axes_painter_ = new AxesPainter(axes_settings_);
}

void GlCanvas::mouseMoved(wxMouseEvent& event)
{
    const wxPoint current_point = event.GetPosition();

    if (left_mouse_button_.isPressed())
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
        axes_interactor_->registerMouseDragInput(current_mouse_interaction_axis_,
                                                    left_mouse_button_.getDeltaPos().x,
                                                    left_mouse_button_.getDeltaPos().y);

        Refresh();
    }
}

void GlCanvas::mouseLeftPressed(wxMouseEvent& event)
{
    const wxPoint current_point = event.GetPosition();

    left_mouse_button_.setIsPressed(current_point.x, current_point.y);
    Refresh();
}

void GlCanvas::mouseLeftReleased(wxMouseEvent& WXUNUSED(event))
{
    left_mouse_button_.setIsReleased();
    Refresh();
}

void GlCanvas::keyPressed(wxKeyEvent& event)
{
    int key_code = event.GetKeyCode();

    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key_code))
    {
        keyboard_state_.keyGotReleased(key_code);
    }
    Refresh();
}

void GlCanvas::keyReleased(wxKeyEvent& event)
{
    int key_code = event.GetKeyCode();

    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key_code))
    {
        keyboard_state_.keyGotReleased(key_code);
    }

    Refresh();
}


void GlCanvas::show()
{
    this->Show();
}

InteractionType keyboardStateToInteractionTypeNew(const KeyboardState& keyboard_state)
{
    if (keyboard_state.keyIsPressed('c'))
    {
        return InteractionType::RESET;
    }
    else if (keyboard_state.keyIsPressed('p') || keyboard_state.keyIsPressed('q'))
    {
        return InteractionType::PAN;
    }
    else if (keyboard_state.keyIsPressed('r') || keyboard_state.keyIsPressed('w'))
    {
        return InteractionType::ROTATE;
    }
    else if (keyboard_state.keyIsPressed('z') || keyboard_state.keyIsPressed('e'))
    {
        return InteractionType::ZOOM;
    }
    else
    {
        return InteractionType::UNCHANGED;
    }
}

void GlCanvas::render(wxPaintEvent& evt)
{
    (void)evt;
    if (!IsShown())
    {
        return;
    }

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);

    glEnable(GL_MULTISAMPLE);

    const float bg_color = 240.0f;

    glClearColor(bg_color / 255.0f, bg_color / 255.0f, bg_color / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    axes_interactor_->update(
        keyboardStateToInteractionTypeNew(keyboard_state_), getWidth(), getHeight());

    axes_painter_->updateStates(axes_interactor_->getAxesLimits(),
                         axes_interactor_->getViewAngles(),
                         axes_interactor_->generateGridVectors(),
                         axes_interactor_->getCoordConverter());
    glUseProgram(shader_.programId());

    glEnable(GL_DEPTH_TEST);
    // axes_painter_->plotBegin();

    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex3f(0.0f, 0.0f, 0.0f);
    glVertex3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 1.0f, 0.0f);
    glEnd();

    // axes_painter_->plotEnd();
    glUseProgram(0);

    glDisable(GL_DEPTH_TEST);

    glFlush();
    SwapBuffers();
}

// Returns window width in pixels
int GlCanvas::getWidth()
{
    return GetSize().x;
}

// Returns window height in pixels
int GlCanvas::getHeight()
{
    return GetSize().y;
}
