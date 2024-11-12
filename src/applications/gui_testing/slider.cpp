#include "slider.h"

#include <array>

using namespace duoplot;

constexpr size_t kNumTrianglesPerEdge = 10U;
constexpr size_t kNumPointsPerEdge = (kNumTrianglesPerEdge)*3U;
constexpr size_t kNumTrianglesForRectangularParts = 2U;
constexpr size_t kTotalNumPoints = kNumPointsPerEdge * 2U + 3U * kNumTrianglesForRectangularParts;

constexpr size_t kNumCircleTriangles = 20U;
constexpr size_t kNumCirclePoints = kNumCircleTriangles * 3U;

constexpr float kEdgeRadius = 10.0f;
constexpr float kCircleRadius = 7.0f;

namespace
{
const RGBTripletf kButtonColor{0x58CBDDU};
constexpr float kClickedFactor = 0.8f;
const RGBTripletf kButtonPressedColor{
    kClickedFactor * 88.0f / 255.0f, kClickedFactor * 203.0f / 255.0f, kClickedFactor * 221.0f / 255.0f};

}  // namespace

Slider::Slider(const float x,
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
               const std::function<void(const uint64_t, const int32_t)>& slider_final_value_callback)
    : GuiElement(x, y, width, height, "SLIDER", color),
      label_(label),
      slider_changed_value_callback_(slider_changed_value_callback),
      slider_final_value_callback_(slider_final_value_callback),
      set_shader_color_(set_shader_color),
      min_val_(min_val),
      max_val_(max_val),
      circle_buffer_(kNumCirclePoints * 2U),
      line_buffer_{kTotalNumPoints * 2U},
      type_(type),
      current_val_(init_val),
      vertex_buffer_line_(OGLPrimitiveType::TRIANGLES),
      vertex_buffer_circle_(OGLPrimitiveType::TRIANGLES)
{
    current_val_ = std::clamp(current_val_, min_val_, max_val_);
    height_ = kEdgeRadius * 2.0f;

    /*if (type_ == SliderType::VERTICAL)
    {
        width_ = 30.0f;
    }
    else if (type_ == SliderType::HORIZONTAL)
    {
        height_ = 30.0f;
    }
    else
    {
        throw std::runtime_error("Invalid slider type!");
    }*/

    shader_mode_ = ShaderMode::SLIDER;

    color_ = kButtonColor;

    std::memset(line_buffer_.data(), 0, kTotalNumPoints * 2U * sizeof(float));

    updatePoints();
    updateCirclePoints();

    vertex_buffer_line_.addBuffer(line_buffer_.data(), num_line_points_to_render_, 2, GL_DYNAMIC_DRAW);
    vertex_buffer_circle_.addBuffer(circle_buffer_.data(), num_circle_points_to_render_, 2, GL_DYNAMIC_DRAW);
}

Slider::~Slider() {}

void Slider::updateCirclePoints()
{
    circle_buffer_.reset();
    const float x_offset = static_cast<float>(current_val_ - min_val_) / static_cast<float>(max_val_ - min_val_);

    PutCircleIntoBuffer(circle_buffer_,
                        x_ + kEdgeRadius + (width_ - kEdgeRadius * 2.0f) * x_offset,
                        y_ + height_ / 2.0f,
                        kCircleRadius,
                        kNumCircleTriangles);

    num_circle_points_to_render_ = circle_buffer_.idx() / 2U;
}

