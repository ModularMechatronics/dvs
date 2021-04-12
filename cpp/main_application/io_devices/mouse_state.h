#ifndef MOUSE_STATE_H_
#define MOUSE_STATE_H_

#include "math/math.h"

using namespace dvs;

enum class MouseActivity
{
    PAN,
    ZOOM,
    ROTATE
};

class MouseButtonState
{
private:
    Vec2Di curr_pos_;
    Vec2Di prev_pos_;
    Vec2Di delta_pos_;

    bool is_pressed_;

public:
    MouseButtonState();
    void setIsPressed(const int x, const int y);
    void setIsReleased();

    void updateOnMotion(const int x, const int y);

    bool isPressed() const;

    void print() const;

    Vec2Di getCurrPos() const;
    Vec2Di getPrevPos() const;
    Vec2Di getDeltaPos() const;
};

#endif
