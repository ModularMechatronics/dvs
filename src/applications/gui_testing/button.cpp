#include "button.h"

Button::Button(const float x,
               const float y,
               const float width,
               const float height,
               const std::string label,
               const RGBTripletf& color)
    : GuiElement(x, y, width, height, "BUTTON", 0U, color)
{
}
