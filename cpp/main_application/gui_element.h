#ifndef GUI_ELEMENT_H_
#define GUI_ELEMENT_H_

#include "math/math.h"

enum class GuiElementType
{
    PLOT,
    IMAGE,
    BUTTON,
    TEXT_FIELD
};

class GuiElement
{
private:
    GuiElementType type_;

    bool is_dockable_;
    bool is_docked_;
public:
    GuiElement() = delete;
    GuiElement(const bool is_dockable, const bool is_docked)
    {
        is_dockable_ = is_dockable;
        is_docked_ = is_docked;
    }


};

class GuiElementDocked : GuiElement
{

};

class GuiElementWindow : GuiElement
{
private:

public:
};



#endif
