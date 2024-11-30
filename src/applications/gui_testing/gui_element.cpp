#include "gui_element.h"

#include "constants.h"

GuiElement::GuiElement(const float x,
                       const float y,
                       const float width,
                       const float height,
                       const std::string& name,
                       const RGBTripletf& color)
    : x_{x},
      y_{y},
      width_{width},
      height_{height},
      vertex_buffer_{OGLPrimitiveType::TRIANGLES},
      name_{name},
      color_{color},
      parent_width_{100.0f},
      parent_height_{100.0f}
{
    /*
    Input arguments are in range 0 to something, where 0 is the top left corner.
    OpenGL is between [-1, and 1] for x and y, with 0, 0 in the center.
    */

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

    vertex_buffer_.addBuffer(vertices, 6, 2, GL_DYNAMIC_DRAW);
}

/*
x = 10, width = 8, margin = 3
10, 11, 12, 13, 14, 15, 16, 17
10, 11, 12 left change margin
                    15, 16, 17 right change margin
if(pt.x >= x_ && pt.x < x_ + margin)
{
    left
}
else if (pt.x >= x_ + width - margin && pt.x < x_ + width)
{
    right
}

*/

ChangeDirection GuiElement::GetDirectionFromMouse(const wxPoint pt) const
{
    enum class LeftRightChangeValid : uint8_t
    {
        LEFT,
        RIGHT,
        NONE
    };

    LeftRightChangeValid left_right_change = LeftRightChangeValid::NONE;
    left_right_change = pt.x >= x_ && pt.x < x_ + kEditingMarginSize ? LeftRightChangeValid::LEFT : left_right_change;
    left_right_change = pt.x >= (x_ + width_ - kEditingMarginSize) && pt.x < (x_ + width_) ? LeftRightChangeValid::RIGHT
                                                                                           : left_right_change;

    enum class UpDownChangeValid : uint8_t
    {
        UP,
        DOWN,
        NONE
    };

    UpDownChangeValid up_down_change = UpDownChangeValid::NONE;
    up_down_change = pt.y >= y_ && pt.y < y_ + kEditingMarginSize ? UpDownChangeValid::UP : up_down_change;
    up_down_change =
        pt.y >= (y_ + height_ - kEditingMarginSize) && pt.y < (y_ + height_) ? UpDownChangeValid::DOWN : up_down_change;

    if (left_right_change == LeftRightChangeValid::LEFT && up_down_change == UpDownChangeValid::UP)
    {
        // std::cout << "ChangeDirection::LEFT_UP" << std::endl;
        return ChangeDirection::LEFT_UP;
    }
    else if (left_right_change == LeftRightChangeValid::LEFT && up_down_change == UpDownChangeValid::DOWN)
    {
        // std::cout << "ChangeDirection::LEFT_DOWN" << std::endl;
        return ChangeDirection::LEFT_DOWN;
    }
    else if (left_right_change == LeftRightChangeValid::RIGHT && up_down_change == UpDownChangeValid::UP)
    {
        // std::cout << "ChangeDirection::RIGHT_UP" << std::endl;
        return ChangeDirection::RIGHT_UP;
    }
    else if (left_right_change == LeftRightChangeValid::RIGHT && up_down_change == UpDownChangeValid::DOWN)
    {
        // std::cout << "ChangeDirection::RIGHT_DOWN" << std::endl;
        return ChangeDirection::RIGHT_DOWN;
    }
    else if (left_right_change == LeftRightChangeValid::LEFT)
    {
        // std::cout << "ChangeDirection::LEFT" << std::endl;
        return ChangeDirection::LEFT;
    }
    else if (left_right_change == LeftRightChangeValid::RIGHT)
    {
        // std::cout << "ChangeDirection::RIGHT" << std::endl;
        return ChangeDirection::RIGHT;
    }
    else if (up_down_change == UpDownChangeValid::UP)
    {
        // std::cout << "ChangeDirection::UP" << std::endl;
        return ChangeDirection::UP;
    }
    else if (up_down_change == UpDownChangeValid::DOWN)
    {
        // std::cout << "ChangeDirection::DOWN" << std::endl;
        return ChangeDirection::DOWN;
    }
    else if (pt.x >= x_ && pt.x < x_ + width_ && pt.y >= y_ && pt.y < y_ + height_)
    {
        // std::cout << "ChangeDirection::MIDDLE" << std::endl;
        return ChangeDirection::MIDDLE;
    }
    else
    {
        assert(false && "Shouldn't end up here!");
        return ChangeDirection::NONE;
    }
}

