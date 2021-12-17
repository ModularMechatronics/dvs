#include "axes_renderer.h"

#include "dvs/math/math.h"
#include "vertex_data.h"
#include "text_rendering.h"
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
    for (const Point3Dd p : points)
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
    plot_box_grid_numbers_ = new PlotBoxGridNumbers(0.1f);

    const std::string v_path = "../applications/improvements/shaders/basic.vertex";
    const std::string f_path = "../applications/improvements/shaders/basic.fragment";
    plot_shader_ = Shader::createFromFiles(v_path, f_path);

    const std::string v_path_text = "../applications/improvements/shaders/text.vertex";
    const std::string f_path_text = "../applications/improvements/shaders/text.fragment";
    text_shader_ = Shader::createFromFiles(v_path_text, f_path_text);

    initText2D("../applications/improvements/Holstein.DDS");

    half_cube_ = VboWrapper3D(half_cube_vertices_num_vertices, half_cube_vertices, half_cube_color);

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
    scale_mat = glm::mat4(1.0);
}

void AxesRenderer::render()
{
    renderPlotBox();
    renderBoxGrid();
    renderBoxGridNumbers();

    plotBegin();
    half_cube_.render();
    plotEnd();
}

void drawXAxisNumbers(const glm::mat4& view_model,
                      const glm::vec4& v_viewport,
                      const glm::mat4& projection,
                      const Vec3Dd& axes_center,
                      const GridVectors& gv)
{
    const double y = -1.0;
    const double z = -1.0;
    for(size_t k = 0; k < gv.x.size(); k++)
    {
        const double x = gv.x(k);
        glm::vec3 v3(x, y, z);

        glm::vec3 v_projected = glm::project(v3,
                         view_model,
                         projection,
                         v_viewport);
        const std::string val = formatNumber(gv.x(k) + axes_center.x, 3);
        printText2D(val.c_str(), v_projected[0], v_projected[1], 10);
    }
}

void AxesRenderer::renderBoxGridNumbers()
{
    glUseProgram(text_shader_.programId());

    // AxesLimits
    const Vec3Dd axes_center = axes_limits_.getAxesCenter();

    // Scales
    const Vec3Dd scale = axes_limits_.getAxesScale();
    const Vec3Dd s = axes_settings_.getAxesScale();

    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0; // / scale.y;
    scale_mat[2][2] = 1.0; // / scale.z;
    scale_mat[3][3] = 1.0;

    // glm::vec3 v3(1.0, 1.0, 1.0);

    const glm::vec4 v_viewport = glm::vec4(0, 0, width_, height_);
    const glm::mat4 view_model = view_mat * model_mat * scale_mat;

    drawXAxisNumbers(view_model, v_viewport, projection_mat, axes_center, gv_);

    /*glm::vec3 v_projected = glm::project(v3,
                         view_model,
                         projection_mat,
                         v_viewport);

    const char* const some_text = "hello";
    printText2D(some_text, v_projected[0], v_projected[1], 20);*/

    glUniform1f(glGetUniformLocation(text_shader_.programId(), "half_width"), width_ / 2.0f);
    glUniform1f(glGetUniformLocation(text_shader_.programId(), "half_height"), height_ / 2.0f);

    /*plot_box_grid_numbers_->render(gv_,
                                   axes_settings_,
                                   axes_limits_,
                                   view_angles_,
                                   coord_converter_,
                                   width_,
                                   height_);*/
    glUseProgram(0);
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
    glUseProgram(plot_shader_.programId());

    // AxesLimits
    const Vec3Dd axes_center = axes_limits_.getAxesCenter();

    // Scales
    const Vec3Dd scale = axes_limits_.getAxesScale();
    const Vec3Dd s = axes_settings_.getAxesScale();

    model_mat[3][0] = axes_center.x;
    model_mat[3][1] = axes_center.y;
    model_mat[3][2] = axes_center.z;

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0 / scale.y;
    scale_mat[2][2] = 1.0 / scale.z;
    scale_mat[3][3] = 1.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat;

    glUniformMatrix4fv(glGetUniformLocation(plot_shader_.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);
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

    // plot_box_walls_->render(view_angles_.getAzimuth(), view_angles_.getElevation());
    plot_box_silhouette_->render();

    glUseProgram(0);
}

void AxesRenderer::reloadShader()
{
    glDeleteShader(plot_shader_.programId());
    glDeleteShader(text_shader_.programId());

    const std::string v_path = "../applications/improvements/shaders/basic.vertex";
    const std::string f_path = "../applications/improvements/shaders/basic.fragment";
    plot_shader_ = Shader::createFromFiles(v_path, f_path);

    const std::string v_path_text = "../applications/improvements/shaders/text.vertex";
    const std::string f_path_text = "../applications/improvements/shaders/text.fragment";
    text_shader_ = Shader::createFromFiles(v_path_text, f_path_text);
}

void AxesRenderer::updateStates(const AxesLimits& axes_limits,
                                const ViewAngles& view_angles,
                                const GridVectors& gv,
                                const CoordinateConverter& coord_converter,
                                const bool use_perspective_proj,
                                const float width,
                                const float height)
{
    axes_limits_ = axes_limits;
    view_angles_ = view_angles;
    gv_ = gv;
    coord_converter_ = coord_converter;
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
