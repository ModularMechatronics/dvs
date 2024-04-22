#ifndef DUOPLOT_INTERNAL_GUI_ELEMENT_HANDLE_H
#define DUOPLOT_INTERNAL_GUI_ELEMENT_HANDLE_H

#include "duoplot/enumerations.h"
#include "duoplot/list_of_strings.h"

typedef struct S_duoplot_internal_BaseHandle
{
    GuiElementType type;
    char* handle_string;
} duoplot_internal_BaseHandle;

typedef struct S_duoplot_SliderState
{
    int32_t min_value;
    int32_t max_value;
    int32_t step_size;
    int32_t value;
} duoplot_SliderState;

typedef struct S_duoplot_ListBoxState
{
    duoplot_ListOfStrings elements;
    char* selected_element;
} duoplot_ListBoxState;

typedef struct S_duoplot_DropdownMenuState
{
    duoplot_ListOfStrings elements;
    char* selected_element;
} duoplot_DropdownMenuState;

typedef struct S_duoplot_ButtonInternalHandle
{
    GuiElementType type;
    char* handle_string;
    bool is_pressed;
} duoplot_internal_ButtonInternalHandle;

typedef struct S_duoplot_ListBoxInternalHandle
{
    GuiElementType type;
    char* handle_string;

    duoplot_ListBoxState state;
} duoplot_internal_ListBoxInternalHandle;

typedef struct S_duoplot_SliderInternalHandle
{
    GuiElementType type;
    char* handle_string;
    duoplot_SliderState state;
} duoplot_internal_SliderInternalHandle;

typedef struct S_duoplot_TextLabelInternalHandle
{
    GuiElementType type;
    char* handle_string;
    char* text;
} duoplot_internal_TextLabelInternalHandle;

typedef struct S_duoplot_DropdownMenuInternalHandle
{
    GuiElementType type;
    char* handle_string;

    duoplot_DropdownMenuState state;
} duoplot_internal_DropdownMenuInternalHandle;

typedef struct S_duoplot_CheckboxInternalHandle
{
    GuiElementType type;
    char* handle_string;
    bool is_checked;
} duoplot_internal_CheckboxInternalHandle;

typedef struct S_duoplot_RadioButtonGroupInternalHandle
{
    GuiElementType type;
    char* handle_string;

    duoplot_ListOfStrings buttons;
    int32_t selected_button_idx;
} RadioButtonGroupInternalHandle;

typedef struct S_duoplot_EditableTextInternalHandle
{
    GuiElementType type;
    char* handle_string;
    char* text;
    bool enter_pressed;
} duoplot_internal_EditableTextInternalHandle;

// Gui element handle types

typedef struct S_duoplot_ButtonHandle
{
    duoplot_internal_ButtonInternalHandle* __handle;
} duoplot_ButtonHandle;

typedef struct S_duoplot_SliderHandle
{
    duoplot_internal_SliderInternalHandle* __handle;
} duoplot_SliderHandle;

typedef struct S_duoplot_ListBoxHandle
{
    duoplot_internal_ListBoxInternalHandle* __handle;
} duoplot_ListBoxHandle;

typedef struct S_duoplot_DropdownMenuHandle
{
    duoplot_internal_DropdownMenuInternalHandle* __handle;

} duoplot_DropdownMenuHandle;

typedef struct S_duoplot_CheckboxHandle
{
    duoplot_internal_CheckboxInternalHandle* __handle;
} duoplot_CheckboxHandle;

typedef struct S_duoplot_RadioButtonGroupHandle
{
    RadioButtonGroupInternalHandle* __handle;
} duoplot_RadioButtonGroupHandle;

typedef struct S_duoplot_EditableTextHandle
{
    duoplot_internal_EditableTextInternalHandle* __handle;
} duoplot_EditableTextHandle;

typedef struct S_duoplot_TextLabelHandle
{
    duoplot_internal_TextLabelInternalHandle* __handle;
} duoplot_TextLabelHandle;

#endif  // DUOPLOT_INTERNAL_GUI_ELEMENT_HANDLE_H
