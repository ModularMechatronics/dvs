#include "axes/axes_interactor.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>

#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include "dvs/math/math.h"
#include "io_devices/io_devices.h"
#include "opengl_low_level/opengl_low_level.h"

AxesInteractor::AxesInteractor(const AxesSettings& axes_settings, const int window_height, const int window_width)
{
    assert(axes_settings.getMinVec().x < axes_settings.getMaxVec().x && "x min larger than x max!");
    assert(axes_settings.getMinVec().y < axes_settings.getMaxVec().y && "y min larger than y max!");
    assert(axes_settings.getMinVec().z < axes_settings.getMaxVec().z && "z min larger than z max!");

    axes_limits_ = AxesLimits(axes_settings.getMinVec(), axes_settings.getMaxVec());
    default_axes_limits_ = axes_limits_;
    should_draw_zoom_rect_ = false;

    current_window_width = window_width;
    current_window_height = window_height;

    view_angles_ = ViewAngles(0.0 * M_PI / 180.0, 0.0 * M_PI / 180.0, 5.0 * M_PI / 180.0);
    default_view_angles_ = view_angles_;

    axes_settings_ = axes_settings;
    current_mouse_activity = MouseActivity::ROTATE;

    const size_t num_lines = axes_settings_.getNumAxesTicks();
    inc0 = 0.9999999999 * (default_axes_limits_.getMax() - default_axes_limits_.getMin()) /
           static_cast<double>(num_lines - 1);
    mouse_pressed_ = false;
    show_legend_ = false;
}

void AxesInteractor::registerMousePressed(const Vec2Df& mouse_pos)
{
    mouse_pressed_ = true;
    mouse_pos_at_press_ = mouse_pos;
}

