#ifndef DVS_INTERNAL_GUI_ELEMENT_HANDLE_H
#define DVS_INTERNAL_GUI_ELEMENT_HANDLE_H

#include "dvs/enumerations.h"
#include "dvs/list_of_strings.h"

typedef struct S_BaseHandle
{
    GuiElementType type;
    char* handle_string;
} BaseHandle;

// Gui element state types

typedef struct S_SliderState
{
    int32_t min_value;
    int32_t max_value;
    int32_t step_size;
    int32_t value;
} SliderState;

typedef struct S_ListBoxState
{
    ListOfStrings elements;
    char* selected_element;
} ListBoxState;

typedef struct S_DropDownMenuState
{
    ListOfStrings elements;
    char* selected_element;
} DropDownMenuState;

// Gui element internal handle types

typedef struct S_ButtonInternalHandle
{
    GuiElementType type;
    char* handle_string;
    bool is_pressed;
} ButtonInternalHandle;

typedef struct S_ListBoxInternalHandle
{
    GuiElementType type;
    char* handle_string;

    ListBoxState state;
} ListBoxInternalHandle;

typedef struct S_SliderInternalHandle
{
    GuiElementType type;
    char* handle_string;
    SliderState state;
} SliderInternalHandle;

typedef struct S_TextLabelInternalHandle
{
    GuiElementType type;
    char* handle_string;
    char* text;
} TextLabelInternalHandle;

typedef struct S_DropDownMenuInternalHandle
{
    GuiElementType type;
    char* handle_string;

    DropDownMenuState state;
} DropDownMenuInternalHandle;

typedef struct S_CheckboxInternalHandle
{
    GuiElementType type;
    char* handle_string;
    bool is_checked;
} CheckboxInternalHandle;

typedef struct S_RadioButtonGroupInternalHandle
{
    GuiElementType type;
    char* handle_string;

    ListOfStrings buttons;
    int32_t selected_button_idx;
} RadioButtonGroupInternalHandle;

typedef struct S_EditableTextInternalHandle
{
    GuiElementType type;
    char* handle_string;
    char* text;
    bool enter_pressed;
} EditableTextInternalHandle;

// Gui element handle types

typedef struct S_ButtonHandle
{
    ButtonInternalHandle* __handle;
} ButtonHandle;

typedef struct S_SliderHandle
{
    SliderInternalHandle* __handle;
} SliderHandle;

typedef struct S_ListBoxHandle
{
    ListBoxInternalHandle* __handle;
} ListBoxHandle;

typedef struct S_DropDownMenuHandle
{
    DropDownMenuInternalHandle* __handle;

} DropDownMenuHandle;

typedef struct S_CheckboxHandle
{
    CheckboxInternalHandle* __handle;
} CheckboxHandle;

typedef struct S_RadioButtonGroupHandle
{
    RadioButtonGroupInternalHandle* __handle;
} RadioButtonGroupHandle;

typedef struct S_EditableTextHandle
{
    EditableTextInternalHandle* __handle;
} EditableTextHandle;

#endif  // DVS_INTERNAL_GUI_ELEMENT_HANDLE_H
