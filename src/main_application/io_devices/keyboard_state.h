#ifndef MAIN_APPLICATION_IO_DEVICES_KEYBOARD_STATE_H_
#define MAIN_APPLICATION_IO_DEVICES_KEYBOARD_STATE_H_

#include <vector>

class KeyboardState
{
private:
    std::vector<char> pressed_keys_;

public:
    KeyboardState() = default;

    bool isPressed() const;
    bool keyIsPressed(const char key) const;

    void keyGotPressed(const char key);
    void keyGotReleased(const char key);
};

#endif  // MAIN_APPLICATION_IO_DEVICES_KEYBOARD_STATE_H_
