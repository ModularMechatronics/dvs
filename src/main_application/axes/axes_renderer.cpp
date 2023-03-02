#include "axes_renderer.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "dvs/math/math.h"
#include "misc/misc.h"

using namespace dvs;

AxesRenderer::AxesRenderer(const ShaderCollection& shader_collection,
                           const ElementSettings& element_settings,
                           const RGBTripletf& tab_background_color)
    : shader_collection_{shader_collection},
      legend_renderer_{text_renderer_, shader_collection_},
      element_settings_{element_settings},
      tab_background_color_{tab_background_color},
      plot_pane_background_{element_settings_.pane_radius}
{
    glUseProgram(shader_collection_.text_shader.programId());

    initFreetype();

    glUniform1i(glGetUniformLocation(shader_collection_.text_shader.programId(), "text_sampler"), 0);

    if (element_settings_.clipping_on)
    {
        shader_collection_.basic_plot_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.basic_plot_shader.programId(), "use_clip_plane"), 1);
        shader_collection_.img_plot_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.img_plot_shader.programId(), "use_clip_plane"), 1);
        shader_collection_.draw_mesh_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "use_clip_plane"), 1);
        shader_collection_.scatter_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "use_clip_plane"), 1);
        shader_collection_.plot_2d_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "use_clip_plane"), 1);
        shader_collection_.plot_3d_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "use_clip_plane"), 1);
    }
    else
    {
        shader_collection_.basic_plot_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.basic_plot_shader.programId(), "use_clip_plane"), 0);
        shader_collection_.img_plot_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.img_plot_shader.programId(), "use_clip_plane"), 0);
        shader_collection_.draw_mesh_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "use_clip_plane"), 0);
        shader_collection_.scatter_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "use_clip_plane"), 0);
        shader_collection_.plot_2d_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "use_clip_plane"), 0);
        shader_collection_.plot_3d_shader.use();
        glUniform1i(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "use_clip_plane"), 0);
    }

    const float sw = 3.0f;
    orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);
    persp_projection_mat = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    use_perspective_proj_ = false;
    current_mouse_interaction_axis_ = MouseInteractionAxis::ALL;

    projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    // Camera matrix
    view_mat = glm::lookAt(glm::vec3(0, -6.0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    model_mat = glm::mat4(1.0f);
    scale_mat = glm::mat4(1.0f);

    window_scale_mat_ = glm::mat4(1.0f);
    scale_vector_ = Vec3d(2.5, 2.5, 2.5);

    global_illumination_active_ = false;
}

void AxesRenderer::enableClipPlanes()
{
    if (element_settings_.clipping_on)
    {
        const double f = 0.1;

        const Vec3d axes_center = axes_limits_.getAxesCenter();

        const Vec3d scale = axes_limits_.getAxesScale() / 2.0;

        // clang-format off
        const Vector<Point3d> points_x0{VectorInitializer{Point3d(scale.x - axes_center.x, f, f),
                                        Point3d(scale.x - axes_center.x, -f, f),
                                        Point3d(scale.x - axes_center.x, f, -f)}};
        const Vector<Point3d> points_x1{VectorInitializer{(Point3d(scale.x + axes_center.x, f, f)),
                                        Point3d(scale.x + axes_center.x, -f, f),
                                        Point3d(scale.x + axes_center.x, f, -f)}};
        const Vector<Point3d> points_y0{VectorInitializer{Point3d(-f, scale.y - axes_center.y, f),
                                        Point3d(f, scale.y - axes_center.y, f),
                                        Point3d(f, scale.y - axes_center.y, -f)}};
        const Vector<Point3d> points_y1{VectorInitializer{Point3d(-f, scale.y + axes_center.y, f),
                                        Point3d(f, scale.y + axes_center.y, f),
                                        Point3d(f, scale.y + axes_center.y, -f)}};
        const Vector<Point3d> points_z0{VectorInitializer{Point3d(-f, f, -(axes_center.z + scale.z)),
                                        Point3d(f, -f, -(axes_center.z + scale.z)),
                                        Point3d(-f, -f, -(axes_center.z + scale.z))}};
        const Vector<Point3d> points_z1{VectorInitializer{Point3d(-f, f, -(scale.z - axes_center.z)),
                                        Point3d(f, -f, -(scale.z - axes_center.z)),
                                        Point3d(-f, -f, -(scale.z - axes_center.z))}};

        // TODO: Simplify
        setClipPlane(shader_collection_.basic_plot_shader.programId(), "clip_plane0", points_x0(0), points_x0(1), points_x0(2), true);
        setClipPlane(shader_collection_.basic_plot_shader.programId(), "clip_plane1", points_x1(0), points_x1(1), points_x1(2), false);
        setClipPlane(shader_collection_.basic_plot_shader.programId(), "clip_plane2", points_y0(0), points_y0(1), points_y0(2), true);
        setClipPlane(shader_collection_.basic_plot_shader.programId(), "clip_plane3", points_y1(0), points_y1(1), points_y1(2), false);
        setClipPlane(shader_collection_.basic_plot_shader.programId(), "clip_plane4", points_z0(0), points_z0(1), points_z0(2), true);
        setClipPlane(shader_collection_.basic_plot_shader.programId(), "clip_plane5", points_z1(0), points_z1(1), points_z1(2), false);

        glUseProgram(shader_collection_.img_plot_shader.programId());
        setClipPlane(shader_collection_.img_plot_shader.programId(), "clip_plane0", points_x0(0), points_x0(1), points_x0(2), true);
        setClipPlane(shader_collection_.img_plot_shader.programId(), "clip_plane1", points_x1(0), points_x1(1), points_x1(2), false);
        setClipPlane(shader_collection_.img_plot_shader.programId(), "clip_plane2", points_y0(0), points_y0(1), points_y0(2), true);
        setClipPlane(shader_collection_.img_plot_shader.programId(), "clip_plane3", points_y1(0), points_y1(1), points_y1(2), false);
        setClipPlane(shader_collection_.img_plot_shader.programId(), "clip_plane4", points_z0(0), points_z0(1), points_z0(2), true);
        setClipPlane(shader_collection_.img_plot_shader.programId(), "clip_plane5", points_z1(0), points_z1(1), points_z1(2), false);

        glUseProgram(shader_collection_.draw_mesh_shader.programId());
        setClipPlane(shader_collection_.draw_mesh_shader.programId(), "clip_plane0", points_x0(0), points_x0(1), points_x0(2), true);
        setClipPlane(shader_collection_.draw_mesh_shader.programId(), "clip_plane1", points_x1(0), points_x1(1), points_x1(2), false);
        setClipPlane(shader_collection_.draw_mesh_shader.programId(), "clip_plane2", points_y0(0), points_y0(1), points_y0(2), true);
        setClipPlane(shader_collection_.draw_mesh_shader.programId(), "clip_plane3", points_y1(0), points_y1(1), points_y1(2), false);
        setClipPlane(shader_collection_.draw_mesh_shader.programId(), "clip_plane4", points_z0(0), points_z0(1), points_z0(2), true);
        setClipPlane(shader_collection_.draw_mesh_shader.programId(), "clip_plane5", points_z1(0), points_z1(1), points_z1(2), false);

        glUseProgram(shader_collection_.scatter_shader.programId());
        setClipPlane(shader_collection_.scatter_shader.programId(), "clip_plane0", points_x0(0), points_x0(1), points_x0(2), true);
        setClipPlane(shader_collection_.scatter_shader.programId(), "clip_plane1", points_x1(0), points_x1(1), points_x1(2), false);
        setClipPlane(shader_collection_.scatter_shader.programId(), "clip_plane2", points_y0(0), points_y0(1), points_y0(2), true);
        setClipPlane(shader_collection_.scatter_shader.programId(), "clip_plane3", points_y1(0), points_y1(1), points_y1(2), false);
        setClipPlane(shader_collection_.scatter_shader.programId(), "clip_plane4", points_z0(0), points_z0(1), points_z0(2), true);
        setClipPlane(shader_collection_.scatter_shader.programId(), "clip_plane5", points_z1(0), points_z1(1), points_z1(2), false);

        glUseProgram(shader_collection_.plot_2d_shader.programId());
        setClipPlane(shader_collection_.plot_2d_shader.programId(), "clip_plane0", points_x0(0), points_x0(1), points_x0(2), true);
        setClipPlane(shader_collection_.plot_2d_shader.programId(), "clip_plane1", points_x1(0), points_x1(1), points_x1(2), false);
        setClipPlane(shader_collection_.plot_2d_shader.programId(), "clip_plane2", points_y0(0), points_y0(1), points_y0(2), true);
        setClipPlane(shader_collection_.plot_2d_shader.programId(), "clip_plane3", points_y1(0), points_y1(1), points_y1(2), false);
        setClipPlane(shader_collection_.plot_2d_shader.programId(), "clip_plane4", points_z0(0), points_z0(1), points_z0(2), true);
        setClipPlane(shader_collection_.plot_2d_shader.programId(), "clip_plane5", points_z1(0), points_z1(1), points_z1(2), false);

        glUseProgram(shader_collection_.plot_3d_shader.programId());
        setClipPlane(shader_collection_.plot_3d_shader.programId(), "clip_plane0", points_x0(0), points_x0(1), points_x0(2), true);
        setClipPlane(shader_collection_.plot_3d_shader.programId(), "clip_plane1", points_x1(0), points_x1(1), points_x1(2), false);
        setClipPlane(shader_collection_.plot_3d_shader.programId(), "clip_plane2", points_y0(0), points_y0(1), points_y0(2), true);
        setClipPlane(shader_collection_.plot_3d_shader.programId(), "clip_plane3", points_y1(0), points_y1(1), points_y1(2), false);
        setClipPlane(shader_collection_.plot_3d_shader.programId(), "clip_plane4", points_z0(0), points_z0(1), points_z0(2), true);
        setClipPlane(shader_collection_.plot_3d_shader.programId(), "clip_plane5", points_z1(0), points_z1(1), points_z1(2), false);
        // clang-format on
    }

    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

void AxesRenderer::setClipPlane(const GLuint program_id,
                                const std::string pln,
                                const Point3d& p0,
                                const Point3d& p1,
                                const Point3d& p2,
                                const bool invert) const
{
    // Fit plane
    const Planed fp = planeFromThreePoints(p0, p1, p2);

    // Invert
    const Planed plane = invert ? Planed(-fp.a, -fp.b, -fp.c, fp.d) : fp;

    glUniform4f(glGetUniformLocation(program_id, pln.c_str()), plane.a, plane.b, plane.c, plane.d);
}

void AxesRenderer::renderTitle()
{
    glUseProgram(shader_collection_.text_shader.programId());

    const GLint text_color_uniform = glGetUniformLocation(shader_collection_.text_shader.programId(), "textColor");
    glUniform3f(text_color_uniform, 0.0, 0.0, 0.0);

    const float x_coord = -1.0f + 5.0f / width_;
    const float y_coord = 1.0f - 20.0f / height_;
    text_renderer_.renderTextFromLeftCenter(name_, x_coord, y_coord, 0.0005f, width_, height_);
}

void AxesRenderer::renderInteractionLetter()
{
    glUseProgram(shader_collection_.text_shader.programId());

    const GLint text_color_uniform = glGetUniformLocation(shader_collection_.text_shader.programId(), "textColor");
    glUniform3f(text_color_uniform, 0.0, 0.0, 0.0);

    std::string interaction_string;
    if (mouse_interaction_type_ == MouseInteractionType::PAN)
    {
        interaction_string = "Pan";
    }
    else if (mouse_interaction_type_ == MouseInteractionType::ZOOM)
    {
        interaction_string = "Zoom";
    }
    else if (mouse_interaction_type_ == MouseInteractionType::ROTATE)
    {
        interaction_string = "Rotate";
        if (current_mouse_interaction_axis_ == MouseInteractionAxis::X)
        {
            interaction_string += " [azimuth]";
        }
        else if (current_mouse_interaction_axis_ == MouseInteractionAxis::Y)
        {
            interaction_string += " [elevation]";
        }
    }
    else
    {
        interaction_string = "U";
    }

    if (mouse_interaction_type_ != MouseInteractionType::ROTATE)
    {
        if (current_mouse_interaction_axis_ == MouseInteractionAxis::X)
        {
            interaction_string += " [x]";
        }
        if (current_mouse_interaction_axis_ == MouseInteractionAxis::Y)
        {
            interaction_string += " [y]";
        }
        if (current_mouse_interaction_axis_ == MouseInteractionAxis::Z)
        {
            interaction_string += " [z]";
        }
        if (current_mouse_interaction_axis_ == MouseInteractionAxis::XY)
        {
            interaction_string += " [xy]";
        }
        if (current_mouse_interaction_axis_ == MouseInteractionAxis::XZ)
        {
            interaction_string += " [xz]";
        }
        if (current_mouse_interaction_axis_ == MouseInteractionAxis::YZ)
        {
            interaction_string += " [yz]";
        }
    }

    const float y_coord = -1.0f + 17.0f / height_;
    const float x_coord = -1.0f + 10.0f / width_;
    text_renderer_.renderTextFromLeftCenter(interaction_string, x_coord, y_coord, 0.0003f, width_, height_);
}

void AxesRenderer::renderViewAngles()
{
    glUseProgram(shader_collection_.text_shader.programId());

    const GLint text_color_uniform = glGetUniformLocation(shader_collection_.text_shader.programId(), "textColor");
    glUniform3f(text_color_uniform, 0.0, 0.0, 0.0);

    const int az = static_cast<float>(view_angles_.getSnappedAzimuth() * 180.0f / M_PI);
    const int el = static_cast<float>(view_angles_.getSnappedElevation() * 180.0f / M_PI);

    const std::string view_angle_str = "[ " + std::to_string(az) + ", " + std::to_string(el) + " ]";
    const float y_coord = -1.0f + 20.0f / height_;
    const float x_coord = 1.0f - 110.0f / width_;
    text_renderer_.renderTextFromLeftCenter(view_angle_str, x_coord, y_coord, 0.0003f, width_, height_);
}

void AxesRenderer::render()
{
    renderTitle();
    renderInteractionLetter();
    if (mouse_pressed_)
    {
        renderViewAngles();
    }
    glUseProgram(shader_collection_.plot_box_shader.programId());
    renderBackground();
    if (element_settings_.plot_box_on)
    {
        renderPlotBox();
    }
    if (render_zoom_rect_)
    {
        model_mat[3][0] = 0.0;
        model_mat[3][1] = 0.0;
        model_mat[3][2] = 0.0;

        const glm::mat4 mvp = projection_mat * view_mat * model_mat * window_scale_mat_;

        glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"),
                           1,
                           GL_FALSE,
                           &mvp[0][0]);

        zoom_rect_.render(mouse_pos_at_press_,
                          current_mouse_pos_,
                          view_angles_.getSnappingAxis(),
                          view_mat,
                          model_mat * window_scale_mat_,
                          projection_mat);
    }

    if (element_settings_.grid_on)
    {
        renderBoxGrid();
    }
    if (element_settings_.plot_box_on)
    {
        // Render the silhouette after the box grid because of reasons...

        scale_mat[0][0] = 1.0;
        scale_mat[1][1] = 1.0;
        scale_mat[2][2] = 1.0;
        scale_mat[3][3] = 1.0;
        const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat * window_scale_mat_;

        glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"),
                           1,
                           GL_FALSE,
                           &mvp[0][0]);
        glUniform3f(
            glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "vertex_color"), 0.0f, 0.0f, 0.0f);
        plot_box_silhouette_.render(
            axes_side_configuration_, view_angles_.getSnappedAzimuth(), view_angles_.getSnappedElevation());
    }

    drawGridNumbers(text_renderer_,
                    shader_collection_.text_shader,
                    axes_limits_,
                    view_angles_,
                    element_settings_,
                    view_mat,
                    model_mat * window_scale_mat_,
                    projection_mat,
                    width_,
                    height_,
                    grid_vectors_,
                    axes_side_configuration_,
                    use_perspective_proj_);
}

