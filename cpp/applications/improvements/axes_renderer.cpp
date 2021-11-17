#include "axes_renderer.h"

AxesRenderer::AxesRenderer(const AxesSettings& axes_settings) : axes_settings_(axes_settings)
{

}

void AxesRenderer::render()
{

}

void AxesRenderer::updateStates(const AxesLimits& axes_limits,
                                const ViewAngles& view_angles,
                                const GridVectors& gv,
                                const CoordinateConverter& coord_converter)
{
    axes_limits_ = axes_limits;
    view_angles_ = view_angles;
    gv_ = gv;
    coord_converter_ = coord_converter;
    // const Vec3Dd new_scale = findScale(view_angles_.getSnappedRotationMatrix());
    // axes_settings_.setAxesScale(new_scale);
}