#include "button.h"

#include <array>

using namespace duoplot;

constexpr size_t kNumTrianglesPerCorner = 10U;
constexpr size_t kNumPointsPerCorner = (kNumTrianglesPerCorner)*3U;
constexpr size_t kNumTrianglesForRectangularParts = 2U * 3U;
constexpr size_t kTotalNumPoints = kNumPointsPerCorner * 4U + 3U * kNumTrianglesForRectangularParts;

constexpr float kCornerRadius = 6.0f;
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
      button_buffer_(kTotalNumPoints * 2U),
      button_released_callback_(button_released_callback),
      vertex_buffer_2_(OGLPrimitiveType::TRIANGLES)
{
    shader_mode_ = ShaderMode::BUTTON;
    color_ = kButtonColor;

    std::memset(button_buffer_.data(), 0, kTotalNumPoints * 2U * sizeof(float));

    changeButtonPoints();

    vertex_buffer_2_.addBuffer(button_buffer_.data(), num_points_to_render_, 2, GL_DYNAMIC_DRAW);
}

Button::~Button() {}

void Button::changeButtonPoints()
{
    button_buffer_.reset();

    // Center rectangle
    PutRectangleIntoBuffer(button_buffer_, x_ + kCornerRadius, y_, width_ - 2.0f * kCornerRadius, height_);

    // Left rectangle
    PutRectangleIntoBuffer(button_buffer_, x_, y_ + kCornerRadius, kCornerRadius, height_ - 2.0f * kCornerRadius);

    // Right rectangle
    PutRectangleIntoBuffer(
        button_buffer_, x_ + width_ - kCornerRadius, y_ + kCornerRadius, kCornerRadius, height_ - 2.0f * kCornerRadius);

    // Top left corner
    PutCircleSegmentIntoBuffer(button_buffer_,
                               x_ + kCornerRadius,
                               y_ + kCornerRadius,
                               kCornerRadius,
                               M_PI / 2.0f,
                               M_PI,
                               kNumTrianglesPerCorner);

    // Top right corner
    PutCircleSegmentIntoBuffer(button_buffer_,
                               x_ + width_ - kCornerRadius,
                               y_ + kCornerRadius,
                               kCornerRadius,
                               0.0f,
                               M_PI / 2.0f,
                               kNumTrianglesPerCorner);

    // Bottom right corner
    PutCircleSegmentIntoBuffer(button_buffer_,
                               x_ + width_ - kCornerRadius,
                               y_ + height_ - kCornerRadius,
                               kCornerRadius,
                               3.0f * M_PI / 2.0f,
                               2.0f * M_PI,
                               kNumTrianglesPerCorner);

    // Bottom left corner
    PutCircleSegmentIntoBuffer(button_buffer_,
                               x_ + kCornerRadius,
                               y_ + height_ - kCornerRadius,
                               kCornerRadius,
                               M_PI,
                               3.0f * M_PI / 2.0f,
                               kNumTrianglesPerCorner);

    num_points_to_render_ = button_buffer_.idx() / 2U;
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

    vertex_buffer_2_.updateBufferData(0, button_buffer_.data(), num_points_to_render_, 2);
}

void Button::render() const
{
    vertex_buffer_2_.render(num_points_to_render_);
}