void AxesInteractor::registerMouseReleased(const Vec2Df& mouse_pos)
{
    if(should_draw_zoom_rect_)
    {
        const float sw = 3.0f;
        const glm::mat4 orth_projection_mat = glm::ortho(-sw, sw, -sw, sw, 0.1f, 100.0f);;
        const glm::vec4 v_viewport = glm::vec4(-1, -1, 2, 2);
        const float az = std::pow(std::fabs(std::sin(view_angles_.getSnappedAzimuth() * 2.0f)), 0.6) * 0.7;
        const float el = std::pow(std::fabs(std::sin(view_angles_.getSnappedElevation() * 2.0f)), 0.7) * 0.5;
        const float f = 2.5;
        glm::mat4 window_scale_mat = glm::mat4(1.0f);

        window_scale_mat[0][0] = 2.7;
        window_scale_mat[1][1] = 2.7;
        window_scale_mat[2][2] = 2.7;

        window_scale_mat[0][0] = f - az - el;
        window_scale_mat[1][1] = f - az - el;
        window_scale_mat[2][2] = f - az - el;
        const Matrix<double> rot_mat = rotationMatrixZ(-view_angles_.getSnappedAzimuth()) * 
              rotationMatrixX(-view_angles_.getSnappedElevation());

        glm::mat4 model_mat = glm::mat4(1.0f);

        for(int r = 0; r < 3; r++)
        {
            for(int c = 0; c < 3; c++)
            {
                model_mat[r][c] = rot_mat(r, c);
            }
        }

        const glm::mat4 view_mat = glm::lookAt(glm::vec3(0, -6.0, 0),
                                               glm::vec3(0, 0, 0),
                                               glm::vec3(0, 0, 1));

        glm::mat4 model_mat_mod = view_mat * model_mat * window_scale_mat;

        model_mat_mod[3][0] = 0.0;
        model_mat_mod[3][1] = 0.0;
        model_mat_mod[3][2] = 0.0;
        const Vec2Df mouse_pos_at_release = mouse_pos;
        const Vec2Df window_xy(current_window_width, current_window_height);

        const Vec2Df mouse_pos_at_press_norm = mouse_pos_at_press_; // .elementWiseDivide(window_xy);
        const Vec2Df mouse_pos_at_release_norm = mouse_pos_at_release; // .elementWiseDivide(window_xy);

        const Vec2Df mouse_pos_at_press_mod = 2.0f * (mouse_pos_at_press_norm.elementWiseMultiply(Vec2Df(1.0f, -1.0f)) + Vec2Df(0.0f, 1.0f)) - 1.0f;
        const Vec2Df current_mouse_pos_mod = 2.0f * (mouse_pos_at_release_norm.elementWiseMultiply(Vec2Df(1.0f, -1.0f)) + Vec2Df(0.0f, 1.0f)) - 1.0f;

        const SnappingAxis snapping_axis = view_angles_.getSnappingAxis();

        const glm::vec3 mouse_pos_at_press_projected(mouse_pos_at_press_mod.x, mouse_pos_at_press_mod.y, 0);
        const glm::vec3 current_mouse_pos_projected(current_mouse_pos_mod.x, current_mouse_pos_mod.y, 0);

        const glm::mat4 view_model = view_mat * model_mat * window_scale_mat;

        const glm::vec3 current_mouse_pos_unprojected = glm::unProject(current_mouse_pos_projected, view_model, orth_projection_mat, v_viewport);
        const glm::vec3 mouse_pos_at_press_unprojected = glm::unProject(mouse_pos_at_press_projected, view_model, orth_projection_mat, v_viewport);

        const Vec3Df scale_vec_div_2 = axes_limits_.getAxesScale() / 2.0;
        const Vec3Df scale_vec = axes_limits_.getAxesScale();
        const Vec3Df axes_center = axes_limits_.getAxesCenter();
        const Vec3Df min_vec = axes_center - scale_vec_div_2;
        const Vec3Df max_vec = axes_center + scale_vec_div_2;

        if(SnappingAxis::X == snapping_axis)
        {
            const float x_start = mouse_pos_at_press_unprojected.y / 2.0f + 0.5f;
            const float y_start = mouse_pos_at_press_unprojected.z / 2.0f + 0.5f;

            const float x_end = current_mouse_pos_unprojected.y / 2.0f + 0.5f;
            const float y_end = current_mouse_pos_unprojected.z / 2.0f + 0.5f;
    
            // Min/max
            const float x_min = std::min(x_start, x_end);
            const float x_max = std::max(x_start, x_end);
            const float y_min = std::min(y_start, y_end);
            const float y_max = std::max(y_start, y_end);

            // Find new points from scale
            const float new_y_min = min_vec.y + x_min * scale_vec.y;
            const float new_y_max = min_vec.y + x_max * scale_vec.y;

            const float new_z_min = min_vec.z + y_min * scale_vec.z;
            const float new_z_max = min_vec.z + y_max * scale_vec.z;

            axes_limits_.setMin(Vec3Df(min_vec.x, new_y_min, new_z_min));
            axes_limits_.setMax(Vec3Df(max_vec.x, new_y_max, new_z_max));
        }
        else if(SnappingAxis::Y == snapping_axis)
        {
            // Normalize to be in the interval [0.0, 1.0]
            const float x_start = mouse_pos_at_press_unprojected.x / 2.0f + 0.5f;
            const float y_start = mouse_pos_at_press_unprojected.z / 2.0f + 0.5f;

            const float x_end = current_mouse_pos_unprojected.x / 2.0f + 0.5f;
            const float y_end = current_mouse_pos_unprojected.z / 2.0f + 0.5f;

            // Min/max
            const float x_min = std::min(x_start, x_end);
            const float x_max = std::max(x_start, x_end);
            const float y_min = std::min(y_start, y_end);
            const float y_max = std::max(y_start, y_end);

            // Find new points from scale
            const float new_x_min = min_vec.x + x_min * scale_vec.x;
            const float new_x_max = min_vec.x + x_max * scale_vec.x;

            const float new_z_min = min_vec.z + y_min * scale_vec.z;
            const float new_z_max = min_vec.z + y_max * scale_vec.z;

            axes_limits_.setMin(Vec3Df(new_x_min, min_vec.y, new_z_min));
            axes_limits_.setMax(Vec3Df(new_x_max, max_vec.y, new_z_max));
            
        }
        else if(SnappingAxis::Z == snapping_axis)
        {
            const float x_start = mouse_pos_at_press_unprojected.x / 2.0f + 0.5f;
            const float y_start = mouse_pos_at_press_unprojected.y / 2.0f + 0.5f;

            const float x_end = current_mouse_pos_unprojected.x / 2.0f + 0.5f;
            const float y_end = current_mouse_pos_unprojected.y / 2.0f + 0.5f;

            // Min/max
            const float x_min = std::min(x_start, x_end);
            const float x_max = std::max(x_start, x_end);
            const float y_min = std::min(y_start, y_end);
            const float y_max = std::max(y_start, y_end);

            // Find new points from scale
            const float new_x_min = min_vec.x + x_min * scale_vec.x;
            const float new_x_max = min_vec.x + x_max * scale_vec.x;

            const float new_y_min = min_vec.y + y_min * scale_vec.y;
            const float new_y_max = min_vec.y + y_max * scale_vec.y;

            axes_limits_.setMin(Vec3Df(new_x_min, new_y_min, min_vec.z));
            axes_limits_.setMax(Vec3Df(new_x_max, new_y_max, max_vec.z));
        }

        should_draw_zoom_rect_ = false;
    }
    mouse_pressed_ = false;
}

