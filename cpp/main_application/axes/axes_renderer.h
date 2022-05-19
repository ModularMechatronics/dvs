#ifndef DVS_AXES_RENDERER_H_
#define DVS_AXES_RENDERER_H_

#include <cmath>
#include <utility>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "axes/text_rendering.h"
#include "axes/structures/axes_limits.h"
#include "axes/structures/axes_settings.h"
#include "axes/structures/grid_vectors.h"
#include "axes/structures/view_angles.h"
#include "opengl_low_level/opengl_low_level.h"
#include "shader.h"
#include "axes/plot_box_walls.h"
#include "axes/plot_box_silhouette.h"
#include "axes/plot_box_grid.h"
#include "axes/grid_numbers.h"
#include "axes/zoom_rect.h"
#include "io_devices/mouse_state.h"

class AxesRenderer
{
private:
    ViewAngles view_angles_;
    AxesLimits axes_limits_;

    AxesSettings axes_settings_;

    TextRenderer text_renderer_;

    ShaderCollection shader_collection_;

    PlotBoxWalls* plot_box_walls_;
    PlotBoxSilhouette* plot_box_silhouette_;
    PlotBoxGrid* plot_box_grid_;

    glm::mat4 orth_projection_mat;
    glm::mat4 persp_projection_mat;
    glm::mat4 projection_mat;
    glm::mat4 view_mat;
    glm::mat4 model_mat;
    glm::mat4 scale_mat;
    glm::mat4 window_scale_mat_;

    Matrix<double> rot_mat;

    float width_, height_;
    Vec3Dd scale_for_window_;

    GridVectors gv_;
    bool use_perspective_proj_;
    Vec2Df mouse_pos_at_press_;
    Vec2Df current_mouse_pos_;
    ZoomRect zoom_rect_;
    MouseActivity mouse_activity_;
    bool mouse_pressed_;

    void renderPlotBox();
    void renderBoxGrid();
    void enableClipPlanes();
    void setClipPlane(const GLuint program_id, const std::string pln, const Point3Dd& p0, const Point3Dd& p1, const Point3Dd& p2, const bool invert) const;

public:
    AxesRenderer(const ShaderCollection shader_collection);

    void updateStates(const AxesLimits& axes_limits,
                      const ViewAngles& view_angles,
                      const GridVectors& gv,
                      const bool use_perspective_proj,
                      const float width,
                      const float height,
                      const Vec2Df mouse_pos_at_press,
                      const Vec2Df current_mouse_pos,
                      const MouseActivity mouse_activity,
                      const bool mouse_pressed);
    void render();
    void plotBegin();
    void plotEnd();
};


#endif
