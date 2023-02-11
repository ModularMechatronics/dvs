#include "grid_numbers.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <iostream>

#include "dvs/math/math.h"
#include "misc/misc.h"
#include "opengl_low_level/opengl_header.h"
#include "text_rendering.h"

const float kTextScale{0.00035f};

void drawXLetter(const TextRenderer& text_renderer,
                 const glm::mat4& view_model,
                 const glm::vec4& v_viewport,
                 const glm::mat4& projection,
                 const float width,
                 const float height,
                 const float y,
                 const float z)
{
    const glm::vec3 v(0.0f, y * 1.1, z * 1.1);

    const glm::vec3 v_projected = glm::project(v, view_model, projection, v_viewport);

    text_renderer.renderTextFromRightCenter("X", v_projected[0], v_projected[1], 0.0004f, width, height);
}

void drawYLetter(const TextRenderer& text_renderer,
                 const glm::mat4& view_model,
                 const glm::vec4& v_viewport,
                 const glm::mat4& projection,
                 const float width,
                 const float height,
                 const float x,
                 const float z)
{
    const glm::vec3 v(x * 1.1, 0.0f, z * 1.1);

    const glm::vec3 v_projected = glm::project(v, view_model, projection, v_viewport);

    text_renderer.renderTextFromRightCenter("Y", v_projected[0], v_projected[1], 0.0004f, width, height);
}

void drawZLetter(const TextRenderer& text_renderer,
                 const glm::mat4& view_model,
                 const glm::vec4& v_viewport,
                 const glm::mat4& projection,
                 const float width,
                 const float height,
                 const float x,
                 const float y)
{
    const glm::vec3 v(x * 1.1, y * 1.1, 0.0f);

    const glm::vec3 v_projected = glm::project(v, view_model, projection, v_viewport);

    text_renderer.renderTextFromRightCenter("Z", v_projected[0], v_projected[1], 0.0004f, width, height);
}

void drawXAxisNumbers(const TextRenderer& text_renderer,
                      const ElementSettings& element_settings,
                      const glm::mat4& view_model,
                      const glm::vec4& v_viewport,
                      const glm::mat4& projection,
                      const double azimuth,
                      const double elevation,
                      const float width,
                      const float height,
                      const SnappingAxis snapping_axis,
                      const Vec3d& axes_center,
                      const GridVectors& gv,
                      const GLint text_color_uniform)
{
    const bool cond = (azimuth > (M_PI / 2.0)) || (azimuth < (-M_PI / 2.0));
    const double y = (elevation == M_PI / 2.0) ? -1.05 : (cond ? 1.05 : -1.05);
    const double z0 = elevation > 0.0 ? -1.05 : 1.05;
    const double z = ((snapping_axis == SnappingAxis::X) || (snapping_axis == SnappingAxis::Y)) ? -1.05 : z0;

    if (element_settings.axes_letters_on)
    {
        const auto c = element_settings.axes_letters_color;
        glUniform3f(text_color_uniform, c.red, c.green, c.blue);
        drawXLetter(text_renderer, view_model, v_viewport, projection, width, height, y, z);
    }

    if (!element_settings.axes_numbers_on)
    {
        return;
    }
    const auto c = element_settings.axes_numbers_color;
    glUniform3f(text_color_uniform, c.red, c.green, c.blue);

    const bool cond2 =
        ((azimuth <= 0) && (azimuth >= (-M_PI / 2.0))) || ((azimuth >= (M_PI / 2.0)) && (azimuth <= (M_PI)));

    for (size_t k = 0; k < gv.x.num_valid_values; k++)
    {
        const double x = gv.x.data[k];
        const glm::vec3 v3(x, y, z);

        const glm::vec3 v_projected = glm::project(v3, view_model, projection, v_viewport);
        const std::string val = formatNumber(gv.x.data[k] + axes_center.x, 3);
        if (cond2)
        {
            text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], kTextScale, width, height);
        }
        else
        {
            text_renderer.renderTextFromLeftCenter(val, v_projected[0], v_projected[1], kTextScale, width, height);
        }
    }
}

void drawYAxisNumbers(const TextRenderer& text_renderer,
                      const ElementSettings& element_settings,
                      const glm::mat4& view_model,
                      const glm::vec4& v_viewport,
                      const glm::mat4& projection,
                      const double azimuth,
                      const double elevation,
                      const float width,
                      const float height,
                      const SnappingAxis snapping_axis,
                      const Vec3d& axes_center,
                      const GridVectors& gv,
                      const GLint text_color_uniform)
{
    const double x = (elevation == M_PI / 2.0) ? -1.05 : (azimuth < 0.0 ? 1.05 : -1.05);
    const double z0 = elevation > 0.0 ? -1.05 : 1.05;
    const double z = ((snapping_axis == SnappingAxis::X) || (snapping_axis == SnappingAxis::Y)) ? -1.05 : z0;

    const bool cond2 =
        ((azimuth <= 0) && (azimuth >= (-M_PI / 2.0))) || ((azimuth >= (M_PI / 2.0)) && (azimuth <= (M_PI)));

    if (element_settings.axes_letters_on)
    {
        const auto c = element_settings.axes_letters_color;
        glUniform3f(text_color_uniform, c.red, c.green, c.blue);
        drawYLetter(text_renderer, view_model, v_viewport, projection, width, height, x, z);
    }

    if (!element_settings.axes_numbers_on)
    {
        return;
    }
    const auto c = element_settings.axes_numbers_color;
    glUniform3f(text_color_uniform, c.red, c.green, c.blue);

    for (size_t k = 0; k < gv.y.num_valid_values; k++)
    {
        const double y = gv.y.data[k];
        const glm::vec3 v3(x, y, z);

        const glm::vec3 v_projected = glm::project(v3, view_model, projection, v_viewport);
        const std::string val = formatNumber(gv.y.data[k] + axes_center.y, 3);
        if (elevation == M_PI / 2.0)
        {
            text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], kTextScale, width, height);
        }
        else if (cond2)
        {
            text_renderer.renderTextFromLeftCenter(val, v_projected[0], v_projected[1], kTextScale, width, height);
        }
        else
        {
            text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], kTextScale, width, height);
        }
    }
}

