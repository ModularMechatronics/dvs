#ifndef DUOPLOT_MAP_H
#define DUOPLOT_MAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "duoplot/internal_gui_element_handle.h"
#include "duoplot/pp.h"
#include "duoplot/uint8_array.h"
#include "gui_element_callback_types.h"

DUOPLOT_WEAK char* duoplot_internal_createAndCopyString(const char* const str)
{
    const size_t string_length = strlen(str);

    char* new_string = malloc(string_length + 1);
    memcpy(new_string, str, string_length);

    new_string[string_length] = '\0';

    return new_string;
}

typedef struct S_duoplot_internal_GuiElementMap
{
    char** keys;
    duoplot_internal_BaseHandle** values;

    size_t size;
} duoplot_internal_GuiElementMap;

typedef struct S_duoplot_internal_ButtonCallbackMap
{
    char** keys;
    duoplot_ButtonCallbackFunction* values;

    size_t size;
} duoplot_internal_ButtonCallbackFunctionMap;

typedef struct S_duoplot_internal_SliderCallbackFunctionMap
{
    char** keys;
    duoplot_SliderCallbackFunction* values;

    size_t size;
} duoplot_internal_SliderCallbackFunctionMap;

typedef struct S_duoplot_internal_ListBoxCallbackFunctionMap
{
    char** keys;
    duoplot_ListBoxCallbackFunction* values;

    size_t size;
} duoplot_internal_ListBoxCallbackFunctionMap;

typedef struct S_duoplot_internal_DropdownMenuCallbackFunctionMap
{
    char** keys;
    duoplot_DropdownMenuCallbackFunction* values;

    size_t size;
} duoplot_internal_DropdownMenuCallbackFunctionMap;

typedef struct S_duoplot_internal_CheckboxCallbackFunctionMap
{
    char** keys;
    duoplot_CheckboxCallbackFunction* values;

    size_t size;
} duoplot_internal_CheckboxCallbackFunctionMap;

typedef struct S_duoplot_internal_RadioButtonGroupCallbackFunctionMap
{
    char** keys;
    duoplot_RadioButtonGroupCallbackFunction* values;

    size_t size;
} duoplot_internal_RadioButtonGroupCallbackFunctionMap;

typedef struct S_duoplot_internal_EditableTextCallbackFunctionMap
{
    char** keys;
    duoplot_EditableTextCallbackFunction* values;

    size_t size;
} duoplot_internal_EditableTextCallbackFunctionMap;

#define DUOPLOT_INTERNAL_INIT_MAP(__map, __initial_size, __map_value_type)                \
    __map->keys = (char**)malloc(__initial_size * sizeof(char*));                         \
    __map->values = (__map_value_type*)malloc(__initial_size * sizeof(__map_value_type)); \
                                                                                          \
    __map->size = __initial_size;                                                         \
                                                                                          \
    for (size_t k = 0U; k < __initial_size; k++)                                          \
    {                                                                                     \
        __map->keys[k] = NULL;                                                            \
        __map->values[k] = NULL;                                                          \
    }

#define DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(__map, __handle_string, __value, __map_value_type) \
    for (size_t k = 0; k < __map->size; k++)                                                        \
    {                                                                                               \
        if (__map->keys[k] == NULL)                                                                 \
        {                                                                                           \
            __map->keys[k] = (char*)malloc((strlen(__handle_string) + 1) * sizeof(char));           \
            __map->keys[k] = strcpy(__map->keys[k], __handle_string);                               \
            __map->values[k] = __value;                                                             \
            return;                                                                                 \
        }                                                                                           \
    }                                                                                               \
    size_t new_size = __map->size * 2U;                                                             \
    char** tmp_keys = (char**)malloc(new_size * sizeof(char*));                                     \
    __map_value_type* tmp_values = (__map_value_type*)malloc(new_size * sizeof(__map_value_type));  \
    for (size_t k = 0; k < new_size; k++)                                                           \
    {                                                                                               \
        if (k < __map->size)                                                                        \
        {                                                                                           \
            tmp_keys[k] = __map->keys[k];                                                           \
            tmp_values[k] = __map->values[k];                                                       \
        }                                                                                           \
        else                                                                                        \
        {                                                                                           \
            tmp_keys[k] = NULL;                                                                     \
            tmp_values[k] = NULL;                                                                   \
        }                                                                                           \
    }                                                                                               \
    tmp_keys[__map->size] = (char*)malloc((strlen(__handle_string) + 1) * sizeof(char));            \
    strcpy(tmp_keys[__map->size], __handle_string);                                                 \
    tmp_values[__map->size] = __value;                                                              \
    free(__map->keys);                                                                              \
    free(__map->values);                                                                            \
    __map->keys = tmp_keys;                                                                         \
    __map->values = tmp_values;                                                                     \
    __map->size = new_size;

