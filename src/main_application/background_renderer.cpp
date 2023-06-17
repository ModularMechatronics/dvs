#include "background_renderer.h"

#include <wx/event.h>
#include <wx/glcanvas.h>

wxGLContext* BackgroundRenderer::getContext()
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

template <typename T> T createShader(const std::string& base_path, const std::string& shader_name)
{
    const std::string v_path = base_path + shader_name + ".vs";
    const std::string f_path = base_path + shader_name + ".fs";

    return T{v_path, f_path, ShaderSource::FILE};
}

wxGLAttributes BackgroundRenderer::getGLAttributes() const
{
    wxGLAttributes attrs;
    attrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).Samplers(4).SampleBuffers(1).EndList();

    if (!wxGLCanvas::IsDisplaySupported(attrs))
    {
        attrs.Reset();
        attrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();

        if (!wxGLCanvas::IsDisplaySupported(attrs))
        {
            attrs.Reset();
            attrs.PlatformDefaults().Defaults().EndList();

            if (!wxGLCanvas::IsDisplaySupported(attrs))
            {
                throw std::runtime_error("Unable to set acceptable wxGLAttributes!");
            }
        }
    }

    return attrs;
}

void BackgroundRenderer::initShaders()
{
    const std::string base_path{"../main_application/axes/shaders/"};

    std::cout << "Init shaders in BackgroundRenderer" << std::endl;
    // shader_collection_.plot_box_shader = createShader<ShaderBase>(base_path, "plot_box_shader");
    // shader_collection_.pane_background_shader = createShader<ShaderBase>(base_path, "pane_background");
    shader_collection_.window_background_shader = createShader<ShaderBase>(base_path, "window_background");
    /*shader_collection_.text_shader = createShader<TextShader>(base_path, "text");
    shader_collection_.basic_plot_shader = createShader<ShaderBase>(base_path, "basic_plot_shader");
    shader_collection_.plot_2d_shader = createShader<Plot2DShader>(base_path, "plot_2d_shader");
    shader_collection_.plot_3d_shader = createShader<Plot3DShader>(base_path, "plot_3d_shader");
    shader_collection_.img_plot_shader = createShader<ImShowShader>(base_path, "img");
    shader_collection_.scatter_shader = createShader<ScatterShader>(base_path, "scatter_shader");
    shader_collection_.draw_mesh_shader = createShader<DrawMeshShader>(base_path, "draw_mesh_shader");
    shader_collection_.legend_shader = createShader<ShaderBase>(base_path, "legend_shader");*/
    std::cout << "Init shaders finished" << std::endl;
}

BackgroundRenderer::BackgroundRenderer(wxFrame* parent, const wxSize panel_size, const RGBTripletf& background_color)
    : wxGLCanvas(parent, getGLAttributes()), context_(getContext()), background_color_(background_color)
{
    data_array_ = new float[3U * 2U * 3U];

    const float edge_val = 3.0;
    data_array_[0] = -edge_val;
    data_array_[1] = -edge_val;

    data_array_[2] = edge_val;
    data_array_[3] = -edge_val;

    data_array_[4] = -edge_val;
    data_array_[5] = edge_val;

    data_array_[6] = edge_val;
    data_array_[7] = -edge_val;

    data_array_[8] = edge_val;
    data_array_[9] = edge_val;

    data_array_[10] = -edge_val;
    data_array_[11] = edge_val;

    const std::size_t num_bytes_ = 3 * 2 * 3 * sizeof(float);

    wxGLCanvas::SetCurrent(*context_);
    initShaders();

    shader_collection_.window_background_shader.use();

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes_, data_array_, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    this->SetPosition(wxPoint(0, 10));
    //this->SetSize(panel_size);

    // glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]); 

    /*std::cout << glGetUniformLocation(shader_collection_.window_background_shader.programId(), "tmp_name") << std::endl;

    // element_position0
    // element_dimension0
    for (size_t k = 0; k < 10; k++)
    {
        const std::string name = "element_position" + std::to_string(k);

        const GLint pos_handle = glGetUniformLocation(shader_collection_.window_background_shader.programId(), name.c_str());
        std::cout << name << ", " << pos_handle << std::endl;
        position_handles_.push_back(pos_handle);
        // dimension_handle_.push_back()
    }*/

    Bind(wxEVT_PAINT, &BackgroundRenderer::render, this);
    Lower();
}

BackgroundRenderer::~BackgroundRenderer()
{
    delete context_;
}

void BackgroundRenderer::updateSize()
{
    wxSize parent_size = GetParent()->GetSize();
    SetSize(parent_size);
}

void BackgroundRenderer::setTabContent(const TabSettings& tab_settings)
{
    for (const auto& elem : tab_settings.elements)
    {
        // elem.height * 2.0f
        // elem.width * 2.0f
        // elem.x * 2.0f - 1.0f
        // -(elem.y * 2.0f - 1.0f)
    }
}

void BackgroundRenderer::render(wxPaintEvent& evt)
{
    if (!IsShown())
    {
        return;
    }

    wxGLCanvas::SetCurrent(*context_);
    wxPaintDC(this);  // TODO: Can be removed?

    glEnable(GL_MULTISAMPLE);

    glClearColor(background_color_.red, background_color_.green, background_color_.blue, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render
    shader_collection_.window_background_shader.use();
    // shader_collection_.window_background_shader.base_uniform_handles.axes_width.setFloat(width_);
    // shader_collection_.window_background_shader.base_uniform_handles.axes_height.setFloat(height_);
    // shader_collection_.window_background_shader.base_uniform_handles.radius.setFloat(element_settings_.pane_radius);

    const float sw = 3.0f;
    const glm::mat4 orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);
    const glm::mat4 view_mat = glm::lookAt(glm::vec3(0, -6.0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));

    const glm::mat4 rotation_mat = glm::rotate(static_cast<float>(M_PI / 2.0), glm::vec3(1.0, 0.0, 0.0));
    const glm::mat4 mvp = orth_projection_mat * view_mat * rotation_mat;

    shader_collection_.window_background_shader.base_uniform_handles.model_view_proj_mat.setMat4x4(mvp);

    // shader_collection_.window_background_shader.base_uniform_handles.vertex_color.setColor(tab_background_color_);

    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, 6U);
    glBindVertexArray(0);
    std::cout << "Rendier" << std::endl;

    // glFlush();
    SwapBuffers();
}