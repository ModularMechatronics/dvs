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

    use_perspective_proj_ = true;

    wxGLCanvas::SetCurrent(*m_context);

    const std::string v_path = "../applications/shader_app/shaders/basic.vertex";
    const std::string f_path = "../applications/shader_app/shaders/basic.fragment";
    shader_ = Shader::createFromFiles(v_path, f_path);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    // glEnable(GL_MULTISAMPLE);
    // glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    cube_ = VboWrapper3D(sizeof(vertex_color) / (sizeof(vertex_color[0]) * 3), vertex_data, vertex_color);

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
    /*axes_renderer_->updateStates(axes_interactor_->getAxesLimits(),
                                 axes_interactor_->getViewAngles(),
                                 axes_interactor_->generateGridVectors(),
                                 axes_interactor_->getCoordConverter());*/

    glUseProgram(shader_.programId());

    // Angles
    const ViewAngles va = axes_interactor_->getViewAngles();
    const Matrix<double> rot_mat = rotationMatrixZ(-va.getAzimuth()) * rotationMatrixZ(static_cast<double>(M_PI)) *  
                                   rotationMatrixX(va.getElevation()) *
                                   rotationMatrixX(static_cast<double>(M_PI) / 2.0f);

    // AxesLimits
    const AxesLimits axes_limits = axes_interactor_->getAxesLimits();
    const Vec3Dd axes_center = axes_limits.getAxesCenter();
    // axes_settings_ = axes_interactor_->getAxesSettings();
    axes_renderer_->updateStates(axes_interactor_->getAxesLimits(),
                                 axes_interactor_->getViewAngles(),
                                 axes_interactor_->generateGridVectors(),
                                 axes_interactor_->getCoordConverter());

    // Scales
    // Vec3Dd sq = axes_settings_.getAxesScale();
    const Vec3Dd s = axes_limits.getAxesScale();

    /*
    glScaled(sq.x, sq.y, sq.z);
    glRotated(ax_ang.phi * 180.0f / M_PI, ax_ang.x, ax_ang.y, ax_ang.z);
    // Not sure why y axis should be negated... But it works like this.
    glScaled(1.0 / s.x, -1.0 / s.y, 1.0 / s.z);
    glTranslated(-axes_center.x, -axes_center.y, -axes_center.z);
    */

    const float sw = 3.0f;
    glm::mat4 orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);;
    glm::mat4 persp_projection_mat = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    glm::mat4 projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    // Camera matrix
    glm::mat4 view_mat = glm::lookAt(glm::vec3(0, 0, -5.9),
                                 glm::vec3(0, 0, 0),
                                 glm::vec3(0, 1, 0));
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 scale_mat = glm::mat4(0.1);

    model_mat[3][0] = axes_center.x;
    model_mat[3][1] = axes_center.y;
    model_mat[3][2] = axes_center.z;

    scale_mat[0][0] = s.x;
    scale_mat[1][1] = s.y;
    scale_mat[2][2] = s.z;
    scale_mat[3][3] = 1.0;

    for(int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            model_mat[r][c] = rot_mat(r, c);
        }
    }

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat;

    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);
    // plot_box_walls_->render(va.getAzimuth(), va.getElevation());
    // plot_box_silhouette_->render();
    // cube_.render();
    axes_renderer_->render();

    glUseProgram(0);

    // glDisable(GL_DEPTH_TEST);

    glFlush();
    SwapBuffers();
}
