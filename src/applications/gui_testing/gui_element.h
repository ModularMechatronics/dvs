#ifndef GUI_ELEMENT_H
#define GUI_ELEMENT_H

#include "vertex_buffer.h"

class GuiElement
{
private:
    float x;
    float y;
    float width;
    float height;

    VertexBuffer vertex_buffer_;
public:
    GuiElement() = default;
    GuiElement(const float x, const float y, const float width, const float height) : x{x}, y{y}, width{width}, height{height}, vertex_buffer_{OGLPrimitiveType::TRIANGLES} 
    {
        const float vertices[] = {
            x, y,
            x + width, y,
            x + width, y + height,
            x, y,
            x + width, y + height,
            x, y + height
        };

        vertex_buffer_.addBuffer(vertices, 4, 2);
    }

    void render() const
    {
        vertex_buffer_.render(6);
    }
};

#endif
