#ifndef TEXT_RENDERING_NEW_H_
#define TEXT_RENDERING_NEW_H_

#include <glm/glm.hpp>

#include <string>

#include "opengl_low_level/opengl_low_level.h"

bool initFreetype();
void renderText(GLuint shader_id, std::string text, float x, float y, float scale, const float width, const float height, glm::vec3 color);

#endif