void GuiElement::ChangePositionOrSize(const wxPoint delta_vec, const ChangeDirection change_direction)
{
    float delta_x = 0.0f, delta_y = 0.0f, delta_width = 0.0f, delta_height = 0.0f;

    switch (change_direction)
    {
        case ChangeDirection::UP:
            delta_height -= delta_vec.y;
            delta_y += delta_vec.y;
            break;
        case ChangeDirection::DOWN:
            delta_height += delta_vec.y;
            break;
        case ChangeDirection::LEFT:
            delta_x += delta_vec.x;
            delta_width -= delta_vec.x;
            break;
        case ChangeDirection::RIGHT:
            delta_width += delta_vec.x;
            break;
        case ChangeDirection::LEFT_UP:
            delta_height -= delta_vec.y;
            delta_y += delta_vec.y;
            delta_x += delta_vec.x;
            delta_width -= delta_vec.x;
            break;
        case ChangeDirection::LEFT_DOWN:
            delta_height += delta_vec.y;
            delta_x += delta_vec.x;
            delta_width -= delta_vec.x;
            break;
        case ChangeDirection::RIGHT_UP:
            delta_height -= delta_vec.y;
            delta_y += delta_vec.y;
            delta_width += delta_vec.x;
            break;
        case ChangeDirection::RIGHT_DOWN:
            delta_height += delta_vec.y;
            delta_width += delta_vec.x;
            break;
        case ChangeDirection::MIDDLE:
            delta_x += delta_vec.x;
            delta_y += delta_vec.y;
            break;
        default:
            assert(false && "Shouldn't end up here!");
            break;
    }

    UpdatePositionOrSize(delta_x, delta_y, delta_width, delta_height, change_direction);

    updateVertexBuffer();
}

void GuiElement::UpdatePositionOrSize(const float delta_x,
                                      const float delta_y,
                                      const float delta_width,
                                      const float delta_height,
                                      const ChangeDirection change_direction)
{
    const float x_before = x_;
    const float y_before = y_;

    x_ += delta_x;
    y_ += delta_y;
    width_ += delta_width;
    height_ += delta_height;

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
}

void GuiElement::updateVertexBuffer()
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

    vertex_buffer_.updateBufferData(0, vertices, 6, 2);
}

void GuiElement::mousePressed(wxMouseEvent& event)
{
    std::cout << "Mouse pressed for element \"" << name_ << "\"" << std::endl;
}

void GuiElement::mouseReleased(wxMouseEvent& event)
{
    std::cout << "Mouse released for element \"" << name_ << "\"" << std::endl;
}

void GuiElement::render() const
{
    vertex_buffer_.render(6);
}

void GuiElement::UpdateSizeFromParent(const wxSize new_size)
{
    parent_width_ = new_size.GetWidth();
    parent_height_ = new_size.GetHeight();
}

bool GuiElement::PointIsWithin(const wxPoint pt) const
{
    return pt.x >= x_ && pt.x < x_ + width_ && pt.y >= y_ && pt.y < y_ + height_;
}

void GuiElement::mouseExited(wxMouseEvent& event)
{
    std::cout << "Mouse exited for element " << name_ << std::endl << std::endl;
}

void GuiElement::mouseExited(wxPoint& exit_point)
{
    std::cout << "Mouse exited for element " << name_ << " from losing focus from a right click" << std::endl;
}

void GuiElement::mouseEntered(wxMouseEvent& event)
{
    std::cout << "Mouse entered for element " << name_ << std::endl;
}

static int i = 0;

void GuiElement::mouseMoved(wxMouseEvent& event, const wxPoint& delta_vec)
{
    /*color_.blue = color_.blue + 0.01f;
    if (color_.blue > 1.0f)
    {
        color_.blue = 0.0f;
    }*/

    i++;
}

void GuiElement::mouseDragged(wxMouseEvent& event, const wxPoint& delta_vec)
{
    /*color_.red = color_.red + 0.01f;
    if (color_.red > 1.0f)
    {
        color_.red = 0.0f;
    }*/
}
