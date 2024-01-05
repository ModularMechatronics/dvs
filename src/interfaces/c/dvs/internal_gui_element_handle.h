#ifndef DVS_INTERNAL_GUI_ELEMENT_HANDLE_H
#define DVS_INTERNAL_GUI_ELEMENT_HANDLE_H

#include "dvs/enumerations.h"

typedef struct S_GuiElementHandleContainer
{
    GuiElementType type;
    uint8_t* data;
} GuiElementHandleContainer;

#endif  // DVS_INTERNAL_GUI_ELEMENT_HANDLE_H