void AxesRenderer::renderLegend()
{
    // const glm::mat4 mvp = projection_mat * view_mat;
    const glm::mat4 mvp = orth_projection_mat * view_mat;

    glUseProgram(shader_collection_.legend_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.legend_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);

    glUseProgram(shader_collection_.plot_box_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);
    legend_renderer_.render(legend_properties_, width_, height_);
}

void AxesRenderer::renderBoxGrid()
{
    const Vec3d scale = axes_limits_.getAxesScale() / 2.0;

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0 / scale.y;
    scale_mat[2][2] = 1.0 / scale.z;
    scale_mat[3][3] = 1.0;

    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat * window_scale_mat_;

    const RGBTripletf color_vec{element_settings_.grid_color};

    glUniform3f(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "vertex_color"),
                color_vec.red,
                color_vec.green,
                color_vec.blue);
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);

    plot_box_grid_.render(grid_vectors_, axes_limits_, view_angles_, axes_side_configuration_);
}

void AxesRenderer::plotBegin()
{
    const Vec3d axes_center = axes_limits_.getAxesCenter();

    const Vec3d scale = axes_limits_.getAxesScale() / 2.0;

    glm::mat4 t_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-axes_center.x, -axes_center.y, -axes_center.z));

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0 / scale.y;
    scale_mat[2][2] = 1.0 / scale.z;
    scale_mat[3][3] = 1.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat * window_scale_mat_ * t_mat;
    const glm::mat4 i_mvp = glm::inverse(mvp);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.basic_plot_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);

    glUseProgram(shader_collection_.draw_mesh_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "rotation_mat"),
                       1,
                       GL_FALSE,
                       &model_mat[0][0]);
    glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "global_illumination_active"),
                static_cast<int>(global_illumination_active_));
    glUniform3f(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "light_pos"),
                light_pos_.x,
                light_pos_.y,
                light_pos_.z);

    glUseProgram(shader_collection_.scatter_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);

    glUseProgram(shader_collection_.plot_2d_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);
    glUniformMatrix4fv(
        glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "inverse_model_view_proj_mat"),
        1,
        GL_FALSE,
        &i_mvp[0][0]);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "axes_width"), width_);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "axes_height"), height_);

    glUseProgram(shader_collection_.plot_3d_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);
    glUniformMatrix4fv(
        glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "inverse_model_view_proj_mat"),
        1,
        GL_FALSE,
        &i_mvp[0][0]);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "axes_width"), width_);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "axes_height"), height_);

    glUseProgram(shader_collection_.basic_plot_shader.programId());
    enableClipPlanes();

    glUseProgram(shader_collection_.img_plot_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.img_plot_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);

    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

