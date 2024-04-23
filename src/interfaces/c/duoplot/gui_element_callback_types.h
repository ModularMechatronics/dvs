#ifndef GUI_ELEMENT_CALLBACK_TYPES_H
#define GUI_ELEMENT_CALLBACK_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "duoplot/enumerations.h"
#include "duoplot/internal_gui_element_handle.h"
#include "duoplot/uint8_array.h"

typedef void (*duoplot_ButtonCallbackFunction)(const duoplot_ButtonHandle);
typedef void (*duoplot_SliderCallbackFunction)(const duoplot_SliderHandle);
typedef void (*duoplot_ListBoxCallbackFunction)(const duoplot_ListBoxHandle);
typedef void (*duoplot_DropdownMenuCallbackFunction)(const duoplot_DropdownMenuHandle);
typedef void (*duoplot_CheckboxCallbackFunction)(const duoplot_CheckboxHandle);
typedef void (*duoplot_RadioButtonGroupCallbackFunction)(const duoplot_RadioButtonGroupHandle);
typedef void (*duoplot_EditableTextCallbackFunction)(const duoplot_EditableTextHandle);

#endif  // GUI_ELEMENT_CALLBACK_TYPES_H