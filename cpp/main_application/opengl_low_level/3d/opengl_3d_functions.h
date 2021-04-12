#ifndef OPENGL_3D_FUNCTIONS_
#define OPENGL_3D_FUNCTIONS_

#include "math/math.h"

#include <cstddef>
#include <vector>

#include "opengl_low_level/data_structures.h"

using namespace dvs;

void drawLines3D(const Vectord& x_values,
                 const Vectord& y_values,
                 const Vectord& z_values);
void drawTriangle3D(const Vec3Dd& v0, const Vec3Dd& v1, const Vec3Dd& v2);
void drawLine3D(const double x0,
                const double y0,
                const double z0,
                const double x1,
                const double y1,
                const double z1);
void drawLine3D(const Vec3Dd& v0, const Vec3Dd& v1);
void drawPolygon3D(const Vec3Dd& c0,
                   const Vec3Dd& c1,
                   const Vec3Dd& c2,
                   const Vec3Dd& c3);
void drawPoints3D(const Vectord& x_values,
                  const Vectord& y_values,
                  const Vectord& z_values);
#endif
