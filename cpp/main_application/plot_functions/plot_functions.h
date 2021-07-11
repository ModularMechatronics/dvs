#ifndef PLOT_FUNCTIONS_H_
#define PLOT_FUNCTIONS_H_

#include <utility>

#include "math/math.h"
#include "misc/color_map.h"
#include "opengl_low_level/data_structures.h"

using namespace dvs;

void plot(const Vectord& x, const Vectord& y);
void plot3(const Vectord& x, const Vectord& y, const Vectord& z);
void scatter3(const Vectord& x, const Vectord& y, const Vectord& z);
void scatter(const Vectord& x, const Vectord& y);

void drawGrid3D(const Matrixd& x, const Matrixd& y, const Matrixd& z);
void surfInternal(const Matrixd& x,
                  const Matrixd& y,
                  const Matrixd& z,
                  const Interval1D<double> min_max_interval,
                  RGBColorMap<float> c_map);
void surf(const Matrixd& x, const Matrixd& y, const Matrixd& z);
void drawArrow3D(const Point3Dd& p, const Vec3Dd& v);
void setSettings(const std::vector<std::pair<std::string, std::string>>& settings_vector);

#endif
