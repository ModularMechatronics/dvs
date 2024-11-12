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
    std::string label_;
    std::function<void(uint64_t)> button_pressed_callback_;
    std::function<void(uint64_t)> button_released_callback_;

    VertexBuffer vertex_buffer_2_;
    size_t num_points_to_render_;

    BufferedVector button_buffer_;

    void changeButtonPoints();

public:
    Button(const float x,
           const float y,
           const float width,
           const float height,
           const std::string label,
           const RGBTripletf& color,
           const std::function<void(uint64_t)>& button_pressed_callback,
           const std::function<void(uint64_t)>& button_released_callback);
    ~Button();
    void mousePressed(wxMouseEvent& event) override;
    void mouseReleased(wxMouseEvent& event) override;

    void render() const override;
    void updateVertexBuffer() override;
};

#endif  // DUOPLOT_BUTTON_H