void AxesInteractor::updateMouseActivity(const InteractionType interaction_type)
{
    if (interaction_type == InteractionType::PAN)
    {
        current_mouse_activity = MouseActivity::PAN;
    }
    else if (interaction_type == InteractionType::ZOOM)
    {
        current_mouse_activity = MouseActivity::ZOOM;
    }
    else if (interaction_type == InteractionType::ROTATE)
    {
        current_mouse_activity = MouseActivity::ROTATE;
    }
}

ViewAngles AxesInteractor::getViewAngles() const
{
    return view_angles_;
}

AxesLimits AxesInteractor::getAxesLimits() const
{
    return axes_limits_;
}

void AxesInteractor::showLegend(const bool show_legend)
{
    show_legend_ = show_legend;
}

void AxesInteractor::update(const InteractionType interaction_type, const int window_width, const int window_height)
{
    current_window_width = window_width;
    current_window_height = window_height;

    if (interaction_type == InteractionType::RESET)
    {
        resetView();
    }
    else
    {
        updateMouseActivity(interaction_type);
    }
}

void AxesInteractor::resetView()
{
    view_angles_ = default_view_angles_;
    axes_limits_ = default_axes_limits_;
}

void AxesInteractor::registerMouseDragInput(const MouseInteractionAxis current_mouse_interaction_axis,
                                            const int dx,
                                            const int dy)
{
    const float dx_mod = 250.0f * static_cast<float>(dx) / current_window_width;
    const float dy_mod = 250.0f * static_cast<float>(dy) / current_window_height;
    should_draw_zoom_rect_ = false;
    switch (current_mouse_activity)
    {
        case MouseActivity::ROTATE:
            changeRotation(dx_mod * rotation_mouse_gain, dy_mod * rotation_mouse_gain, current_mouse_interaction_axis);
            break;
        case MouseActivity::ZOOM:
            if(SnappingAxis::None == view_angles_.getSnappingAxis())
            {
                changeZoom(dy_mod * zoom_mouse_gain, current_mouse_interaction_axis);
            }
            else
            {
                should_draw_zoom_rect_ = true;
            }
            break;
        case MouseActivity::PAN:
            changePan(dx_mod * pan_mouse_gain, dy_mod * pan_mouse_gain, current_mouse_interaction_axis);
            break;
        default:
            break;
    }
}

