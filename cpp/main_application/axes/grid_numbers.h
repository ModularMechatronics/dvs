#ifndef GRID_NUMBERS_H_
#define GRID_NUMBERS_H_

#include <stddef.h>

#include "opengl_low_level/opengl_header.h"
#include "axes/structures/axes_limits.h"
#include "axes/structures/axes_settings.h"
#include "axes/structures/grid_vectors.h"
#include "axes/structures/view_angles.h"
#include "shader.h"
#include "dvs/math/math.h"

#include <glm/mat4x4.hpp>

void drawGridNumbers(const Shader text_shader, const AxesLimits& axes_limits, const ViewAngles& view_angles, const glm::mat4& view_mat, const glm::mat4& model_mat, const glm::mat4& projection_mat, const float width, const float height, const GridVectors& gv);

#endif
