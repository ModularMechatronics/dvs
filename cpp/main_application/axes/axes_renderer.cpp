#include "axes_renderer.h"

#include "dvs/math/math.h"
#include "misc/misc.h"

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using namespace dvs;

static Vec3Dd findScale(const glm::mat4& pm)
{
    // Currently unknown exactly how 'q' affects the results...
    const double q = 1.0;
    // clang-format off
    const Vector<glm::vec4> points = {{q, q, q, 1.0},
                                     {-q, q, q, 1.0},
                                     {q, -q, q, 1.0},
                                     {-q, -q, q, 1.0},
                                     {q, q, -q, 1.0},
                                     {-q, q, -q, 1.0},
                                     {q, -q, -q, 1.0},
                                     {-q, -q, -q, 1.0}};
    const glm::vec4 pr0 = pm * points(0);
    // clang-format on
    std::pair<glm::vec4, glm::vec4> pmiw = {points(0), pr0}, pmaw = {points(0), pr0}, pmih = {points(0), pr0},
                                  pmah = {points(0), pr0};
    for (const glm::vec4& p : points)
    {
        auto pqr = pm * p;
        pqr /= pqr.w;
        pqr = pqr * 0.5f + 0.5f;
        pqr = pqr * 2.0f + 1.0f;

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

    // const double r00 = R(0, 0), r01 = R(0, 1), r02 = R(0, 2);
    // const double r10 = R(1, 0), r11 = R(1, 1), r12 = R(1, 2);
    // const double r20 = R(2, 0), r21 = R(2, 1), r22 = R(2, 2);
    const double q00 = pm[0][0], q01 = pm[0][1], q02 = pm[0][2], q03 = pm[0][3];
    const double q10 = pm[1][0], q11 = pm[1][1], q12 = pm[1][2], q13 = pm[1][3];
    const double q20 = pm[2][0], q21 = pm[2][1], q22 = pm[2][2], q23 = pm[2][3];
    const double q30 = pm[3][0], q31 = pm[3][1], q32 = pm[3][2], q33 = pm[3][3];
    const double w = 0.1, h = 0.1;
    const double pmiw_x = pmiw.first.x, pmiw_y = pmiw.first.y, pmiw_z = pmiw.first.z;
    const double pmih_x = pmih.first.x, pmih_y = pmih.first.y, pmih_z = pmih.first.z;

    const double sx = (pmih_y*q03*q11 - pmiw_y*q01*q13 + h*pmih_y*q03*q31 - h*pmiw_y*q01*q33 + pmih_y*pmiw_z*q02*q11 - pmih_z*pmiw_y*q01*q12 + pmih_y*q11*q33*w - pmiw_y*q13*q31*w + h*pmih_y*pmiw_z*q02*q31 - h*pmih_z*pmiw_y*q01*q32 + h*pmih_y*q31*q33*w - h*pmiw_y*q31*q33*w + pmih_y*pmiw_z*q11*q32*w - pmih_z*pmiw_y*q12*q31*w + h*pmih_y*pmiw_z*q31*q32*w - h*pmih_z*pmiw_y*q31*q32*w)/(pmih_x*pmiw_y*q01*q10 - pmih_y*pmiw_x*q00*q11 + h*pmih_x*pmiw_y*q01*q30 - h*pmih_y*pmiw_x*q00*q31 + pmih_x*pmiw_y*q10*q31*w - pmih_y*pmiw_x*q11*q30*w + h*pmih_x*pmiw_y*q30*q31*w - h*pmih_y*pmiw_x*q30*q31*w);
    const double sy = -(pmih_x*q03*q10 - pmiw_x*q00*q13 + h*pmih_x*q03*q30 - h*pmiw_x*q00*q33 + pmih_x*pmiw_z*q02*q10 - pmih_z*pmiw_x*q00*q12 + pmih_x*q10*q33*w - pmiw_x*q13*q30*w + h*pmih_x*pmiw_z*q02*q30 - h*pmih_z*pmiw_x*q00*q32 + h*pmih_x*q30*q33*w - h*pmiw_x*q30*q33*w + pmih_x*pmiw_z*q10*q32*w - pmih_z*pmiw_x*q12*q30*w + h*pmih_x*pmiw_z*q30*q32*w - h*pmih_z*pmiw_x*q30*q32*w)/(pmih_x*pmiw_y*q01*q10 - pmih_y*pmiw_x*q00*q11 + h*pmih_x*pmiw_y*q01*q30 - h*pmih_y*pmiw_x*q00*q31 + pmih_x*pmiw_y*q10*q31*w - pmih_y*pmiw_x*q11*q30*w + h*pmih_x*pmiw_y*q30*q31*w - h*pmih_y*pmiw_x*q30*q31*w);
    const double sz = 1.0;

    // std::cout << Vec3Dd(sx, sy, sz) << std::endl;

    return Vec3Dd(sx, sy, sz);
}

AxesRenderer::AxesRenderer(const ShaderCollection shader_collection) :
shader_collection_{shader_collection}, legend_renderer_{text_renderer_, shader_collection_}
{
    plot_box_walls_ = new PlotBoxWalls(1.0f);
    plot_box_silhouette_ = new PlotBoxSilhouette(1.0f);
    plot_box_grid_ = new PlotBoxGrid(1.0f);

    glUseProgram(shader_collection_.text_shader.programId());

    initFreetype();

    glUniform1i(glGetUniformLocation(shader_collection_.text_shader.programId(), "text_sampler"), 0);

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
    
    window_scale_mat_ = glm::mat4(1.0f);
    window_scale_mat_[0][0] = 2.7;
    window_scale_mat_[1][1] = 2.7;
    window_scale_mat_[2][2] = 2.7;
}

void AxesRenderer::enableClipPlanes()
{
    const double f = 0.1;

    const Vec3Dd axes_center = axes_limits_.getAxesCenter();

    const Vec3Dd scale = axes_limits_.getAxesScale() / 2.0;

    // clang-format off
    const Vector<Point3Dd> points_x0 = {(Point3Dd(scale.x - axes_center.x, f, f)),
                                       (Point3Dd(scale.x - axes_center.x, -f, f)),
                                       (Point3Dd(scale.x - axes_center.x, f, -f))};
    const Vector<Point3Dd> points_x1 = {(Point3Dd(scale.x + axes_center.x, f, f)),
                                       (Point3Dd(scale.x + axes_center.x, -f, f)),
                                       (Point3Dd(scale.x + axes_center.x, f, -f))};
    const Vector<Point3Dd> points_y0 = {(Point3Dd(-f, scale.y - axes_center.y, f)),
                                       (Point3Dd(f, scale.y - axes_center.y, f)),
                                       (Point3Dd(f, scale.y - axes_center.y, -f))};
    const Vector<Point3Dd> points_y1 = {(Point3Dd(-f, scale.y + axes_center.y, f)),
                                       (Point3Dd(f, scale.y + axes_center.y, f)),
                                       (Point3Dd(f, scale.y + axes_center.y, -f))};
    const Vector<Point3Dd> points_z0 = {(Point3Dd(-f, f, -(axes_center.z + scale.z))),
                                       (Point3Dd(f, -f, -(axes_center.z + scale.z))),
                                       (Point3Dd(-f, -f, -(axes_center.z + scale.z)))};
    const Vector<Point3Dd> points_z1 = {(Point3Dd(-f, f, -(scale.z - axes_center.z))),
                                       (Point3Dd(f, -f, -(scale.z - axes_center.z))),
                                       (Point3Dd(-f, -f, -(scale.z - axes_center.z)))};
    // clang-format on*/

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

    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

void AxesRenderer::setClipPlane(const GLuint program_id, const std::string pln, const Point3Dd& p0, const Point3Dd& p1, const Point3Dd& p2, const bool invert) const
{
    // Fit plane
    const Planed fp = planeFromThreePoints(p0, p1, p2);

    // Invert
    const Planed plane = invert ? Planed(-fp.a, -fp.b, -fp.c, fp.d) : fp;

    glUniform4f(glGetUniformLocation(program_id, pln.c_str()), plane.a, plane.b, plane.c, plane.d);
}

void AxesRenderer::render()
{
    renderPlotBox();
    if(render_zoom_rect_)
    {
        glUseProgram(shader_collection_.plot_box_shader.programId());

        model_mat[3][0] = 0.0;
        model_mat[3][1] = 0.0;
        model_mat[3][2] = 0.0;

        const glm::mat4 mvp = projection_mat * view_mat * model_mat * window_scale_mat_;

        glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);
        
        zoom_rect_.render(mouse_pos_at_press_, current_mouse_pos_, view_angles_.getSnappingAxis(), view_mat, model_mat * window_scale_mat_, projection_mat);
        glUseProgram(0);
    }

    renderBoxGrid();
    drawGridNumbers(text_renderer_, shader_collection_.text_shader, axes_limits_, view_angles_, view_mat, model_mat * window_scale_mat_, projection_mat, width_, height_, gv_);
    if(render_legend_)
    {
        renderLegend();
    }
}

void AxesRenderer::renderLegend()
{
    glUseProgram(shader_collection_.plot_box_shader.programId());
    const glm::mat4 model_mat_tmp = glm::mat4(1.0f);
    
    const glm::mat4 mvp = projection_mat * view_mat * model_mat_tmp;

    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);
    legend_renderer_.render(legend_names_, width_, height_);
    glUseProgram(0);
}

