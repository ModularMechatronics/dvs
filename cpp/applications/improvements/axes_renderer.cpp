#include "axes_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "dvs/math/math.h"
#include "vertex_data.h"
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

    const std::string v_path = "../applications/improvements/shaders/basic.vertex";
    const std::string f_path = "../applications/improvements/shaders/basic.fragment";
    shader_ = Shader::createFromFiles(v_path, f_path);
    plot_shader_ = Shader::createFromFiles(v_path, f_path);

    half_cube_ = VboWrapper3D(half_cube_vertices_num_vertices, half_cube_vertices, half_cube_color);
}

void AxesRenderer::render()
{
    renderPlotBox();
    renderBoxGrid();

    plotBegin();
    half_cube_.render();
    plotEnd();
}

void AxesRenderer::renderBoxGrid()
{
    glUseProgram(shader_.programId());

    // Angles
    const ViewAngles va = view_angles_;
    const Matrix<double> rot_mat = rotationMatrixZ(-va.getAzimuth()) * 
                                   rotationMatrixX(-va.getElevation());

    const Vec3Dd new_scale = findScale(rot_mat);
    const Vec3Dd scale = axes_limits_.getAxesScale();
    const Vec3Dd s = axes_settings_.getAxesScale();

    const float sw = 3.0f;
    glm::mat4 orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);;
    glm::mat4 persp_projection_mat = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    glm::mat4 projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    // Camera matrix
    glm::mat4 view_mat = glm::lookAt(glm::vec3(0, -6.0, 0),
                                 glm::vec3(0, 0, 0),
                                 glm::vec3(0, 0, 1));
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 scale_mat = glm::mat4(0.1);

    // scale_mat[0][0] = 1.0f / scale.x;
    // scale_mat[1][1] = 1.0f / scale.y;
    // scale_mat[2][2] = 1.0f / scale.z;
    // scale_mat[3][3] = 1.0;

    for(int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            model_mat[r][c] = rot_mat(r, c);
        }
    }

    const glm::mat4 mvp = projection_mat * view_mat * model_mat; //  * scale_mat;

    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);

    plot_box_grid_->render(gv_,
                           axes_settings_,
                           axes_limits_,
                           view_angles_);

    glUseProgram(0);
}

void AxesRenderer::plotBegin()
{
    glUseProgram(plot_shader_.programId());

    // Angles
    const ViewAngles va = view_angles_;
    const Matrix<double> rot_mat = rotationMatrixZ(-va.getAzimuth()) * 
                                   rotationMatrixX(-va.getElevation());

    const Vec3Dd new_scale = findScale(rot_mat);
    // AxesLimits
    // const AxesLimits axes_limits_ = axes_interactor_->getAxesLimits();
    const Vec3Dd axes_center = axes_limits_.getAxesCenter();

    // We should be rotating around z axis

    // Scales
    const Vec3Dd scale = axes_limits_.getAxesScale();
    const Vec3Dd s = axes_settings_.getAxesScale();

    const float sw = 3.0f;
    glm::mat4 orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);;
    glm::mat4 persp_projection_mat = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    glm::mat4 projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    // Camera matrix
    glm::mat4 view_mat = glm::lookAt(glm::vec3(0, -6.0, 0),
                                 glm::vec3(0, 0, 0),
                                 glm::vec3(0, 0, 1));
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 scale_mat = glm::mat4(0.1);

    model_mat[3][0] = axes_center.x;
    model_mat[3][1] = axes_center.y;
    model_mat[3][2] = axes_center.z;

    scale_mat[0][0] = scale.x;
    scale_mat[1][1] = scale.y;
    scale_mat[2][2] = scale.z;
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
}

void AxesRenderer::plotEnd()
{
    glUseProgram(0);
}

void AxesRenderer::renderPlotBox()
{
    glUseProgram(shader_.programId());

    // Angles
    const ViewAngles va = view_angles_;
    const Matrix<double> rot_mat = rotationMatrixZ(-va.getAzimuth()) * 
                                   rotationMatrixX(-va.getElevation());

    const Vec3Dd new_scale = findScale(rot_mat);
    const Vec3Dd scale = axes_limits_.getAxesScale();
    const Vec3Dd s = axes_settings_.getAxesScale();

    const float sw = 3.0f;
    glm::mat4 orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);;
    glm::mat4 persp_projection_mat = glm::perspective(glm::radians(75.0f), 1.0f, 0.1f, 100.0f);

    glm::mat4 projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    // Camera matrix
    glm::mat4 view_mat = glm::lookAt(glm::vec3(0, -6.0, 0),
                                 glm::vec3(0, 0, 0),
                                 glm::vec3(0, 0, 1));
    glm::mat4 model_mat = glm::mat4(1.0f);
    glm::mat4 scale_mat = glm::mat4(0.1);

    scale_mat[0][0] = 2.0f * new_scale.x;
    scale_mat[1][1] = 2.0f * new_scale.y;
    scale_mat[2][2] = 2.0f * new_scale.z;
    scale_mat[3][3] = 1.0;

    for(int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            model_mat[r][c] = rot_mat(r, c);
        }
    }

    const glm::mat4 mvp = projection_mat * view_mat * model_mat; //  * scale_mat;

    glUniformMatrix4fv(glGetUniformLocation(shader_.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);

    plot_box_walls_->render(view_angles_.getAzimuth(), view_angles_.getElevation());
    plot_box_silhouette_->render();

    glUseProgram(0);
}

void AxesRenderer::reloadShader()
{
    glDeleteShader(shader_.programId());
    glDeleteShader(plot_shader_.programId());

    const std::string v_path = "../applications/improvements/shaders/basic.vertex";
    const std::string f_path = "../applications/improvements/shaders/basic.fragment";
    shader_ = Shader::createFromFiles(v_path,f_path);
    plot_shader_ = Shader::createFromFiles(v_path, f_path);
}

void AxesRenderer::updateStates(const AxesLimits& axes_limits,
                                const ViewAngles& view_angles,
                                const GridVectors& gv,
                                const CoordinateConverter& coord_converter,
                                const bool use_perspective_proj)
{
    axes_limits_ = axes_limits;
    view_angles_ = view_angles;
    gv_ = gv;
    coord_converter_ = coord_converter;
    use_perspective_proj_ = use_perspective_proj;
}