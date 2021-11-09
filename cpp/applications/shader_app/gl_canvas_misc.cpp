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

    if(key_code == 82)
    {
        reloadShader();
    }

    // Only add alpha numeric keys due to errors when clicking outside of window
    if (std::isalnum(key_code))
    {
        keyboard_state_.keyGotReleased(key_code);
    }
    Refresh();
}

void GlCanvas::reloadShader()
{
    std::cout << "Reloading shader..." << std::endl;
    glDeleteShader(shader_.programId());

    const std::string v_path = "../applications/shader_app/shaders/basic.vertex";
    const std::string f_path = "../applications/shader_app/shaders/basic.fragment";
    shader_ = Shader::createFromFiles(v_path,f_path);
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

std::string getGLErrorString(const GLenum err)
{
    switch(err)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
        break;
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
        break;
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
        break;
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    default:
        return "default";
    }
}

void printGlStuff()
{
    char *GL_version=(char *)glGetString(GL_VERSION);
    char *GL_vendor=(char *)glGetString(GL_VENDOR);
    char *GL_renderer=(char *)glGetString(GL_RENDERER);

    printf("Version: %s\n", GL_version);
    printf("Vendor: %s\n", GL_vendor);
    printf("Renderer: %s\n", GL_renderer);
}