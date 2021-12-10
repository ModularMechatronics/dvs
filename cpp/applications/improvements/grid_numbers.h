#ifndef GRID_NUMBERS_H_
#define GRID_NUMBERS_H_

#include <stddef.h>

#include "opengl_low_level/opengl_header.h"
#include "axes/structures/axes_limits.h"
#include "axes/structures/axes_settings.h"
#include "axes/structures/grid_vectors.h"
#include "axes/structures/view_angles.h"
#include "axes/structures/coordinate_converter.h"
#include "dvs/math/math.h"


class PlotBoxGridNumbers
{
private:
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;
    float* grid_points_;
    GLfloat* color_;
    size_t idx_;
    float azimuth_;
    float elevation_;

    Vec3Dd axes_scale_;


public:
    PlotBoxGridNumbers() = delete;
    PlotBoxGridNumbers(const float size);
    ~PlotBoxGridNumbers();

    void render(const GridVectors& gv,
                const AxesSettings& axes_settings,
                const AxesLimits& axes_limits,
                const ViewAngles& view_angles,
                const CoordinateConverter& coord_converter,
                const float width,
                const float height);
};


#endif
