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
#include "shader.h"
#include "plot_box_walls.h"
#include "plot_box_silhouette.h"
#include "plot_box_grid.h"

class AxesRenderer
{
private:
    ViewAngles view_angles_;
    AxesLimits axes_limits_;

    AxesSettings axes_settings_;
    CoordinateConverter coord_converter_;

    Shader shader_;

    PlotBoxWalls* plot_box_walls_;
    PlotBoxSilhouette* plot_box_silhouette_;
    PlotBoxGrid* plot_box_grid_;

    float width_, height_;

    GridVectors gv_;
    bool use_perspective_proj_;

public:
    AxesRenderer() = default;
    AxesRenderer(const AxesSettings& axes_settings);

    void updateStates(const AxesLimits& axes_limits,
                      const ViewAngles& view_angles,
                      const GridVectors& gv,
                      const CoordinateConverter& coord_converter,
                      const bool use_perspective_proj);
    void render();
    void reloadShader();

};


#endif
