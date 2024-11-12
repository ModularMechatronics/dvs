#ifndef DUOPLOT_SLIDER_H
#define DUOPLOT_SLIDER_H

#include "constants.h"
#include "gui_element.h"
#include "rgbtriplet.h"

class Slider : public GuiElement
{
public:
    enum class SliderType
    {
        HORIZONTAL,
        VERTICAL
    };

private:
    std::string label_;
    std::function<void(const uint64_t, const int32_t)> slider_changed_value_callback_;
    std::function<void(const uint64_t, const int32_t)> slider_final_value_callback_;

    VertexBuffer vertex_buffer_line_;
    VertexBuffer vertex_buffer_circle_;
    size_t num_line_points_to_render_;
    size_t num_circle_points_to_render_;

    BufferedVector line_buffer_;
    BufferedVector circle_buffer_;

    int32_t min_val_;
    int32_t max_val_;
    int32_t current_val_;
    SliderType type_;

    std::function<void(RGBTripletf)> set_shader_color_;

    void updatePoints();
    void updateCirclePoints();

public:
    Slider(const float x,
           const float y,
           const float width,
           const float height,
           const int32_t min_val,
           const int32_t max_val,
           const int32_t init_val,
           const SliderType type,
           const std::function<void(RGBTripletf)> set_shader_color,
           const std::string label,
           const RGBTripletf& color,
           const std::function<void(const uint64_t, const int32_t)>& slider_changed_value_callback,
           const std::function<void(const uint64_t, const int32_t)>& slider_final_value_callback);
    ~Slider();
    void mousePressed(wxMouseEvent& event) override;
    void mouseReleased(wxMouseEvent& event) override;
    void mouseDragged(wxMouseEvent& event, const wxPoint& delta_vec) override;
    void ChangePositionOrSize(const wxPoint delta_vec, const ChangeDirection change_direction) override;

    void render() const override;
    void updateVertexBuffer() override;
};

#endif  // DUOPLOT_SLIDER_H
