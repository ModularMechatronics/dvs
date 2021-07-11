#ifndef PLOT_BOX_COORDINATE_ARROWS_H_
#define PLOT_BOX_COORDINATE_ARROWS_H_

#include "axes/structures/axes_settings.h"
#include "axes/structures/coordinate_converter.h"
#include "axes/structures/view_angles.h"
#include "math/math.h"

void drawCoordinateAxesLines(const AxesSettings& axes_settings);
void drawCoordinateArrowHead(const AxesSettings& axes_settings, const CoordinateConverter& coord_conv);
void drawCoordinateAxesLetters(const AxesSettings& axes_settings, const CoordinateConverter& coord_conv);

#endif
