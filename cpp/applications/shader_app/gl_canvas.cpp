#include "gl_canvas.h"

#include <stdlib.h>

#include <wx/event.h>
#include <wx/glcanvas.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    float VertexBufferData[] = {
         -1.0f,-1.0f,-1.0f, // triangle 1 : begin
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // triangle 1 : end
        1.0f, 1.0f,-1.0f, // triangle 2 : begin
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // triangle 2 : end
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };
    for(size_t k = 0; k < (sizeof(VertexBufferData)/sizeof(float)); k++)
    {
        // VertexBufferData[k] = VertexBufferData[k] / 5.0f;
    }

    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f};

    glGenVertexArrays(1, &m_VertexBufferArray);
    glBindVertexArray(m_VertexBufferArray);

    glGenBuffers(1, &m_VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferData), VertexBufferData, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // glBindVertexArray(0); // Needed?

    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );

    glMatrixMode(GL_MODELVIEW);

    axes_interactor_ = new AxesInteractor(AxesSettings({-1.0f, -1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}), getWidth(), getHeight());
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

    // std::cout << "render" << std::endl;

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

    // axes_painter_->plotBegin();
    glUseProgram(shader_.programId());

    std::array<GLfloat, 16> projection;
    std::array<GLfloat, 16> model_view;
    glGetFloatv(GL_PROJECTION_MATRIX, projection.data());
    glGetFloatv(GL_MODELVIEW_MATRIX, model_view.data());

    // Angles
    const ViewAngles va = axes_interactor_->getViewAngles();
    const Matrix<double> rot_mat = rotationMatrixY(-va.getAzimuth()) * rotationMatrixX(va.getElevation());

    // AxesLimits
    const AxesLimits axes_limits = axes_interactor_->getAxesLimits();
    const Vec3Dd axes_center = axes_limits.getAxesCenter();
    // axes_settings_ = axes_interactor_->getAxesSettings();
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

    Matrix<float> mat;
    // Projection matrix : 45° Field of View, 1.0 ratio, display range : 0.1 unit <-> 100 units
    // glm::mat4 Projection = glm::ortho(0.0f, 800.f, 0.0f, 800.f, -10.0f, 10.0f);
    glm::mat4 Projection = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View = glm::lookAt(glm::vec3(0, 0, -5.9),
                                 glm::vec3(0, 0, 0),
                                 glm::vec3(0, 1, 0));
    glm::mat4 Model = glm::mat4(1.0f);
    glm::mat4 scale_mat = glm::mat4(0.1);

    mat.setInternalData(&Projection[0][0], 4, 4);
    // std::cout << mat << std::endl;
    mat.setInternalData(nullptr, 0, 0);

    Model[3][0] = axes_center.x;
    Model[3][1] = axes_center.y;
    Model[3][2] = axes_center.z;

    scale_mat[0][0] = s.x;
    scale_mat[1][1] = s.y;
    scale_mat[2][2] = s.z;
    scale_mat[3][3] = 1.0;

    for(int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            Model[r][c] = rot_mat(r, c);
            // std::cout << scale_mat[r][c] << std::endl;
        }
    }

    // Our ModelViewProjection : multiplication of our 3 matrices
    glm::mat4 MVP = Projection * View * Model;

    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "ProjectionMatrix"), 1, GL_FALSE, projection.data());
    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "ModelViewMatrix"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "scale_matrix"), 1, GL_FALSE, &scale_mat[0][0]);
    // glUniform3f(glGetUniformLocation(shader_.programId(), "translation"), 1, GL_FALSE, &translation[0]);

    glBindVertexArray(m_VertexBufferArray);

    glDrawArrays(GL_TRIANGLES, 0, 3 * 12);

    glBindVertexArray(0);

    glUseProgram(0);
    // axes_painter_->plotEnd();

    // glDisable(GL_DEPTH_TEST);

    glFlush();
    SwapBuffers();
}