void AxesInteractor::changeRotation(const double dx, const double dy, const MouseInteractionAxis mia)
{
    Vec2Dd sa(1.0, 1.0);
    if (mia == MouseInteractionAxis::X)
    {
        sa.y = 0.0;
    }
    else if (mia == MouseInteractionAxis::Y)
    {
        sa.x = 0.0;
    }
    else
    {
        sa.x = 1.0;
        sa.y = 1.0;
    }
    view_angles_.changeAnglesWithDelta(dx * sa.x, dy * sa.y);
}

double changeIncrement(const double scale, const double inc, const size_t num_lines)
{
    double new_inc = inc;
    if ((scale / inc) < static_cast<double>(num_lines) / 2.0)
    {
        new_inc = inc / 2.0;
    }
    else if ((scale / inc) > static_cast<double>(num_lines) * 1.0)
    {
        new_inc = inc * 2.0;
    }
    return new_inc;
}

void AxesInteractor::changeZoom(const double dy, const MouseInteractionAxis mia)
{
    const Vec3Dd s = axes_limits_.getAxesScale();
    const SnappingAxis snapping_axis = view_angles_.getSnappingAxis();
    Vec3Dd sa(1.0, 1.0, 1.0);

    if(SnappingAxis::None == snapping_axis)
    {
        if (mia == MouseInteractionAxis::X)
        {
            sa.y = 0.0;
            sa.z = 0.0;
        }
        else if (mia == MouseInteractionAxis::Y)
        {
            sa.x = 0.0;
            sa.z = 0.0;
        }
        else if (mia == MouseInteractionAxis::Z)
        {
            sa.x = 0.0;
            sa.y = 0.0;
        }
    }
    else
    {
        if (snapping_axis == SnappingAxis::X)
        {
            sa.x = 0.0;
        }
        else if (snapping_axis == SnappingAxis::Y)
        {
            sa.y = 0.0;
        }
        else if (snapping_axis == SnappingAxis::Z)
        {
            sa.z = 0.0;
        }
    }
    const Vec3Dd inc_vec = Vec3Dd(dy * s.x * sa.x, dy * s.y * sa.y, dy * s.z * sa.z);

    axes_limits_.setMin(axes_limits_.getMin() - inc_vec);
    axes_limits_.setMax(axes_limits_.getMax() + inc_vec);

    const size_t num_lines = axes_settings_.getNumAxesTicks();
    inc0.x = changeIncrement(s.x, inc0.x, num_lines);
    inc0.y = changeIncrement(s.y, inc0.y, num_lines);
    inc0.z = changeIncrement(s.z, inc0.z, num_lines);
}

void AxesInteractor::changePan(const double dx, const double dy, const MouseInteractionAxis mia)
{
    Vec3Dd sa(1.0, 1.0, 1.0);
    if (mia == MouseInteractionAxis::X)
    {
        sa.y = 0.0;
        sa.z = 0.0;
    }
    else if (mia == MouseInteractionAxis::Y)
    {
        sa.x = 0.0;
        sa.z = 0.0;
    }
    else if (mia == MouseInteractionAxis::Z)
    {
        sa.x = 0.0;
        sa.y = 0.0;
    }
    const Matrixd rotation_mat = view_angles_.getSnappedRotationMatrix();
    const Vec3Dd v = rotation_mat.getTranspose() * Vec3Dd(-dx, dy, 0.0);

    const Vec3Dd s = axes_limits_.getAxesScale();

    const Vec3Dd v_scaled = v.elementWiseMultiply(s).elementWiseMultiply(sa);

    axes_limits_.incrementMinMax(v_scaled);
}

// TODO: Remove?
double findFirstPointBeforeMin(const double min_point, const double start_point, const double inc)
{
    assert(inc > 0.0);
    double t0 = start_point;
    if (start_point <= min_point)
    {
        while ((t0 + inc) < min_point)
        {
            t0 = t0 + inc;
        }
    }
    else
    {
        while (t0 > min_point)
        {
            t0 = t0 - inc;
        }
    }
    return t0;
}