#define DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(__map, __handle_string, __ret_val)    \
    if (!__handle_string || !__map || !(__map->keys) || __handle_string[0] == '\0') \
    {                                                                               \
        return NULL;                                                                \
    }                                                                               \
    for (size_t k = 0; k < __map->size; k++)                                        \
    {                                                                               \
        if (__map->keys[k] && (strcmp(__map->keys[k], __handle_string) == 0))       \
        {                                                                           \
            __ret_val = __map->values[k];                                           \
            break;                                                                  \
        }                                                                           \
    }

#define DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(__map, __handle_string, __ret_val)       \
    if (!__handle_string || !__map || !(__map->keys) || __handle_string[0] == '\0') \
    {                                                                               \
        __ret_val = false;                                                          \
    }                                                                               \
    else                                                                            \
    {                                                                               \
        for (size_t k = 0; k < __map->size; k++)                                    \
        {                                                                           \
            if (__map->keys[k] && (strcmp(__map->keys[k], __handle_string) == 0))   \
            {                                                                       \
                __ret_val = true;                                                   \
                break;                                                              \
            }                                                                       \
        }                                                                           \
    }

#define DUOPLOT_INTERNAL_RESET_MAP(__map, __initial_size, __map_value_type) \
    free(__map->keys);                                                      \
    free(__map->values);                                                    \
    DUOPLOT_INTERNAL_INIT_MAP(__map, __initial_size, __map_value_type);

