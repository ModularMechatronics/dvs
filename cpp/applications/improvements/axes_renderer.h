#ifndef DVS_AXES_RENDERER_H_
#define DVS_AXES_RENDERER_H_

#include <cmath>
#include <utility>
#include <vector>

#include "axes/structures/axes_limits.h"
#include "axes/structures/axes_settings.h"
#include "axes/structures/coordinate_converter.h"
#include "axes/structures/grid_vectors.h"
#include "axes/structures/view_angles.h"
#include "opengl_low_level/opengl_low_level.h"

class AxesRenderer
{
private:
    ViewAngles view_angles_;
    AxesLimits axes_limits_;

    AxesSettings axes_settings_;
    CoordinateConverter coord_converter_;

    float width_, height_;

    GridVectors gv_;

public:
    AxesRenderer() = default;
    AxesRenderer(const AxesSettings& axes_settings);

};


#endif
