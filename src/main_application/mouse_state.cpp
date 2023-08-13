#include "mouse_state.h"

#include <iostream>
#include <string>
#include <vector>

MouseState::MouseState()
{
    is_pressed_ = false;

    curr_pos_.x = 0;
    curr_pos_.y = 0;

    prev_pos_.x = 0;
    prev_pos_.y = 0;

    delta_pos_.x = 0;
    delta_pos_.y = 0;
}

Vec2i MouseState::getDeltaPos() const
{
    return delta_pos_;
}

Vec2i MouseState::getCurrPos() const
{
    return curr_pos_;
}

void MouseState::setCurrentPos(const int current_x, const int current_y)
{
    curr_pos_.x = current_x;
    curr_pos_.y = current_y;

    prev_pos_.x = current_x;
    prev_pos_.y = current_y;

    delta_pos_.x = 0;
    delta_pos_.y = 0;
}

void MouseState::updateOnMotion(const int x, const int y)
{
    prev_pos_.x = curr_pos_.x;
    prev_pos_.y = curr_pos_.y;

    curr_pos_.x = x;
    curr_pos_.y = y;

    delta_pos_ = curr_pos_ - prev_pos_;
}

void MouseState::print() const
{
    const std::string p = is_pressed_ ? "pressed" : "not pressed";
    std::cout << "Button is " << p << " with xy coords [ " << curr_pos_.x << ", " << curr_pos_.y << " ]" << std::endl;
}
