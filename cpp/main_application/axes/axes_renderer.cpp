#include "axes_renderer.h"

#include "dvs/math/math.h"
#include "misc/misc.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using namespace dvs;

static Vec3Dd findScale(const Matrixd& R)
{
    // Currently unknown exactly how 'q' affects the results...
    const double q = 0.5;
    // clang-format off
    const Vector<Point3Dd> points = {{q, q, q},
                                     {-q, q, q},
                                     {q, -q, q},
                                     {-q, -q, q},
                                     {q, q, -q},
                                     {-q, q, -q},
                                     {q, -q, -q},
                                     {-q, -q, -q}};
    const Point3Dd pr0 = R * points(0);
    // clang-format on
    std::pair<Point3Dd, Point3Dd> pmiw = {points(0), pr0}, pmaw = {points(0), pr0}, pmih = {points(0), pr0},
                                  pmah = {points(0), pr0};
    for (const Point3Dd& p : points)
    {
        const auto pqr = R * p;

        if (pqr.x < pmiw.second.x)
        {
            pmiw.first = p;
            pmiw.second = pqr;
        }

        if (pqr.x > pmaw.second.x)
        {
            pmaw.first = p;
            pmaw.second = pqr;
        }

        if (pqr.y < pmih.second.y)
        {
            pmih.first = p;
            pmih.second = pqr;
        }

        if (pqr.y > pmah.second.y)
        {
            pmah.first = p;
            pmah.second = pqr;
        }
    }

    const double r00 = R(0, 0), r01 = R(0, 1), r02 = R(0, 2);
    const double r10 = R(1, 0), r11 = R(1, 1), r12 = R(1, 2);
    const double w = 0.5, h = 0.5;
    const double pmiw_x = pmiw.first.x, pmiw_y = pmiw.first.y, pmiw_z = pmiw.first.z;
    const double pmih_x = pmih.first.x, pmih_y = pmih.first.y, pmih_z = pmih.first.z;

    const double sx = -w / (pmiw_x * r00 + pmiw_y * r01 + pmiw_z * r02);
    const double sy = -h / (pmih_x * r10 + pmih_y * r11 + pmih_z * r12);
    const double sz = 1.0;

    return Vec3Dd(sx, sy, sz);
}

AxesRenderer::AxesRenderer(const AxesSettings& axes_settings) : axes_settings_(axes_settings)
{
    plot_box_walls_ = new PlotBoxWalls(1.0f);
    plot_box_silhouette_ = new PlotBoxSilhouette(1.0f);
    plot_box_grid_ = new PlotBoxGrid(1.0f);

    const std::string v_path = "../main_application/axes/shaders/basic.vertex";
    const std::string f_path = "../main_application/axes/shaders/basic.fragment";
    plot_shader_ = Shader::createFromFiles(v_path, f_path);

    const std::string v_path_text = "../main_application/axes/shaders/text.vs";
    const std::string f_path_text = "../main_application/axes/shaders/text.fs";
    text_shader_ = Shader::createFromFiles(v_path_text, f_path_text);

    const std::string v_path_plot_shader = "../main_application/axes/shaders/plot_shader.vs";
    const std::string f_path_plot_shader = "../main_application/axes/shaders/plot_shader.fs";

    plot_shader_2 = Shader::createFromFiles(v_path_plot_shader, f_path_plot_shader);

    glUseProgram(text_shader_.programId());

    initFreetype();

    glUniform1i(glGetUniformLocation(text_shader_.programId(), "text_sampler"), 0);

    const float sw = 3.0f;
    orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);;
    persp_projection_mat = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    use_perspective_proj_ = false;

    projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    // Camera matrix
    view_mat = glm::lookAt(glm::vec3(0, -6.0, 0),
                           glm::vec3(0, 0, 0),
                           glm::vec3(0, 0, 1));
    model_mat = glm::mat4(1.0f);
    scale_mat = glm::mat4(1.0f);
}

void AxesRenderer::render()
{
    renderPlotBox();
    renderBoxGrid();
    drawGridNumbers(text_renderer_, text_shader_, axes_limits_, view_angles_, view_mat, model_mat, projection_mat, width_, height_, gv_);
}

void AxesRenderer::renderBoxGrid()
{
    glUseProgram(plot_shader_.programId());

    const Vec3Dd scale = axes_limits_.getAxesScale();
    const Vec3Dd s = axes_settings_.getAxesScale();

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0 / scale.y;
    scale_mat[2][2] = 1.0 / scale.z;
    scale_mat[3][3] = 1.0;

    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat;

    glUniformMatrix4fv(glGetUniformLocation(plot_shader_.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);

    plot_box_grid_->render(gv_,
                           axes_settings_,
                           axes_limits_,
                           view_angles_);

    glUseProgram(0);
}

void AxesRenderer::plotBegin()
{
    glUseProgram(plot_shader_2.programId());

    const Vec3Dd axes_center = axes_limits_.getAxesCenter();

    const Vec3Dd scale = axes_limits_.getAxesScale();

    glm::mat4 t_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-axes_center.x, -axes_center.y, -axes_center.z));

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0 / scale.y;
    scale_mat[2][2] = 1.0 / scale.z;
    scale_mat[3][3] = 1.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat * t_mat;

    glUniform3f(glGetUniformLocation(plot_shader_2.programId(), "vertex_color"), 1.0f, 0.0f, 0.0f);
    glUniform1f(glGetUniformLocation(plot_shader_2.programId(), "point_size"), 3.0f);
    glUniformMatrix4fv(glGetUniformLocation(plot_shader_2.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);
}

void AxesRenderer::plotEnd()
{
    glUseProgram(0);
}

void AxesRenderer::renderPlotBox()
{
    glUseProgram(plot_shader_.programId());

    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    scale_mat[0][0] = 1.0;
    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0;
    scale_mat[3][3] = 1.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat;

    glUniformMatrix4fv(glGetUniformLocation(plot_shader_.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);

    plot_box_walls_->render(view_angles_.getAzimuth(), view_angles_.getElevation());
    plot_box_silhouette_->render();

    glUseProgram(0);
}

void AxesRenderer::updateStates(const AxesLimits& axes_limits,
                                const ViewAngles& view_angles,
                                const GridVectors& gv,
                                const bool use_perspective_proj,
                                const float width,
                                const float height)
{
    axes_limits_ = axes_limits;
    view_angles_ = view_angles;
    gv_ = gv;
    use_perspective_proj_ = use_perspective_proj;
    width_ = width;
    height_ = height;

    rot_mat = rotationMatrixZ(-view_angles_.getAzimuth()) * 
              rotationMatrixX(-view_angles_.getElevation());
    // const Vec3Dd new_scale = findScale(rot_mat);

    for(int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            model_mat[r][c] = rot_mat(r, c);
        }
    }


    projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;
}