void AxesRenderer::plotEnd()
{
    glUseProgram(0);
    if (render_legend_)
    {
        renderLegend();
    }
}

void AxesRenderer::activateGlobalIllumination(const Vec3d& light_pos)
{
    global_illumination_active_ = true;
    light_pos_ = light_pos;
}

void AxesRenderer::resetGlobalIllumination()
{
    global_illumination_active_ = false;
}

void AxesRenderer::renderBackground()
{
    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    scale_mat[0][0] = 1.0;
    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0;
    scale_mat[3][3] = 1.0;

    const glm::mat4 rotation_mat = glm::rotate(static_cast<float>(M_PI / 2.0), glm::vec3(1.0, 0.0, 0.0));
    const glm::mat4 mvp = projection_mat * view_mat * rotation_mat;

    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);

    const RGBTripletf color_vec{tab_background_color_};

    glUniform3f(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "vertex_color"),
                color_vec.red,
                color_vec.green,
                color_vec.blue);
    plot_pane_background_.render(width_, height_);
}

void AxesRenderer::renderPlotBox()
{
    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    scale_mat[0][0] = 1.0;
    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0;
    scale_mat[3][3] = 1.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * window_scale_mat_;

    const RGBTripletf color_vec{element_settings_.plot_box_color};

    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"),
                       1,
                       GL_FALSE,
                       &mvp[0][0]);
    glUniform3f(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "vertex_color"),
                color_vec.red,
                color_vec.green,
                color_vec.blue);

    plot_box_walls_.render(axes_side_configuration_);
}

