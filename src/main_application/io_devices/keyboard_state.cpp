#include "io_devices/keyboard_state.h"

#include <algorithm>

bool KeyboardState::isPressed() const
{
    return pressed_keys_.size() > 0;
}

bool KeyboardState::keyIsPressed(const char key) const
{
    const char key_upper = toupper(key);
    const char key_lower = tolower(key_upper);
    return (std::count(pressed_keys_.begin(), pressed_keys_.end(), key_upper) > 0) || 
        (std::count(pressed_keys_.begin(), pressed_keys_.end(), key_lower) > 0);
}

void KeyboardState::keyGotPressed(const char key)
{
    if (!keyIsPressed(key))
    {
        pressed_keys_.push_back(key);
    }
}

void KeyboardState::keyGotReleased(const char key)
{
    if (keyIsPressed(key))
    {
        pressed_keys_.erase(std::find(pressed_keys_.begin(), pressed_keys_.end(), key));
    }
}