// duoplot_internal_GuiElementMap
DUOPLOT_WEAK void duoplot_internal_initGuiElementHandleContainerMap(duoplot_internal_GuiElementMap* const map,
                                                                    const size_t initial_size)
{
    map->keys = (char**)malloc(initial_size * sizeof(char*));
    map->values = (duoplot_internal_BaseHandle**)malloc(initial_size * sizeof(duoplot_internal_BaseHandle*));

    map->size = initial_size;

    for (size_t k = 0U; k < initial_size; k++)
    {
        map->keys[k] = NULL;
        map->values[k] = NULL;
    }
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoGuiElementHandleContainerMap(
    duoplot_internal_GuiElementMap* const map, const char* const handle_string, duoplot_internal_BaseHandle* handle)
{
    for (size_t k = 0; k < map->size; k++)
    {
        if (map->keys[k] == NULL)
        {
            map->keys[k] = (char*)malloc((strlen(handle_string) + 1) * sizeof(char));
            map->keys[k] = strcpy(map->keys[k], handle_string);
            map->values[k] = handle;
            return;
        }
    }
    size_t new_size = map->size * 2U;
    char** tmp_keys = (char**)malloc(new_size * sizeof(char*));
    duoplot_internal_BaseHandle** tmp_values =
        (duoplot_internal_BaseHandle**)malloc(new_size * sizeof(duoplot_internal_BaseHandle*));
    for (size_t k = 0; k < new_size; k++)
    {
        if (k < map->size)
        {
            tmp_keys[k] = map->keys[k];
            tmp_values[k] = map->values[k];
        }
        else
        {
            tmp_keys[k] = NULL;
            tmp_values[k] = NULL;
        }
    }
    tmp_keys[map->size] = (char*)malloc((strlen(handle_string) + 1) * sizeof(char));
    strcpy(tmp_keys[map->size], handle_string);
    tmp_values[map->size] = handle;
    free(map->keys);
    free(map->values);
    map->keys = tmp_keys;
    map->values = tmp_values;
    map->size = new_size;
}

DUOPLOT_WEAK duoplot_internal_BaseHandle* duoplot_internal_getGuiElementHandleContainer(
    const char* const handle_string, const duoplot_internal_GuiElementMap* const map)
{
    duoplot_internal_BaseHandle* ret_val = NULL;

    if (!handle_string || !map || !(map->keys) || handle_string[0] == '\0')
    {
        return NULL;
    }
    for (size_t k = 0; k < map->size; k++)
    {
        if (map->keys[k] && (strcmp(map->keys[k], handle_string) == 0))
        {
            ret_val = map->values[k];
            break;
        }
    }

    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isGuiElementHandleContainerKeyInMap(
    const char* const handle_string, const duoplot_internal_GuiElementMap* const gui_element_handles)
{
    bool is_in_map = false;
    if (!handle_string || !gui_element_handles || !(gui_element_handles->keys) || handle_string[0] == '\0')
    {
        is_in_map = false;
    }
    else
    {
        for (size_t k = 0; k < gui_element_handles->size; k++)
        {
            if (gui_element_handles->keys[k] && (strcmp(gui_element_handles->keys[k], handle_string) == 0))
            {
                is_in_map = true;
                break;
            }
        }
    }
    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetGuiElementHandleContainerMap(duoplot_internal_GuiElementMap* const map,
                                                                     const size_t initial_size)
{
    free(map->keys);
    free(map->values);
    duoplot_internal_initGuiElementHandleContainerMap(map, initial_size);
}

// duoplot_internal_ButtonCallbackFunctionMap
DUOPLOT_WEAK void duoplot_internal_initButtonCallbackFunctionMap(duoplot_internal_ButtonCallbackFunctionMap* const map,
                                                                 const size_t initial_size)
{
    DUOPLOT_INTERNAL_INIT_MAP(map, initial_size, duoplot_ButtonCallbackFunction);
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoButtonCallbackFunctionMap(
    duoplot_internal_ButtonCallbackFunctionMap* const map,
    const char* const handle_string,
    duoplot_ButtonCallbackFunction const callback_function)
{
    DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, duoplot_ButtonCallbackFunction);
}

DUOPLOT_WEAK duoplot_ButtonCallbackFunction duoplot_internal_getButtonCallbackFunction(
    const char* const handle_string, const duoplot_internal_ButtonCallbackFunctionMap* const map)
{
    duoplot_ButtonCallbackFunction ret_val = NULL;
    DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isButtonCallbackFunctionKeyInMap(
    const char* const handle_string, const duoplot_internal_ButtonCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetButtonCallbackFunctionMap(duoplot_internal_ButtonCallbackFunctionMap* const map,
                                                                  const size_t initial_size)
{
    DUOPLOT_INTERNAL_RESET_MAP(map, initial_size, duoplot_ButtonCallbackFunction);
}

// duoplot_internal_SliderCallbackFunctionMap
DUOPLOT_WEAK void duoplot_internal_initSliderCallbackFunctionMap(duoplot_internal_SliderCallbackFunctionMap* const map,
                                                                 const size_t initial_size)
{
    DUOPLOT_INTERNAL_INIT_MAP(map, initial_size, duoplot_SliderCallbackFunction);
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoSliderCallbackFunctionMap(
    duoplot_internal_SliderCallbackFunctionMap* const map,
    const char* const handle_string,
    duoplot_SliderCallbackFunction const callback_function)
{
    DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, duoplot_SliderCallbackFunction);
}

DUOPLOT_WEAK duoplot_SliderCallbackFunction duoplot_internal_getSliderCallbackFunction(
    const char* const handle_string, const duoplot_internal_SliderCallbackFunctionMap* const map)
{
    duoplot_SliderCallbackFunction ret_val = NULL;
    DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isSliderCallbackFunctionKeyInMap(
    const char* const handle_string, const duoplot_internal_SliderCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetSliderCallbackFunctionMap(duoplot_internal_SliderCallbackFunctionMap* const map,
                                                                  const size_t initial_size)
{
    DUOPLOT_INTERNAL_RESET_MAP(map, initial_size, duoplot_SliderCallbackFunction);
}

// duoplot_internal_ListBoxCallbackFunctionMap
DUOPLOT_WEAK void duoplot_internal_initListBoxCallbackFunctionMap(
    duoplot_internal_ListBoxCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_INIT_MAP(map, initial_size, duoplot_ListBoxCallbackFunction);
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoListBoxCallbackFunctionMap(
    duoplot_internal_ListBoxCallbackFunctionMap* const map,
    const char* const handle_string,
    duoplot_ListBoxCallbackFunction const callback_function)
{
    DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, duoplot_ListBoxCallbackFunction);
}

DUOPLOT_WEAK duoplot_ListBoxCallbackFunction duoplot_internal_getListBoxCallbackFunction(
    const char* const handle_string, const duoplot_internal_ListBoxCallbackFunctionMap* const map)
{
    duoplot_ListBoxCallbackFunction ret_val = NULL;
    DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isListBoxCallbackFunctionKeyInMap(
    const char* const handle_string, const duoplot_internal_ListBoxCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetListBoxCallbackFunctionMap(
    duoplot_internal_ListBoxCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_RESET_MAP(map, initial_size, duoplot_ListBoxCallbackFunction);
}

// duoplot_internal_DropdownMenuCallbackFunctionMap
DUOPLOT_WEAK void duoplot_internal_initDropdownMenuCallbackFunctionMap(
    duoplot_internal_DropdownMenuCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_INIT_MAP(map, initial_size, duoplot_DropdownMenuCallbackFunction);
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoDropdownMenuCallbackFunctionMap(
    duoplot_internal_DropdownMenuCallbackFunctionMap* const map,
    const char* const handle_string,
    duoplot_DropdownMenuCallbackFunction const callback_function)
{
    DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(
        map, handle_string, callback_function, duoplot_DropdownMenuCallbackFunction);
}

DUOPLOT_WEAK duoplot_DropdownMenuCallbackFunction duoplot_internal_getDropdownMenuCallbackFunction(
    const char* const handle_string, const duoplot_internal_DropdownMenuCallbackFunctionMap* const map)
{
    duoplot_DropdownMenuCallbackFunction ret_val = NULL;
    DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isDropdownMenuCallbackFunctionKeyInMap(
    const char* const handle_string, const duoplot_internal_DropdownMenuCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetDropdownMenuCallbackFunctionMap(
    duoplot_internal_DropdownMenuCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_RESET_MAP(map, initial_size, duoplot_DropdownMenuCallbackFunction);
}

// duoplot_internal_CheckboxCallbackFunctionMap
DUOPLOT_WEAK void duoplot_internal_initCheckboxCallbackFunctionMap(
    duoplot_internal_CheckboxCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_INIT_MAP(map, initial_size, duoplot_CheckboxCallbackFunction);
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoCheckboxCallbackFunctionMap(
    duoplot_internal_CheckboxCallbackFunctionMap* const map,
    const char* const handle_string,
    duoplot_CheckboxCallbackFunction const callback_function)
{
    DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, duoplot_CheckboxCallbackFunction);
}

DUOPLOT_WEAK duoplot_CheckboxCallbackFunction duoplot_internal_getCheckboxCallbackFunction(
    const char* const handle_string, const duoplot_internal_CheckboxCallbackFunctionMap* const map)
{
    duoplot_CheckboxCallbackFunction ret_val = NULL;
    DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isCheckboxCallbackFunctionKeyInMap(
    const char* const handle_string, const duoplot_internal_CheckboxCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetCheckboxCallbackFunctionMap(
    duoplot_internal_CheckboxCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_RESET_MAP(map, initial_size, duoplot_CheckboxCallbackFunction);
}

// duoplot_internal_RadioButtonGroupCallbackFunctionMap
DUOPLOT_WEAK void duoplot_internal_initRadioButtonGroupCallbackFunctionMap(
    duoplot_internal_RadioButtonGroupCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_INIT_MAP(map, initial_size, duoplot_RadioButtonGroupCallbackFunction);
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoRadioButtonGroupCallbackFunctionMap(
    duoplot_internal_RadioButtonGroupCallbackFunctionMap* const map,
    const char* const handle_string,
    duoplot_RadioButtonGroupCallbackFunction const callback_function)
{
    DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(
        map, handle_string, callback_function, duoplot_RadioButtonGroupCallbackFunction);
}

DUOPLOT_WEAK duoplot_RadioButtonGroupCallbackFunction duoplot_internal_getRadioButtonGroupCallbackFunction(
    const char* const handle_string, const duoplot_internal_RadioButtonGroupCallbackFunctionMap* const map)
{
    duoplot_RadioButtonGroupCallbackFunction ret_val = NULL;
    DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isRadioButtonGroupCallbackFunctionKeyInMap(
    const char* const handle_string,
    const duoplot_internal_RadioButtonGroupCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetRadioButtonGroupCallbackFunctionMap(
    duoplot_internal_RadioButtonGroupCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_RESET_MAP(map, initial_size, duoplot_RadioButtonGroupCallbackFunction);
}

// duoplot_internal_EditableTextCallbackFunctionMap
DUOPLOT_WEAK void duoplot_internal_initEditableTextCallbackFunctionMap(
    duoplot_internal_EditableTextCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_INIT_MAP(map, initial_size, duoplot_EditableTextCallbackFunction);
}

DUOPLOT_WEAK void duoplot_internal_insertElementIntoEditableTextCallbackFunctionMap(
    duoplot_internal_EditableTextCallbackFunctionMap* const map,
    const char* const handle_string,
    duoplot_EditableTextCallbackFunction const callback_function)
{
    DUOPLOT_INTERNAL_INSERT_ELEMENT_INTO_MAP(
        map, handle_string, callback_function, duoplot_EditableTextCallbackFunction);
}

DUOPLOT_WEAK duoplot_EditableTextCallbackFunction duoplot_internal_getEditableTextCallbackFunction(
    const char* const handle_string, const duoplot_internal_EditableTextCallbackFunctionMap* const map)
{
    duoplot_EditableTextCallbackFunction ret_val = NULL;
    DUOPLOT_INTERNAL_GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

DUOPLOT_WEAK bool duoplot_internal_isEditableTextCallbackFunctionKeyInMap(
    const char* const handle_string, const duoplot_internal_EditableTextCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    DUOPLOT_INTERNAL_IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

DUOPLOT_WEAK void duoplot_internal_resetEditableTextCallbackFunctionMap(
    duoplot_internal_EditableTextCallbackFunctionMap* const map, const size_t initial_size)
{
    DUOPLOT_INTERNAL_RESET_MAP(map, initial_size, duoplot_EditableTextCallbackFunction);
}

#endif  // DUOPLOT_MAP_H