void AxesRenderer::renderBoxGrid()
{
    glUseProgram(shader_collection_.plot_box_shader.programId());

    const Vec3Dd scale = axes_limits_.getAxesScale() / 2.0;

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0 / scale.y;
    scale_mat[2][2] = 1.0 / scale.z;
    scale_mat[3][3] = 1.0;

    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat * window_scale_mat_;

    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);

    plot_box_grid_->render(gv_,
                           axes_limits_,
                           view_angles_);

    glUseProgram(0);
}

void AxesRenderer::plotBegin()
{
    glUseProgram(shader_collection_.basic_plot_shader.programId());

    const Vec3Dd axes_center = axes_limits_.getAxesCenter();

    const Vec3Dd scale = axes_limits_.getAxesScale() / 2.0;

    glm::mat4 t_mat = glm::translate(glm::mat4(1.0f), glm::vec3(-axes_center.x, -axes_center.y, -axes_center.z));

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0 / scale.y;
    scale_mat[2][2] = 1.0 / scale.z;
    scale_mat[3][3] = 1.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * scale_mat * window_scale_mat_ * t_mat;
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.basic_plot_shader.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);
    enableClipPlanes();

    glUseProgram(shader_collection_.img_plot_shader.programId());
    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.img_plot_shader.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);

    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

