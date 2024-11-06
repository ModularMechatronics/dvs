#include "slider.h"

#include <array>

using namespace duoplot;

constexpr size_t kNumTrianglesPerEdge = 10U;
constexpr size_t kNumPointsPerEdge = (kNumTrianglesPerEdge)*3U;
constexpr size_t kNumTrianglesForRectangularParts = 2U;
constexpr size_t kTotalNumPoints = kNumPointsPerEdge * 2U + 3U * kNumTrianglesForRectangularParts;

constexpr size_t kNumCircleTriangles = 20U;
constexpr size_t kNumCirclePoints = kNumCircleTriangles * 3U;

constexpr float kEdgeRadius = 3.0f;
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
      type_(type),
      current_val_(init_val),
      vertex_buffer_line_(OGLPrimitiveType::TRIANGLES),
      vertex_buffer_circle_(OGLPrimitiveType::TRIANGLES)
{
    current_val_ = std::clamp(current_val_, min_val_, max_val_);

    if (type_ == SliderType::VERTICAL)
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
    }

    line_points_ = new float[kTotalNumPoints * 2U];
    circle_points_ = new float[kNumCirclePoints * 2U];

    shader_mode_ = ShaderMode::SLIDER;

    color_ = kButtonColor;

    std::memset(line_points_, 0, kTotalNumPoints * 2U * sizeof(float));

    updatePoints();
    updateCirclePoints();

    vertex_buffer_line_.addBuffer(line_points_, num_line_points_to_render_, 2, GL_DYNAMIC_DRAW);
    vertex_buffer_circle_.addBuffer(circle_points_, num_circle_points_to_render_, 2, GL_DYNAMIC_DRAW);
}

Slider::~Slider() {}

void Slider::updateCirclePoints()
{
    const float x_offset = static_cast<float>(current_val_ - min_val_) / static_cast<float>(max_val_ - min_val_);

    for (size_t i = 0; i < kNumCircleTriangles; ++i)
    {
        const float theta0 = 2.0f * M_PI * static_cast<float>(i) / static_cast<float>(kNumCircleTriangles);
        const float theta1 = 2.0f * M_PI * static_cast<float>(i + 1) / static_cast<float>(kNumCircleTriangles);

        const float x0 = kCircleRadius * std::cos(theta0);
        const float y0 = kCircleRadius * std::sin(theta0);

        const float x1 = kCircleRadius * std::cos(theta1);
        const float y1 = kCircleRadius * std::sin(theta1);

        circle_points_[i * 6] = x_ + width_ * x_offset + x0;
        circle_points_[i * 6 + 1] = y_ + height_ / 2.0f + kEdgeRadius + y0;

        circle_points_[i * 6 + 2] = x_ + width_ * x_offset;
        circle_points_[i * 6 + 3] = y_ + height_ / 2.0f + kEdgeRadius;

        circle_points_[i * 6 + 4] = x_ + width_ * x_offset + x1;
        circle_points_[i * 6 + 5] = y_ + height_ / 2.0f + kEdgeRadius + y1;
    }

    num_circle_points_to_render_ = kNumCirclePoints;
}

void Slider::updatePoints()
{
    size_t idx = 0U;

    const auto set_points = [this, &idx](
                                const float x_left, const float x_right, const float y_top, const float y_bottom) {
        const Vec2f bottom_left{x_ + x_left, y_ + height_ / 2.0f + y_bottom};
        const Vec2f bottom_right{x_ + x_right, y_ + height_ / 2.0f + y_bottom};

        const Vec2f top_left{x_ + x_left, y_ + height_ / 2.0f + y_top};
        const Vec2f top_right{x_ + x_right, y_ + height_ / 2.0f + y_top};

        using Triangle = std::array<Vec2f, 3>;

        const std::array<Triangle, 2> triangles{Triangle{top_left, top_right, bottom_left},
                                                Triangle{bottom_left, top_right, bottom_right}};

        for (const auto& corner_points : triangles)
        {
            for (const auto& corner_point : corner_points)
            {
                line_points_[idx] = corner_point.x;
                line_points_[idx + 1U] = corner_point.y;
                idx += 2U;
            }
        }
    };

    set_points(kEdgeRadius, width_ - kEdgeRadius, 0.0f, kEdgeRadius * 2.0f);

    constexpr float kAngleInc = M_PI / (static_cast<float>(kNumTrianglesPerEdge));

    const auto add_corner_triangles =
        [this, &idx](const float x_center, const float y_center, const float theta0, const float angle_inc) -> void {
        float theta = theta0;
        for (size_t k = 0; k < kNumTrianglesPerEdge; ++k)
        {
            line_points_[idx] = x_center;
            line_points_[idx + 1U] = y_center;

            const float x_pt = kEdgeRadius * std::cos(theta);
            const float y_pt = kEdgeRadius * std::sin(theta);

            line_points_[idx + 2U] = x_center + x_pt;
            line_points_[idx + 3U] = y_center + y_pt;

            theta += angle_inc;
            const float x_pt_next = kEdgeRadius * std::cos(theta);
            const float y_pt_next = kEdgeRadius * std::sin(theta);

            line_points_[idx + 4U] = x_center + x_pt_next;
            line_points_[idx + 5U] = y_center + y_pt_next;

            idx += 6U;
        }
    };

    add_corner_triangles(x_ + kEdgeRadius, y_ + height_ / 2.0f + kEdgeRadius, M_PI / 2.0f, kAngleInc);
    add_corner_triangles(x_ + width_ - kEdgeRadius, y_ + height_ / 2.0f + kEdgeRadius, -M_PI / 2.0f, kAngleInc);

    num_line_points_to_render_ = idx / 2U;
}

void Slider::mouseDragged(wxMouseEvent& event, const wxPoint& delta_vec)
{
    const wxPoint pos = event.GetPosition();
    const float normalized_pos = std::clamp(static_cast<float>(pos.x - x_) / width_, 0.0f, 1.0f);

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
    vertex_buffer_circle_.updateBufferData(0, circle_points_, num_circle_points_to_render_, 2);
}

void Slider::mousePressed(wxMouseEvent& event)
{
    const wxPoint pos = event.GetPosition();
    const float normalized_pos = std::clamp(static_cast<float>(pos.x - x_) / width_, 0.0f, 1.0f);

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
    vertex_buffer_circle_.updateBufferData(0, circle_points_, num_circle_points_to_render_, 2);
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

    vertex_buffer_line_.updateBufferData(0, line_points_, num_line_points_to_render_, 2);
    vertex_buffer_circle_.updateBufferData(0, circle_points_, num_circle_points_to_render_, 2);
}

void Slider::render() const
{
    set_shader_color_(color_);
    vertex_buffer_line_.render(num_line_points_to_render_);

    set_shader_color_(RGBTripletf(1.0f, 1.0f, 1.0f));
    vertex_buffer_circle_.render(num_circle_points_to_render_);
}
