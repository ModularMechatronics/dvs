#ifndef TEXT_RENDERING_NEW_H_
#define TEXT_RENDERING_NEW_H_

#include <glm/glm.hpp>

#include <string>

#include "opengl_low_level/opengl_low_level.h"

bool initFreetype();
void renderText(GLuint shader_id, std::string text, float x, float y, float scale, const float axes_width, const float axes_height, glm::vec3 color);
Vec2Df calculateStringSize(std::string text, float x, float y, float scale, const float axes_width, const float axes_height);

#endif
