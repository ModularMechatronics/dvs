#ifndef MAIN_APPLICATION_IO_DEVICES_MOUSE_STATE_H_
#define MAIN_APPLICATION_IO_DEVICES_MOUSE_STATE_H_

#include "dvs/math/math.h"

using namespace dvs;

class MouseButtonState
{
private:
    Vec2i curr_pos_;
    Vec2i prev_pos_;
    Vec2i delta_pos_;

    bool is_pressed_;

public:
    MouseButtonState();
    void setIsPressed(const int x, const int y);
    void setIsReleased();

    void updateOnMotion(const int x, const int y);

    bool isPressed() const;

    void print() const;

    Vec2i getCurrPos() const;
    Vec2i getPrevPos() const;
    Vec2i getDeltaPos() const;
};

#endif // MAIN_APPLICATION_IO_DEVICES_MOUSE_STATE_H_
