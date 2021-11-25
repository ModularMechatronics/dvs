#include "gl_canvas.h"

#include <stdlib.h>

#include <wx/event.h>
#include <wx/glcanvas.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "axes_renderer.h"
#include "axes/axes.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_low_level.h"

#include "vertex_data.h"

#define vertex_data cube_vertices
#define vertex_color cube_color


GlCanvas::GlCanvas(wxWindow* parent)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), wxPoint(0, 0), wxSize(600, 600), wxFULL_REPAINT_ON_RESIZE)
{
#ifdef PLATFORM_APPLE_M
    wxGLContextAttrs cxtAttrs;
    cxtAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 2).EndList();
    // https://stackoverflow.com/questions/41145024/wxwidgets-and-modern-opengl-3-3
    m_context = new wxGLContext(this, NULL, &cxtAttrs);
#endif

#ifdef PLATFORM_LINUX_M
    // m_context = new wxGLContext(this);
#endif
    if ( !m_context->IsOK() )
    {
        std::cout << "Not ok!!" << std::endl;
    }

    use_perspective_proj_ = false;

    wxGLCanvas::SetCurrent(*m_context);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    // glEnable(GL_MULTISAMPLE);
    // glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // cube_ = VboWrapper3D(sizeof(vertex_color) / (sizeof(vertex_color[0]) * 3), vertex_data, vertex_color);

    glMatrixMode(GL_MODELVIEW);

    axes_interactor_ = new AxesInteractor(AxesSettings({-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}), getWidth(), getHeight());
    axes_renderer_ = new AxesRenderer(axes_settings_);

    Bind(wxEVT_PAINT, &GlCanvas::render, this);
    Bind(wxEVT_MOTION, &GlCanvas::mouseMoved, this);
    Bind(wxEVT_LEFT_DOWN, &GlCanvas::mouseLeftPressed, this);
    Bind(wxEVT_LEFT_UP, &GlCanvas::mouseLeftReleased, this);
    Bind(wxEVT_KEY_DOWN, &GlCanvas::keyPressed, this);
    Bind(wxEVT_KEY_UP, &GlCanvas::keyReleased, this);

}

void GlCanvas::render(wxPaintEvent& evt)
{
    (void)evt;
    if (!IsShown())
    {
        return;
    }

    // SO thread that made it work
    // https://stackoverflow.com/questions/26378289/osx-opengl-4-1-glenablevertexattribarray-gldrawarrays-gl-invalid-operation

    std::cout << "render" << std::endl;

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);

    // glEnable(GL_MULTISAMPLE);

    const float bg_color = 240.0f;
    glClearColor(bg_color / 255.0f, bg_color / 255.0f, bg_color / 255.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    axes_interactor_->update(
        keyboardStateToInteractionTypeNew(keyboard_state_), getWidth(), getHeight());

    axes_renderer_->updateStates(axes_interactor_->getAxesLimits(),
                                 axes_interactor_->getViewAngles(),
                                 axes_interactor_->generateGridVectors(),
                                 axes_interactor_->getCoordConverter(),
                                 use_perspective_proj_);    
    axes_renderer_->render();

    // glDisable(GL_DEPTH_TEST);

    glFlush();
    SwapBuffers();
}
