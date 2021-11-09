#include "gl_canvas.h"

#include <stdlib.h>

#include <wx/event.h>
#include <wx/glcanvas.h>

#include "axes/axes.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_low_level.h"


GlCanvas::GlCanvas(wxWindow* parent)
    : wxGLCanvas(parent, wxID_ANY, getArgsPtr(), wxPoint(0, 0), wxSize(600, 600), wxFULL_REPAINT_ON_RESIZE)
{
// #ifdef PLATFORM_APPLE_M
    wxGLContextAttrs cxtAttrs;
    // cxtAttrs.PlatformDefaults().OGLVersion(3, 2).EndList();
    cxtAttrs.PlatformDefaults().CoreProfile().OGLVersion(3, 2).EndList();
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
    shader_ = Shader::createFromFiles(v_path, f_path);

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    // glEnable(GL_MULTISAMPLE);
    // glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    /*float vertices[] = {
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f   // top 
    };

    glGenBuffers(1, &VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VAO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 3, vertices, GL_STATIC_DRAW);*/
    static const float VertexBufferData[] = {
            0.75f, 0.75f, 0.0f, 1.0f,
            0.75f, -0.75f, 0.0f, 1.0f,
            -0.75f, -0.75f, 0.0f, 1.0f,
        };

    glGenVertexArrays(1, &m_VertexBufferArray);
    glBindVertexArray(m_VertexBufferArray);

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferData), VertexBufferData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

    glBindVertexArray(0);

    glMatrixMode(GL_PROJECTION);

    axes_settings_ = AxesSettings({-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f});
    axes_interactor_ = new AxesInteractor(axes_settings_, getWidth(), getHeight());
    axes_painter_ = new AxesPainter(axes_settings_);

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

    axes_painter_->updateStates(axes_interactor_->getAxesLimits(),
                         axes_interactor_->getViewAngles(),
                         axes_interactor_->generateGridVectors(),
                         axes_interactor_->getCoordConverter());

    axes_painter_->plotBegin();
    glUseProgram(shader_.programId());

    std::array<GLfloat, 16> projection;
    std::array<GLfloat, 16> model_view;
    glGetFloatv(GL_PROJECTION_MATRIX, projection.data());
    glGetFloatv(GL_MODELVIEW_MATRIX, model_view.data());

    const Matrix<double> rot_mat = axes_interactor_->getViewAngles().getRotationMatrix();

    GLfloat modelviewmatrix[] = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    for(size_t r = 0; r < 3; r++)
    {
        for(size_t c = 0; c < 3; c++)
        {
            modelviewmatrix[r * 3 + c] = rot_mat(r, c);
        }
    }

    // GL_MODELVIEW_PROJECTION_NV
    // glm::mat4 mvp = Projection * View * Model
    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "ProjectionMatrix"), 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "ModelViewMatrix"), 1, GL_FALSE, modelviewmatrix);

    glBindVertexArray(m_VertexBufferArray);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);

    glUseProgram(0);
    axes_painter_->plotEnd();

    // glDisable(GL_DEPTH_TEST);

    glFlush();
    SwapBuffers();
}
