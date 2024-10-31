#include "button.h"

Button::Button(const float x,
               const float y,
               const float width,
               const float height,
               const std::string label,
               const RGBTripletf& color,
               const std::function<void(uint64_t)>& button_pressed_callback,
               const std::function<void(uint64_t)>& button_released_callback)
    : GuiElement(x, y, width, height, "BUTTON", color),
      label_(label),
      button_pressed_callback_(button_pressed_callback),
      button_released_callback_(button_released_callback),
      vertex_buffer_2_(OGLPrimitiveType::TRIANGLES)
{
    const float vertices[] = {// First triangle
                              x_,
                              y_,
                              x_ + width_,
                              y_,
                              x_ + width_,
                              y_ + height_,
                              // Second triangle
                              x_,
                              y_,
                              x_,
                              y_ + height_,
                              x_ + width_,
                              y_ + height_};
    // clang-format on

    vertex_buffer_2_.addBuffer(vertices, 6, 2, GL_DYNAMIC_DRAW);
}

Button::~Button() {}

void Button::mousePressed(wxMouseEvent& event)
{
    color_.green = 0.0f;
    color_.blue = 0.0f;
    if (button_pressed_callback_)
    {
        button_pressed_callback_(id_);
    }
}

void Button::mouseReleased(wxMouseEvent& event)
{
    color_.green = 1.0f;
    color_.blue = 1.0f;
    if (button_released_callback_)
    {
        button_released_callback_(id_);
    }
}

void Button::updateVertexBuffer()
{
    // clang-format off
    const float vertices[] = {// First triangle
                              x_ , y_,
                              x_ + width_, y_,
                              x_ + width_, y_ + height_,
                              // Second triangle
                              x_, y_,
                              x_, y_ + height_,
                              x_ + width_, y_ + height_};
    // clang-format on

    vertex_buffer_2_.updateBufferData(0, vertices, 6, 2);
}

void Button::render() const
{
    vertex_buffer_2_.render(6);
}
