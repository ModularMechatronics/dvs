#include "button.h"

#include <array>

using namespace duoplot;

constexpr size_t kNumTrianglesPerCorner = 10U;
constexpr size_t kNumPointsPerCorner = (kNumTrianglesPerCorner)*3U;
constexpr size_t kNumTrianglesForRectangularParts = 2U * 3U;
constexpr size_t kTotalNumPoints = kNumPointsPerCorner * 4U + 3U * kNumTrianglesForRectangularParts;

constexpr float kCornerRadius = 3.0f;
namespace
{
const RGBTripletf kButtonColor{0x58CBDDU};
constexpr float kClickedFactor = 0.8f;
const RGBTripletf kButtonPressedColor{
    kClickedFactor * 88.0f / 255.0f, kClickedFactor * 203.0f / 255.0f, kClickedFactor * 221.0f / 255.0f};

}  // namespace

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
    points_ = new float[kTotalNumPoints * 2U];
    shader_mode_ = ShaderMode::BUTTON;
    color_ = kButtonColor;

    std::memset(points_, 0, kTotalNumPoints * 2U * sizeof(float));

    changeButtonPoints();

    vertex_buffer_2_.addBuffer(points_, num_points_to_render_, 2, GL_DYNAMIC_DRAW);
}

Button::~Button() {}

void Button::changeButtonPoints()
{
    size_t idx = 0U;

    const auto set_points = [this, &idx](
                                const float x_left, const float x_right, const float y_top, const float y_bottom) {
        const Vec2f bottom_left{x_ + x_left, y_ + y_bottom};
        const Vec2f bottom_right{x_ + x_right, y_ + y_bottom};

        const Vec2f top_left{x_ + x_left, y_ + y_top};
        const Vec2f top_right{x_ + x_right, y_ + y_top};

        using Triangle = std::array<Vec2f, 3>;

        const std::array<Triangle, 2> triangles{Triangle{top_left, top_right, bottom_left},
                                                Triangle{bottom_left, top_right, bottom_right}};

        for (const auto& corner_points : triangles)
        {
            for (const auto& corner_point : corner_points)
            {
                points_[idx] = corner_point.x;
                points_[idx + 1U] = corner_point.y;
                idx += 2U;
            }
        }
    };

    set_points(kCornerRadius, width_ - kCornerRadius, 0.0f, height_);
    set_points(width_ - kCornerRadius, width_, kCornerRadius, height_ - kCornerRadius);
    set_points(0.0f, kCornerRadius, kCornerRadius, height_ - kCornerRadius);

    constexpr float kAngleInc = (M_PI / 2.0f) / (static_cast<float>(kNumTrianglesPerCorner));

    const auto add_corner_triangles =
        [this, &idx](const float x_center, const float y_center, const float theta0, const float angle_inc) -> void {
        float theta = theta0;
        for (size_t k = 0; k < kNumTrianglesPerCorner; ++k)
        {
            points_[idx] = x_center;
            points_[idx + 1U] = y_center;

            const float x_pt = kCornerRadius * std::cos(theta);
            const float y_pt = kCornerRadius * std::sin(theta);

            points_[idx + 2U] = x_center + x_pt;
            points_[idx + 3U] = y_center + y_pt;

            theta += angle_inc;
            const float x_pt_next = kCornerRadius * std::cos(theta);
            const float y_pt_next = kCornerRadius * std::sin(theta);

            points_[idx + 4U] = x_center + x_pt_next;
            points_[idx + 5U] = y_center + y_pt_next;

            idx += 6U;
        }
    };

    add_corner_triangles(x_ + width_ - kCornerRadius, y_ + height_ - kCornerRadius, 0.0f, kAngleInc);
    add_corner_triangles(x_ + kCornerRadius, y_ + height_ - kCornerRadius, M_PI / 2.0f, kAngleInc);
    add_corner_triangles(x_ + kCornerRadius, y_ + kCornerRadius, M_PI, kAngleInc);
    add_corner_triangles(x_ + width_ - kCornerRadius, y_ + kCornerRadius, 3.0f * M_PI / 2.0f, kAngleInc);

    num_points_to_render_ = idx / 2U;
}

void Button::mousePressed(wxMouseEvent& event)
{
    color_ = kButtonPressedColor;

    if (button_pressed_callback_)
    {
        button_pressed_callback_(id_);
    }
}

void Button::mouseReleased(wxMouseEvent& event)
{
    color_ = kButtonColor;

    if (button_released_callback_)
    {
        button_released_callback_(id_);
    }
}

void Button::updateVertexBuffer()
{
    changeButtonPoints();

    vertex_buffer_2_.updateBufferData(0, points_, num_points_to_render_, 2);
}

void Button::render() const
{
    vertex_buffer_2_.render(num_points_to_render_);
}
