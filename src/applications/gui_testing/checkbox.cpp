#include "checkbox.h"

#include <array>

using namespace duoplot;

constexpr size_t kNumRectanglesPerCorner = 10U;
constexpr size_t kNumPointsPerCorner = kNumRectanglesPerCorner * 2U * 3U;
constexpr size_t kNumTrianglesForRectangularParts = 2U * 3U;
constexpr size_t kTotalNumPoints = kNumPointsPerCorner * 4U + kNumTrianglesForRectangularParts * 4U;

constexpr float kCornerRadius = 30.0f;
constexpr float kEdgeThickness = 20.0f;
namespace
{
const RGBTripletf kButtonColor{0.6f, 0.5f, 0.4f};
constexpr float kClickedFactor = 0.8f;
const RGBTripletf kButtonPressedColor{
    kClickedFactor * 88.0f / 255.0f, kClickedFactor * 203.0f / 255.0f, kClickedFactor * 221.0f / 255.0f};

}  // namespace

Checkbox::Checkbox(const float x,
                   const float y,
                   const float width,
                   const float height,
                   const std::string label,
                   const RGBTripletf& color,
                   const std::function<void(const uint64_t, const bool)>& checkbox_state_changed_callback)
    : GuiElement(x, y, width, height, "CHECKBOX", color),
      label_(label),
      checkbox_state_changed_callback_(checkbox_state_changed_callback),
      checkbox_buffer_{kTotalNumPoints * 2U},
      vertex_buffer_2_(OGLPrimitiveType::TRIANGLES)
{
    is_checked_ = false;
    shader_mode_ = ShaderMode::CHECKBOX;
    color_ = kButtonColor;

    std::memset(checkbox_buffer_.data(), 0, kTotalNumPoints * 2U * sizeof(float));

    changeCheckboxPoints();

    vertex_buffer_2_.addBuffer(checkbox_buffer_.data(), num_points_to_render_, 2, GL_DYNAMIC_DRAW);
}

Checkbox::~Checkbox() {}

void Checkbox::changeCheckboxPoints()
{
    checkbox_buffer_.reset();

    // Top rectangle
    PutRectangleIntoBuffer(checkbox_buffer_, x_ + kCornerRadius, y_, width_ - 2.0f * kCornerRadius, kEdgeThickness);

    // Left rectangle
    PutRectangleIntoBuffer(checkbox_buffer_, x_, y_ + kCornerRadius, kEdgeThickness, height_ - 2.0f * kCornerRadius);

    // Right rectangle
    PutRectangleIntoBuffer(checkbox_buffer_,
                           x_ + width_ - kEdgeThickness,
                           y_ + kCornerRadius,
                           kEdgeThickness,
                           height_ - 2.0f * kCornerRadius);

    // Bottom rectangle
    PutRectangleIntoBuffer(checkbox_buffer_,
                           x_ + kCornerRadius,
                           y_ + height_ - kEdgeThickness,
                           width_ - 2.0f * kCornerRadius,
                           kEdgeThickness);

    // Top left corner
    PutCircularCurveSegmentIntoBuffer(checkbox_buffer_,
                                      x_ + kCornerRadius,
                                      y_ + kCornerRadius,
                                      kCornerRadius - kEdgeThickness,
                                      kCornerRadius,
                                      M_PI / 2.0f,
                                      M_PI,
                                      kNumRectanglesPerCorner);

    // Top right corner
    PutCircularCurveSegmentIntoBuffer(checkbox_buffer_,
                                      x_ + width_ - kCornerRadius,
                                      y_ + kCornerRadius,
                                      kCornerRadius - kEdgeThickness,
                                      kCornerRadius,
                                      0.0f,
                                      M_PI / 2.0f,
                                      kNumRectanglesPerCorner);

    // Bottom right corner
    PutCircularCurveSegmentIntoBuffer(checkbox_buffer_,
                                      x_ + width_ - kCornerRadius,
                                      y_ + height_ - kCornerRadius,
                                      kCornerRadius - kEdgeThickness,
                                      kCornerRadius,
                                      0.0f,
                                      -M_PI / 2.0f,
                                      kNumRectanglesPerCorner);

    // Bottom left corner
    PutCircularCurveSegmentIntoBuffer(checkbox_buffer_,
                                      x_ + kCornerRadius,
                                      y_ + height_ - kCornerRadius,
                                      kCornerRadius - kEdgeThickness,
                                      kCornerRadius,
                                      M_PI,
                                      3.0f * M_PI / 2.0f,
                                      kNumRectanglesPerCorner);

    num_points_to_render_ = checkbox_buffer_.idx() / 2U;
}

void Checkbox::mousePressed(wxMouseEvent& event)
{
    color_ = kButtonPressedColor;
}

void Checkbox::mouseReleased(wxMouseEvent& event)
{
    color_ = kButtonColor;
    is_checked_ = !is_checked_;

    if (checkbox_state_changed_callback_)
    {
        checkbox_state_changed_callback_(id_, is_checked_);
    }
}

void Checkbox::updateVertexBuffer()
{
    changeCheckboxPoints();

    vertex_buffer_2_.updateBufferData(0, checkbox_buffer_.data(), num_points_to_render_, 2);
}

void Checkbox::render() const
{
    vertex_buffer_2_.render(num_points_to_render_);
}
