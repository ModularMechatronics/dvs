#ifndef DUOPLOT_BUTTON_H
#define DUOPLOT_BUTTON_H

#include "constants.h"
#include "gui_element.h"
#include "rgbtriplet.h"

class Button : public GuiElement
{
private:
    // std::string text_;
    // std::function<void()> callback_;
    RGBTripletf color_;
    std::string label_;

public:
    Button(const float x,
           const float y,
           const float width,
           const float height,
           const std::string label,
           const RGBTripletf& color);

    /*void render() const override
    {
        // GuiElement::render();
        // Render text
    }*/
};

#endif  // DUOPLOT_BUTTON_H
