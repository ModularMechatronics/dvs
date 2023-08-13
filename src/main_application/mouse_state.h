#ifndef MAIN_APPLICATION_IO_DEVICES_MOUSE_STATE_H_
#define MAIN_APPLICATION_IO_DEVICES_MOUSE_STATE_H_

#include "dvs/math/math.h"

using namespace dvs;

class MouseState
{
private:
    Vec2i curr_pos_;
    Vec2i prev_pos_;
    Vec2i delta_pos_;

    bool is_pressed_;

public:
    MouseState();

    void updateOnMotion(const int x, const int y);
    void setCurrentPos(const int current_x, const int current_y);

    void print() const;

    Vec2i getDeltaPos() const;
    Vec2i getCurrPos() const;
};

#endif  // MAIN_APPLICATION_IO_DEVICES_MOUSE_STATE_H_
