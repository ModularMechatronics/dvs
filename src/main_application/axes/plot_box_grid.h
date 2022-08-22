#ifndef MAIN_APPLICATION_AXES_PLOT_BOX_GRID_H_
#define MAIN_APPLICATION_AXES_PLOT_BOX_GRID_H_

#include <stddef.h>

#include "opengl_low_level/opengl_header.h"
#include "axes/structures/axes_limits.h"
#include "axes/structures/axes_settings.h"
#include "axes/structures/grid_vectors.h"
#include "axes/structures/view_angles.h"
#include "dvs/math/math.h"


class PlotBoxGrid
{
private:
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;
    float* grid_points_;
    GLfloat* color_;
    size_t idx_;
    float azimuth_;
    float elevation_;

    Vec3d axes_scale_;

    void renderXYGrid(const GridVectors& gv);
    void renderXZGrid(const GridVectors& gv);
    void renderYZGrid(const GridVectors& gv);

public:
    PlotBoxGrid() = delete;
    PlotBoxGrid(const float size);
    ~PlotBoxGrid();

    void render(const GridVectors& gv,
                const AxesLimits& axes_limits,
                const ViewAngles& view_angles);
};

#endif // MAIN_APPLICATION_AXES_PLOT_BOX_GRID_H_