void AxesRenderer::setAxesBoxScaleFactor(const Vec3d& scale_vector)
{
    scale_vector_ = scale_vector;
}

void AxesRenderer::updateStates(const AxesLimits& axes_limits,
                                const ViewAngles& view_angles,
                                const GridVectors& gv,
                                const AxesSideConfiguration& axes_side_configuration,
                                const bool use_perspective_proj,
                                const float width,
                                const float height,
                                const Vec2f mouse_pos_at_press,
                                const Vec2f current_mouse_pos,
                                const MouseInteractionType mouse_interaction_type,
                                const bool mouse_pressed,
                                const bool render_zoom_rect,
                                const bool render_legend,
                                const float legend_scale_factor,
                                const std::vector<LegendProperties>& legend_properties,
                                const std::string& name,
                                const MouseInteractionAxis current_mouse_interaction_axis)
{
    axes_limits_ = axes_limits;
    view_angles_ = view_angles;
    grid_vectors_ = gv;
    use_perspective_proj_ = use_perspective_proj;
    width_ = width;
    height_ = height;
    mouse_pos_at_press_ = mouse_pos_at_press;
    current_mouse_pos_ = current_mouse_pos;
    mouse_interaction_type_ = mouse_interaction_type;
    mouse_pressed_ = mouse_pressed;
    render_zoom_rect_ = render_zoom_rect;
    render_legend_ = render_legend;
    legend_properties_ = legend_properties;
    legend_renderer_.setLegendScaleFactor(legend_scale_factor);
    name_ = name;
    current_mouse_interaction_axis_ = current_mouse_interaction_axis;
    axes_side_configuration_ = axes_side_configuration;

    rot_mat = rotationMatrixZ(-view_angles_.getSnappedAzimuth()) * rotationMatrixX(-view_angles_.getSnappedElevation());

    for (int r = 0; r < 3; r++)
    {
        for (int c = 0; c < 3; c++)
        {
            model_mat[r][c] = rot_mat(r, c);
        }
    }

    projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    float az;
    float el;
    float s = 0.0f;

    Vec3d scale_vector_to_use;

    if (use_perspective_proj_)
    {
        az = 0.2;
        el = 0.0;
        s = az + el;
        scale_vector_to_use = scale_vector_;
    }
    else
    {
        scale_vector_to_use = scale_vector_;
        az = std::pow(std::fabs(std::sin(view_angles_.getSnappedAzimuth() * 2.0f)), 0.6) * 0.7;
        el = std::pow(std::fabs(std::sin(view_angles_.getSnappedElevation() * 2.0f)), 0.7) * 0.5;
        s = std::sqrt(az * az + el * el);
    }

    window_scale_mat_[0][0] = scale_vector_to_use.x - s;
    window_scale_mat_[1][1] = scale_vector_to_use.y - s;
    window_scale_mat_[2][2] = scale_vector_to_use.z - s;
}
