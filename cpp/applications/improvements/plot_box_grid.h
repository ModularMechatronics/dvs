#ifndef PLOT_BOX_GRID_H_
#define PLOT_BOX_GRID_H_

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
    size_t num_vertices_;
public:
    PlotBoxGrid() = delete;
    PlotBoxGrid(const float size);
    ~PlotBoxGrid();

    void render(const GridVectors& gv,
                const AxesSettings& axes_settings,
                const AxesLimits& axes_limits,
                const ViewAngles& view_angles);
};

#endif