void drawZAxisNumbers(const TextRenderer& text_renderer,
                      const ElementSettings& element_settings,
                      const glm::mat4& view_model,
                      const glm::vec4& v_viewport,
                      const glm::mat4& projection,
                      const double azimuth,
                      const double elevation,
                      const float width,
                      const float height,
                      const Vec3d& axes_center,
                      const GridVectors& gv,
                      const GLint text_color_uniform)
{
    const bool cond = (azimuth >= (M_PI / 2.0)) || (azimuth <= (-M_PI / 2.0));
    const double x = cond ? 1.05 : -1.05;
    const double y = ((azimuth >= (M_PI / 2.0)) || (azimuth <= (-M_PI / 2.0))) ? 1.05 : -1.05;

    if (element_settings.axes_letters_on)
    {
        const auto c = element_settings.axes_letters_color;
        glUniform3f(text_color_uniform, c.red, c.green, c.blue);
        drawZLetter(text_renderer, view_model, v_viewport, projection, width, height, x, y);
    }

    if (!element_settings.axes_numbers_on)
    {
        return;
    }
    const auto c = element_settings.axes_numbers_color;
    glUniform3f(text_color_uniform, c.red, c.green, c.blue);

    for (size_t k = 0; k < gv.z.num_valid_values; k++)
    {
        const double z = gv.z.data[k];
        const glm::vec3 v3(x, y, z);

        const glm::vec3 v_projected = glm::project(v3, view_model, projection, v_viewport);
        const std::string val = formatNumber(gv.z.data[k] + axes_center.z, 3);

        text_renderer.renderTextFromRightCenter(val, v_projected[0], v_projected[1], kTextScale, width, height);
    }
}

void drawGridNumbers(const TextRenderer& text_renderer,
                     const Shader text_shader,
                     const AxesLimits& axes_limits,
                     const ViewAngles& view_angles,
                     const ElementSettings& element_settings,
                     const glm::mat4& view_mat,
                     const glm::mat4& model_mat,
                     const glm::mat4& projection_mat,
                     const float width,
                     const float height,
                     const GridVectors& gv)
{
    glUseProgram(text_shader.programId());

    glm::mat4 model_mat_local = model_mat;

    const GLint text_color_uniform = glGetUniformLocation(text_shader.programId(), "textColor");

    // AxesLimits
    const Vec3d axes_center = axes_limits.getAxesCenter();

    // Scales
    const Vec3d scale = axes_limits.getAxesScale() / 2.0;

    model_mat_local[3][0] = 0.0;
    model_mat_local[3][1] = 0.0;
    model_mat_local[3][2] = 0.0;

    const glm::vec4 v_viewport = glm::vec4(-1, -1, 2, 2);

    const double az = view_angles.getSnappedAzimuth();
    const double el = view_angles.getSnappedElevation();

    glm::mat4 scale_mat = glm::mat4(1.0f);

    scale_mat[0][0] = 1.0 / scale.x;
    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0;
    scale_mat[3][3] = 1.0;

    const glm::mat4 view_model_x = view_mat * model_mat_local * scale_mat;

    scale_mat[0][0] = 1.0;
    scale_mat[1][1] = 1.0 / scale.y;
    const glm::mat4 view_model_y = view_mat * model_mat_local * scale_mat;

    scale_mat[1][1] = 1.0;
    scale_mat[2][2] = 1.0 / scale.z;
    const glm::mat4 view_model_z = view_mat * model_mat_local * scale_mat;

    if (!view_angles.isSnappedAlongX())
    {
        drawXAxisNumbers(text_renderer,
                         element_settings,
                         view_model_x,
                         v_viewport,
                         projection_mat,
                         az,
                         el,
                         width,
                         height,
                         view_angles.getSnappingAxis(),
                         axes_center,
                         gv,
                         text_color_uniform);
    }
    if (!view_angles.isSnappedAlongY())
    {
        drawYAxisNumbers(text_renderer,
                         element_settings,
                         view_model_y,
                         v_viewport,
                         projection_mat,
                         az,
                         el,
                         width,
                         height,
                         view_angles.getSnappingAxis(),
                         axes_center,
                         gv,
                         text_color_uniform);
    }
    if (!view_angles.isSnappedAlongZ())
    {
        drawZAxisNumbers(text_renderer,
                         element_settings,
                         view_model_z,
                         v_viewport,
                         projection_mat,
                         az,
                         el,
                         width,
                         height,
                         axes_center,
                         gv,
                         text_color_uniform);
    }

    glUseProgram(0);
}