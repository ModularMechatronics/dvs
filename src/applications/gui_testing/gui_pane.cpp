#include "gui_pane.h"

#include <wx/event.h>
#include <wx/glcanvas.h>

using namespace duoplot::internal;

wxGLAttributes GuiPane::getGLAttributes() const
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

wxGLContext* GuiPane::getContext()
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

void GuiPane::initShaders()
{
    const std::string base_path{"/Users/danielpi/work/dvs/src/resources/shaders/"};

    shader_collection_.plot_box_shader = createShader<ShaderBase>(base_path, "plot_box_shader");
    shader_collection_.pane_background_shader = createShader<ShaderBase>(base_path, "pane_background");
    shader_collection_.text_shader = createShader<TextShader>(base_path, "text");
    shader_collection_.basic_plot_shader = createShader<ShaderBase>(base_path, "basic_plot_shader");
    shader_collection_.plot_2d_shader = createShader<Plot2DShader>(base_path, "plot_2d_shader");
    shader_collection_.plot_3d_shader = createShader<Plot3DShader>(base_path, "plot_3d_shader");
    shader_collection_.img_plot_shader = createShader<ImShowShader>(base_path, "img");
    shader_collection_.scatter_shader = createShader<ScatterShader>(base_path, "scatter_shader");
    shader_collection_.draw_mesh_shader = createShader<DrawMeshShader>(base_path, "draw_mesh_shader");
    shader_collection_.legend_shader = createShader<ShaderBase>(base_path, "legend_shader");
    shader_collection_.simple_shader = createShader<SimpleShader>(base_path, "simple_shader");
}

GuiPane::GuiPane(wxFrame* parent)
    : wxGLCanvas(parent, getGLAttributes()),
      m_context(getContext())
{
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);

    Show();
    SetSize(wxSize(300, 300));

    wxGLCanvas::SetCurrent(*m_context);
    initShaders();
    glClearColor(1.0f, 0.0f, 0.0f, 0.0f);

    /*Bind(wxEVT_RIGHT_DOWN, &ApplicationGuiElement::mouseRightPressed, this);
    Bind(wxEVT_MIDDLE_DOWN, &GuiPane::mouseMiddlePressed, this);

    Bind(wxEVT_MIDDLE_UP, &GuiPane::mouseMiddleReleased, this);
    Bind(wxEVT_RIGHT_UP, &ApplicationGuiElement::mouseRightReleased, this);*/

    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

    /*Bind(wxEVT_LEFT_DOWN, &ApplicationGuiElement::mouseLeftPressed, this);
    Bind(wxEVT_MOTION, &ApplicationGuiElement::mouseMovedOverItem, this);
    Bind(wxEVT_LEFT_UP, &ApplicationGuiElement::mouseLeftReleased, this);

    Bind(wxEVT_KEY_DOWN, &ApplicationGuiElement::keyPressedCallback, this);
    Bind(wxEVT_KEY_UP, &ApplicationGuiElement::keyReleasedCallback, this);*/
    Bind(wxEVT_PAINT, &GuiPane::render, this);
    // Bind(wxEVT_ENTER_WINDOW, &ApplicationGuiElement::mouseEnteredElement, this);
    // Bind(wxEVT_LEAVE_WINDOW, &ApplicationGuiElement::mouseLeftElement, this);
    shader_collection_.basic_plot_shader.use();
    shader_collection_.basic_plot_shader.base_uniform_handles.use_clip_plane.setInt(0);

    const float sw = 3.0f;
    orth_projection_mat_ = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);
    persp_projection_mat_ = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    projection_mat_ = persp_projection_mat_;

    // Camera matrix
    view_mat_ = glm::lookAt(glm::vec3(0, -6.0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    model_mat_ = glm::mat4(1.0f);
    scale_mat_ = glm::mat4(1.0f);

    window_scale_mat_ = glm::mat4(1.0f);
    scale_vector_ = Vec3d(2.5, 2.5, 2.5);

    orth_scale_vector_ = Vec3d(2.5, 2.5, 2.5);
    persp_scale_vector_ = Vec3d(2.5, 2.5, 2.5);

    gui_element_ = new GuiElement(0.0f, 0.0f, 0.2f, 0.3f);

    receive_timer_.Bind(wxEVT_TIMER, &GuiPane::TimerFunc, this);
    receive_timer_.Start(20);
}

void GuiPane::TimerFunc(wxTimerEvent&)
{
    Refresh();
}

GuiPane::~GuiPane()
{
    delete m_context;
}

void GuiPane::refresh()
{
    Refresh();
}

void GuiPane::render(wxPaintEvent& WXUNUSED(evt))
{
    if (!IsShown())
    {
        return;
    }

    std::cout << "Render" << std::endl;

    wxGLCanvas::SetCurrent(*m_context);
    wxPaintDC(this);  // TODO: Can be removed?

    glClearColor(0.0, 0.6, 0.5, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_collection_.simple_shader.use();
    shader_collection_.simple_shader.base_uniform_handles.vertex_color.setColor(RGBTripletf(0.6f, 0.5f, 0.0f));

    gui_element_->render();

    /*processActionQueue();

    glEnable(GL_MULTISAMPLE);

    const RGBTripletf color_vec{plot_pane_settings_->background_color};
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

    const Vec2f mouse_pos_at_press(mouse_pos_at_press_.x, mouse_pos_at_press_.y);
    const Vec2f current_mouse_pos(current_mouse_pos_.x, current_mouse_pos_.y);

    // axes_renderer_->render();
    glEnable(GL_DEPTH_TEST);  // TODO: Put in "plotBegin" and "plotEnd"?
    axes_renderer_->plotBegin();

    plot_data_handler_->render();

    axes_renderer_->plotEnd();

    if (axes_interactor_.getQueryPoint().has_query_point)
    {
        bool has_closest_point = false;

        std::tie(closest_point_, has_closest_point) = point_selection_.getClosestPoint(
            axes_renderer_->getLine(), axes_interactor_.getQueryPoint().has_query_point);

        if (has_closest_point)
        {
            should_render_point_selection_ = true;
        }
    }

    if (should_render_point_selection_)
    {
        axes_renderer_->renderPointSelection(closest_point_);
    }

    glDisable(GL_DEPTH_TEST);*/

    // glFlush();
    SwapBuffers();
}
