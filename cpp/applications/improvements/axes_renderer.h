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
#include "plot_box_walls.h"
#include "plot_box_silhouette.h"

class AxesRenderer
{
private:
    ViewAngles view_angles_;
    AxesLimits axes_limits_;

    AxesSettings axes_settings_;
    CoordinateConverter coord_converter_;

    PlotBoxWalls* plot_box_walls_;
    PlotBoxSilhouette* plot_box_silhouette_;

    float width_, height_;

    GridVectors gv_;

public:
    AxesRenderer() = default;
    AxesRenderer(const AxesSettings& axes_settings);

    void updateStates(const AxesLimits& axes_limits,
                      const ViewAngles& view_angles,
                      const GridVectors& gv,
                      const CoordinateConverter& coord_converter);
    void render();

};


#endif