void AxesRenderer::plotEnd()
{
    glUseProgram(0);
}

void AxesRenderer::renderPlotBox()
{
    glUseProgram(shader_collection_.plot_box_shader.programId());

    model_mat[3][0] = 0.0;
    model_mat[3][1] = 0.0;
    model_mat[3][2] = 0.0;

    scale_mat[0][0] = 1.0;
    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0;
    scale_mat[3][3] = 1.0;

    const glm::mat4 mvp = projection_mat * view_mat * model_mat * window_scale_mat_;

    glUniformMatrix4fv(glGetUniformLocation(shader_collection_.plot_box_shader.programId(), "model_view_proj_mat"), 1, GL_FALSE, &mvp[0][0]);

    plot_box_walls_->render(view_angles_.getAzimuth(), view_angles_.getElevation());
    plot_box_silhouette_->render();

    glUseProgram(0);
}

void AxesRenderer::updateStates(const AxesLimits& axes_limits,
                                const ViewAngles& view_angles,
                                const GridVectors& gv,
                                const bool use_perspective_proj,
                                const float width,
                                const float height,
                                const Vec2Df mouse_pos_at_press,
                                const Vec2Df current_mouse_pos,
                                const MouseActivity mouse_activity,
                                const bool mouse_pressed,
                                const bool render_zoom_rect,
                                const bool render_legend,
                                const std::vector<LegendProperties>& legend_names)
{
    axes_limits_ = axes_limits;
    view_angles_ = view_angles;
    gv_ = gv;
    use_perspective_proj_ = use_perspective_proj;
    width_ = width;
    height_ = height;
    mouse_pos_at_press_ = mouse_pos_at_press;
    current_mouse_pos_ = current_mouse_pos;
    mouse_activity_ = mouse_activity;
    mouse_pressed_ = mouse_pressed;
    render_zoom_rect_ = render_zoom_rect;
    render_legend_ = render_legend;
    legend_names_ = legend_names;

    rot_mat = rotationMatrixZ(-view_angles_.getSnappedAzimuth()) * 
              rotationMatrixX(-view_angles_.getSnappedElevation());

    for(int r = 0; r < 3; r++)
    {
        for(int c = 0; c < 3; c++)
        {
            model_mat[r][c] = rot_mat(r, c);
        }
    }

    projection_mat = use_perspective_proj_ ? persp_projection_mat : orth_projection_mat;

    const float az = std::pow(std::fabs(std::sin(view_angles_.getSnappedAzimuth() * 2.0f)), 0.6) * 0.7;
    const float el = std::pow(std::fabs(std::sin(view_angles_.getSnappedElevation() * 2.0f)), 0.7) * 0.5;
    const float f = 2.5;

    window_scale_mat_[0][0] = f - az - el;
    window_scale_mat_[1][1] = f - az - el;
    window_scale_mat_[2][2] = f - az - el;

}
