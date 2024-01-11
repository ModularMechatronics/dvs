#ifndef GUI_ELEMENT_CALLBACK_TYPES_H
#define GUI_ELEMENT_CALLBACK_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dvs/enumerations.h"
#include "dvs/internal_gui_element_handle.h"
#include "dvs/uint8_array.h"

typedef void (*ButtonCallbackFunction)(const ButtonHandle);
typedef void (*SliderCallbackFunction)(const SliderHandle);
typedef void (*ListBoxCallbackFunction)(const ListBoxHandle);
typedef void (*DropDownMenuCallbackFunction)(const DropDownMenuHandle);
typedef void (*CheckboxCallbackFunction)(const CheckboxHandle);
typedef void (*RadioButtonGroupCallbackFunction)(const RadioButtonGroupHandle);
typedef void (*EditableTextCallbackFunction)(const EditableTextHandle);

#define TO_BASE_HANDLE_PTR(button_handle) ((BaseHandle*)(&(button_handle)))

#endif  // GUI_ELEMENT_CALLBACK_TYPES_H