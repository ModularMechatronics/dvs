#include "axes_renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

AxesRenderer::AxesRenderer(const AxesSettings& axes_settings) : axes_settings_(axes_settings)
{
    plot_box_walls_ = new PlotBoxWalls(1.0f);
    plot_box_silhouette_ = new PlotBoxSilhouette(1.0f);
    plot_box_grid_ = new PlotBoxGrid(1.0f);

    const std::string v_path = "../applications/shader_app/shaders/basic.vertex";
    const std::string f_path = "../applications/shader_app/shaders/basic.fragment";
    shader_ = Shader::createFromFiles(v_path, f_path);
}

void AxesRenderer::render()
{
    glUseProgram(shader_.programId());

    // Angles
    const ViewAngles va = view_angles_;
    const Matrix<double> rot_mat = rotationMatrixZ(-va.getAzimuth()) * rotationMatrixZ(static_cast<double>(M_PI)) *  
                                   rotationMatrixX(va.getElevation()) *
                                   rotationMatrixX(static_cast<double>(M_PI) / 2.0f);

    // AxesLimits
    // const AxesLimits axes_limits_ = axes_interactor_->getAxesLimits();
    const Vec3Dd axes_center = axes_limits_.getAxesCenter();
    // axes_settings_ = axes_interactor_->getAxesSettings();

    // Scales
    // Vec3Dd sq = axes_settings_.getAxesScale();
    const Vec3Dd s = axes_limits_.getAxesScale();

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

    plot_box_walls_->render(view_angles_.getAzimuth(), view_angles_.getElevation());
    plot_box_silhouette_->render();
    plot_box_grid_->render(gv_,
                           axes_settings_,
                           axes_limits_,
                           view_angles_);

    glUseProgram(0);
}

void AxesRenderer::reloadShader()
{
    glDeleteShader(shader_.programId());

    const std::string v_path = "../applications/shader_app/shaders/basic.vertex";
    const std::string f_path = "../applications/shader_app/shaders/basic.fragment";
    shader_ = Shader::createFromFiles(v_path,f_path);
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
    // const Vec3Dd new_scale = findScale(view_angles_.getSnappedRotationMatrix());
    // axes_settings_.setAxesScale(new_scale);
}