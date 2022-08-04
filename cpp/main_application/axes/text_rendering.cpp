
#include "text_rendering.h"

#include <iostream>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "opengl_low_level/opengl_header.h"

using namespace dvs;

namespace
{
struct Character
{
    unsigned int texture_id;
    glm::ivec2   size;
    glm::ivec2   bearing;
    unsigned int increment;
};

std::map<GLchar, Character> characters;

bool is_initialized = false;

const char* const kFontPath = "../resources/fonts/Roboto-Regular.ttf";

constexpr float kTextScaleParameter = 1000.0f;

}

TextRenderer::TextRenderer()
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TextRenderer::renderTextFromCenter(const std::string_view& text, float x, float y, float scale, const float axes_width, const float axes_height) const
{
    const Vec2Df text_size = calculateStringSize(text, scale, axes_width, axes_height);
    this->renderTextFromLeftCenter(text, x - text_size.x / 2.0f, y, scale, axes_width, axes_height);
}

void TextRenderer::renderTextFromRightCenter(const std::string_view& text, float x, float y, float scale, const float axes_width, const float axes_height) const
{
    const Vec2Df text_size = calculateStringSize(text, scale, axes_width, axes_height);
    this->renderTextFromLeftCenter(text, x - text_size.x, y, scale, axes_width, axes_height);
}

void TextRenderer::renderTextFromLeftCenter(const std::string_view& text, float x, float y, float scale, const float axes_width, const float axes_height) const
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    const Vec2Df text_size = calculateStringSize(text, scale, axes_width, axes_height);
    y -= text_size.y / 2.0f;

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(vao);

    const float sx = kTextScaleParameter / axes_width;
    const float sy = kTextScaleParameter / axes_height;

    for (size_t k = 0; k < text.length(); k++) 
    {
        const Character& ch = characters[text[k]];

        const float xpos = x + ch.bearing.x * scale;
        const float ypos = y - (ch.size.y - ch.bearing.y) * scale;

        const float w = ch.size.x * scale * sx;
        const float h = ch.size.y * scale * sy;

        const float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },            
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }           
        };

        glBindTexture(GL_TEXTURE_2D, ch.texture_id);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.increment >> 6) * scale * sx;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
}


bool initFreetype()
{
    if(is_initialized)
    {
        // return true;
    }

    FT_Library ft;

    if (FT_Init_FreeType(&ft))
    {
        std::cout << "Could not initialize font library." << std::endl;
        return false;
    }

    FT_Face face;
    if (FT_New_Face(ft, kFontPath, 0, &face))
    {
        std::cout << "Failed to load font" << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face, 0, 68);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "Could not load glyph '" << c << "'" << std::endl;
            continue;
        }

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // GL_CLAMP_TO_BORDER or GL_CLAMP_TO_EDGE?
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    is_initialized = true;

    return true;
}

// TODO: This is incorrect, it shouldn't depend on width/height. Maybe it
// calculates the height/width in the local "compressed" axes window?
Vec2Df calculateStringSize(const std::string_view& text, const float scale, const float axes_width, const float axes_height)
{
    if(text.length() == 0)
    {
        return Vec2Df(0.0f, 0.0f);
    }

    float x = 0.0f;

    const float sx = kTextScaleParameter / axes_width;
    const float sy = kTextScaleParameter / axes_height;

    float x_min, x_max, y_min, y_max;

    Character ch = characters[text[0]];

    x_min = ch.bearing.x * scale;
    x_max = x_min;
    y_min = -(ch.size.y - ch.bearing.y) * scale;
    y_max = y_min;

    for (size_t k = 0; k < text.length(); k++) 
    {
        const Character& ch = characters[text[k]];

        const float xpos = x + ch.bearing.x * scale;
        const float ypos = -(ch.size.y - ch.bearing.y) * scale;

        const float w = ch.size.x * scale * sx;
        const float h = ch.size.y * scale * sy;

        x_min = std::min(x_min, xpos);
        x_min = std::min(x_min, xpos + w);

        x_max = std::max(x_min, xpos);
        x_max = std::max(x_min, xpos + w);

        y_min = std::min(y_min, ypos);
        y_min = std::min(y_min, ypos + h);

        y_max = std::max(y_min, ypos);
        y_max = std::max(y_min, ypos + h);

        x += (ch.increment >> 6) * scale * sx;
    }
    
    return Vec2Df(x_max - x_min, y_max - y_min);
}
