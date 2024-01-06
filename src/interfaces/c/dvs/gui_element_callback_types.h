#ifndef GUI_ELEMENT_CALLBACK_TYPES_H
#define GUI_ELEMENT_CALLBACK_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dvs/enumerations.h"
#include "dvs/internal_gui_element_handle.h"
#include "dvs/uint8_array.h"

typedef struct S_BaseHandle
{
    GuiElementType type;
    char* handle_string;
} BaseHandle;

typedef struct S_ButtonHandle
{
    GuiElementType type;
    char* handle_string;
    bool is_pressed;
} ButtonHandle;

typedef struct S_SliderHandle
{
    GuiElementType type;
    char* handle_string;
    int32_t min_value;
    int32_t max_value;
    int32_t step_size;
    int32_t value;
} SliderHandle;

typedef void (*ButtonCallbackFunction)(const ButtonHandle* const);
typedef void (*SliderCallbackFunction)(const SliderHandle* const);
// void (*button_callback_function)(const ButtonHandle* const)

#endif  // GUI_ELEMENT_CALLBACK_TYPES_H