// TODO: Remove?
double findFirstPointInInterval(const double min_point,
                                const double max_point,
                                const double start_point,
                                const double inc)
{
    assert(min_point < max_point);
    const double t0 = findFirstPointBeforeMin(min_point, start_point, inc);
    double new_inc = inc;
    double t1 = t0;

    while (t1 < min_point)
    {
        while (t1 < min_point)
        {
            t1 = t1 + new_inc;
        }
        if (t1 > max_point)
        {
            new_inc = new_inc * 0.5;
            t1 = t0;
        }
        else if ((t1 < max_point) && ((t1 - new_inc) > min_point))
        {
            new_inc = new_inc * 0.5;
            t1 = t0;
        }
    }

    return t1;
}

void AxesInteractor::setViewAngles(const double azimuth, const double elevation)
{
    view_angles_.setAngles(azimuth, elevation);
    default_view_angles_.setAngles(azimuth, elevation);
}

void AxesInteractor::setAxesLimits(const Vec3Dd& min_vec, const Vec3Dd& max_vec)
{
    axes_limits_ = AxesLimits(min_vec, max_vec);
    default_axes_limits_ = axes_limits_;

    // TODO: Remove below?
    const size_t num_lines = axes_settings_.getNumAxesTicks();
    inc0 = 0.9999999999 * (default_axes_limits_.getMax() - default_axes_limits_.getMin()) /
           static_cast<double>(num_lines - 1);
}

void AxesInteractor::setAxesLimits(const Vec2Dd& min_vec, const Vec2Dd& max_vec)
{
    axes_limits_ =
        AxesLimits({min_vec.x, min_vec.y, axes_limits_.getMin().z}, {max_vec.x, max_vec.y, axes_limits_.getMax().z});
    default_axes_limits_ = axes_limits_;

    const size_t num_lines = axes_settings_.getNumAxesTicks();
    inc0 = 0.9999999999 * (default_axes_limits_.getMax() - default_axes_limits_.getMin()) /
           static_cast<double>(num_lines - 1);
}

GridVector generateAxisVector(const double min_val, const double max_val, const double num_lines, const double offset, GridVector& ret_vec)
{
    const double d = max_val - min_val;

    const double d_inc = std::pow(2.0, std::floor(std::log2(d / num_lines)));
    double val = min_val - std::fmod(min_val, d_inc);

    std::vector<double> vec;
    vec.reserve(num_lines);

    int it = 0;

    while (val < max_val)
    {
        if (val > min_val)
        {
            vec.push_back(val - offset);
        }
        val = val + d_inc;
        it++;
        if (it > static_cast<int>(num_lines * 3))
        {
            std::cout << "ERROR: Number of lines grew a lot!" << std::endl;
        }
    }

    assert(vec.size() <= GridVector::kMaxNumGridNumbers);
    ret_vec.num_valid_values = vec.size();
    for (size_t k = 0; k < vec.size(); k++)
    {
        ret_vec.data[k] = vec[k];
    }

    return ret_vec;
}

GridVectors AxesInteractor::generateGridVectors()
{
    GridVectors gv;

    const Vec3Dd axes_center = axes_limits_.getAxesCenter();

    const Vec3Dd v_min = (axes_limits_.getMin() - axes_center) + axes_center;
    const Vec3Dd v_max = (axes_limits_.getMax() - axes_center) + axes_center;

    generateAxisVector(v_min.x, v_max.x, axes_settings_.getNumAxesTicks(), axes_center.x, gv.x);
    generateAxisVector(v_min.y, v_max.y, axes_settings_.getNumAxesTicks(), axes_center.y, gv.y);
    generateAxisVector(v_min.z, v_max.z, axes_settings_.getNumAxesTicks(), axes_center.z, gv.z);

    return gv;
}