void Slider::ChangePositionOrSize(const wxPoint delta_vec, const ChangeDirection change_direction)
{
    const float x_before = x_;
    const float y_before = y_;
    const float width_before = width_;
    const float height_before = height_;

    switch (change_direction)
    {
        case ChangeDirection::UP:
            // height_ -= delta_vec.y;
            // y_ += delta_vec.y;
            break;
        case ChangeDirection::DOWN:
            // height_ += delta_vec.y;
            break;
        case ChangeDirection::LEFT:
            x_ += delta_vec.x;
            width_ -= delta_vec.x;
            break;
        case ChangeDirection::RIGHT:
            width_ += delta_vec.x;
            break;
        case ChangeDirection::LEFT_UP:
            // height_ -= delta_vec.y;
            // y_ += delta_vec.y;
            x_ += delta_vec.x;
            width_ -= delta_vec.x;
            break;
        case ChangeDirection::LEFT_DOWN:
            // height_ += delta_vec.y;
            x_ += delta_vec.x;
            width_ -= delta_vec.x;
            break;
        case ChangeDirection::RIGHT_UP:
            // height_ -= delta_vec.y;
            // y_ += delta_vec.y;
            width_ += delta_vec.x;
            break;
        case ChangeDirection::RIGHT_DOWN:
            // height_ += delta_vec.y;
            width_ += delta_vec.x;
            break;
        case ChangeDirection::MIDDLE:
            x_ += delta_vec.x;
            y_ += delta_vec.y;
            break;
        default:
            assert(false && "Shouldn't end up here!");
            break;
    }

    if (width_ < kMinWidth)
    {
        width_ = kMinWidth;
        x_ = x_before;
    }
    if (height_ < kMinHeight)
    {
        height_ = kMinHeight;
        y_ = y_before;
    }

    updateVertexBuffer();
}

void Slider::updatePoints()
{
    line_buffer_.reset();

    PutRectangleIntoBuffer(line_buffer_, x_ + kEdgeRadius, y_, width_ - 2.0f * kEdgeRadius, kEdgeRadius * 2.0f);

    PutCircleSegmentIntoBuffer(line_buffer_,
                               x_ + width_ - kEdgeRadius,
                               y_ + height_ / 2.0f,
                               kEdgeRadius,
                               -M_PI / 2.0f,
                               M_PI / 2.0f,
                               kNumTrianglesPerEdge);

    PutCircleSegmentIntoBuffer(line_buffer_,
                               x_ + kEdgeRadius,
                               y_ + height_ / 2.0f,
                               kEdgeRadius,
                               M_PI / 2.0f,
                               3.0f * M_PI / 2.0f,
                               kNumTrianglesPerEdge);

    num_line_points_to_render_ = line_buffer_.idx() / 2U;
}

void Slider::mouseDragged(wxMouseEvent& event, const wxPoint& delta_vec)
{
    const wxPoint pos = event.GetPosition();
    const float normalized_pos =
        std::clamp(static_cast<float>(pos.x - x_ - kEdgeRadius) / (width_ - kEdgeRadius * 2.0f), 0.0f, 1.0f);

    const int32_t new_val = static_cast<int32_t>(normalized_pos * static_cast<float>(max_val_ - min_val_)) + min_val_;
    if (new_val == current_val_)
    {
        return;
    }
    current_val_ = new_val;

    if (slider_changed_value_callback_)
    {
        slider_changed_value_callback_(id_, current_val_);
    }

    updateCirclePoints();
    vertex_buffer_circle_.updateBufferData(0, circle_buffer_.data(), num_circle_points_to_render_, 2);
}

void Slider::mousePressed(wxMouseEvent& event)
{
    const wxPoint pos = event.GetPosition();
    const float normalized_pos =
        std::clamp(static_cast<float>(pos.x - x_ - kEdgeRadius) / (width_ - kEdgeRadius * 2.0f), 0.0f, 1.0f);

    const int32_t new_val = static_cast<int32_t>(normalized_pos * static_cast<float>(max_val_ - min_val_)) + min_val_;
    if (new_val == current_val_)
    {
        return;
    }
    current_val_ = new_val;

    if (slider_changed_value_callback_)
    {
        slider_changed_value_callback_(id_, current_val_);
    }

    updateCirclePoints();
    vertex_buffer_circle_.updateBufferData(0, circle_buffer_.data(), num_circle_points_to_render_, 2);
}

void Slider::mouseReleased(wxMouseEvent& event)
{
    if (slider_final_value_callback_)
    {
        slider_final_value_callback_(id_, current_val_);
    }
}

void Slider::updateVertexBuffer()
{
    updatePoints();
    updateCirclePoints();

    vertex_buffer_line_.updateBufferData(0, line_buffer_.data(), num_line_points_to_render_, 2);
    vertex_buffer_circle_.updateBufferData(0, circle_buffer_.data(), num_circle_points_to_render_, 2);
}

void Slider::render() const
{
    set_shader_color_(color_);
    vertex_buffer_line_.render(num_line_points_to_render_);

    set_shader_color_(RGBTripletf(1.0f, 1.0f, 1.0f));
    vertex_buffer_circle_.render(num_circle_points_to_render_);
}
