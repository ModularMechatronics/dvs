#ifndef GUI_ELEMENT_CALLBACK_TYPES_H
#define GUI_ELEMENT_CALLBACK_TYPES_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dvs/enumerations.h"
#include "dvs/internal_gui_element_handle.h"
#include "dvs/uint8_array.h"

typedef struct S_String
{
    char* data;
    size_t length;
} String;

typedef struct S_ListOfStrings
{
    char** strings;
    size_t size;
} ListOfStrings;

ListOfStrings createListOfStrings(const size_t size)
{
    ListOfStrings list_of_strings;

    list_of_strings.size = size;
    list_of_strings.strings = (char**)malloc(size * sizeof(char*));

    for (size_t k = 0; k < size; k++)
    {
        list_of_strings.strings[k] = NULL;
    }

    return list_of_strings;
}

void destroyListOfStrings(ListOfStrings* const list_of_strings)
{
    for (size_t k = 0; k < list_of_strings->size; k++)
    {
        if (list_of_strings->strings[k] != NULL)
        {
            free(list_of_strings->strings[k]);
        }
    }

    free(list_of_strings->strings);
    list_of_strings->strings = NULL;
    list_of_strings->size = 0;
}

String createString(const size_t length)
{
    String s;

    s.length = length;
    s.data = (char*)malloc(s.length + 1);
    s.data[0] = '\0';

    return s;
}

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

typedef struct S_ListBoxState
{
    ListOfStrings elements;
    char* selected_string;
} ListBoxState;

typedef struct S_ListBoxInternalHandle
{
    GuiElementType type;
    char* handle_string;

    ListBoxState state;
} ListBoxInternalHandle;

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

typedef void (*ButtonCallbackFunction)(const ButtonHandle);
typedef void (*SliderCallbackFunction)(const SliderHandle);

#define TO_BASE_HANDLE_PTR(button_handle) ((BaseHandle*)(&(button_handle)))

#endif  // GUI_ELEMENT_CALLBACK_TYPES_H