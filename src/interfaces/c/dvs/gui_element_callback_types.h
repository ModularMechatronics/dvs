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

typedef struct S_ButtonInternalHandle
{
    GuiElementType type;
    char* handle_string;
    bool is_pressed;
} ButtonInternalHandle;

typedef struct S_SliderState
{
    int32_t min_value;
    int32_t max_value;
    int32_t step_size;
    int32_t value;
} SliderState;

typedef struct S_SliderInternalHandle
{
    GuiElementType type;
    char* handle_string;
    SliderState state;
} SliderInternalHandle;

typedef struct S_ButtonHandle
{
    ButtonInternalHandle* __handle;
} ButtonHandle;

typedef struct S_SliderHandle
{
    SliderInternalHandle* __handle;
} SliderHandle;

typedef void (*ButtonCallbackFunction)(const ButtonHandle);
typedef void (*SliderCallbackFunction)(const SliderHandle);

#define TO_BASE_HANDLE_PTR(button_handle) ((BaseHandle*)(&(button_handle)))

#endif  // GUI_ELEMENT_CALLBACK_TYPES_H