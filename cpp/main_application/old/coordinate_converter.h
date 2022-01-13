#ifndef COORDINATE_CONVERTER_H_
#define COORDINATE_CONVERTER_H_

#include "axes/structures/axes_limits.h"
#include "axes/structures/view_angles.h"
#include "dvs/math/math.h"

// Model - view - screen
// Model: 3D coordinates, world, defined with axes_limits_
// View: 2D gl world coordinates, defined as [-1, -1] in bottom left corner
//       and [1, 1] in top right corner
// Screen: Window pixels, defined as [0, 0] in top left corner

using namespace dvs;

struct CoordinateConverter
{
private:
    Vec2Dd window_size_;
    ViewAngles view_angles_;
    AxesLimits axes_limits_;

public:
    CoordinateConverter() = default;
    CoordinateConverter(const Vec2Dd& window_size, const ViewAngles& view_angles, const AxesLimits& axes_limits);
    void updateInternalState(const Vec2Dd& window_size, const ViewAngles& view_angles, const AxesLimits& axes_limits);

    Vec2Dd orthogonalViewToModelCoordinate(const Vec2Dd& view_coord) const;
    Vec2Dd screenToViewCoordinate(const Vec2Dd& screen_coord) const;
    Vec2Dd viewToScreenCoordinate(const Vec2Dd& view_coord) const;
    Vec2Dd modelToViewCoordinate(const Vec3Dd& model_coord) const;
};

#endif
