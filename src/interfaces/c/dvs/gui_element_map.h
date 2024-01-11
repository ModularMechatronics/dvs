#ifndef DVS_MAP_H
#define DVS_MAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "dvs/internal_gui_element_handle.h"
#include "dvs/uint8_array.h"
#include "gui_element_callback_types.h"

char* createAndCopyString(const char* const str)
{
    const size_t string_length = strlen(str);

    char* new_string = malloc(string_length + 1);
    memcpy(new_string, str, string_length);

    new_string[string_length] = '\0';

    return new_string;
}

typedef struct S_GuiElementMap
{
    char** keys;
    BaseHandle** values;

    size_t size;
} GuiElementMap;

typedef struct S_ButtonCallbackMap
{
    char** keys;
    ButtonCallbackFunction* values;

    size_t size;
} ButtonCallbackFunctionMap;

typedef struct S_SliderCallbackFunctionMap
{
    char** keys;
    SliderCallbackFunction* values;

    size_t size;
} SliderCallbackFunctionMap;

typedef struct S_ListBoxCallbackFunctionMap
{
    char** keys;
    ListBoxCallbackFunction* values;

    size_t size;
} ListBoxCallbackFunctionMap;

typedef struct S_DropDownMenuCallbackFunctionMap
{
    char** keys;
    DropDownMenuCallbackFunction* values;

    size_t size;
} DropDownMenuCallbackFunctionMap;

typedef struct S_CheckboxCallbackFunctionMap
{
    char** keys;
    CheckboxCallbackFunction* values;

    size_t size;
} CheckboxCallbackFunctionMap;

typedef struct S_RadioButtonGroupCallbackFunctionMap
{
    char** keys;
    RadioButtonGroupCallbackFunction* values;

    size_t size;
} RadioButtonGroupCallbackFunctionMap;

typedef struct S_EditableTextCallbackFunctionMap
{
    char** keys;
    EditableTextCallbackFunction* values;

    size_t size;
} EditableTextCallbackFunctionMap;

#define INIT_MAP(__map, __initial_size, __map_value_type)                                 \
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

#define INSERT_ELEMENT_INTO_MAP(__map, __handle_string, __value, __map_value_type)                 \
    for (size_t k = 0; k < __map->size; k++)                                                       \
    {                                                                                              \
        if (__map->keys[k] == NULL)                                                                \
        {                                                                                          \
            __map->keys[k] = (char*)malloc((strlen(__handle_string) + 1) * sizeof(char));          \
            __map->keys[k] = strcpy(__map->keys[k], __handle_string);                              \
            __map->values[k] = __value;                                                            \
            return;                                                                                \
        }                                                                                          \
    }                                                                                              \
    size_t new_size = __map->size * 2U;                                                            \
    char** tmp_keys = (char**)malloc(new_size * sizeof(char*));                                    \
    __map_value_type* tmp_values = (__map_value_type*)malloc(new_size * sizeof(__map_value_type)); \
    for (size_t k = 0; k < new_size; k++)                                                          \
    {                                                                                              \
        if (k < __map->size)                                                                       \
        {                                                                                          \
            tmp_keys[k] = __map->keys[k];                                                          \
            tmp_values[k] = __map->values[k];                                                      \
        }                                                                                          \
        else                                                                                       \
        {                                                                                          \
            tmp_keys[k] = NULL;                                                                    \
            tmp_values[k] = NULL;                                                                  \
        }                                                                                          \
    }                                                                                              \
    tmp_keys[__map->size] = (char*)malloc((strlen(__handle_string) + 1) * sizeof(char));           \
    strcpy(tmp_keys[__map->size], __handle_string);                                                \
    tmp_values[__map->size] = __value;                                                             \
    free(__map->keys);                                                                             \
    free(__map->values);                                                                           \
    __map->keys = tmp_keys;                                                                        \
    __map->values = tmp_values;                                                                    \
    __map->size = new_size;

#define GET_ELEMENT_FROM_MAP(__map, __handle_string, __ret_val)                     \
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

#define IS_ELEMENT_IN_MAP(__map, __handle_string, __ret_val)                        \
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

#define RESET_MAP(__map, __initial_size, __map_value_type) \
    free(__map->keys);                                     \
    free(__map->values);                                   \
    INIT_MAP(__map, __initial_size, __map_value_type);

// GuiElementMap
void initGuiElementHandleContainerMap(GuiElementMap* const map, const size_t initial_size)
{
    map->keys = (char**)malloc(initial_size * sizeof(char*));
    map->values = (BaseHandle**)malloc(initial_size * sizeof(BaseHandle*));

    map->size = initial_size;

    for (size_t k = 0U; k < initial_size; k++)
    {
        map->keys[k] = NULL;
        map->values[k] = NULL;
    }
}

void insertElementIntoGuiElementHandleContainerMap(GuiElementMap* const map,
                                                   const char* const handle_string,
                                                   BaseHandle* handle)
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
    BaseHandle** tmp_values = (BaseHandle**)malloc(new_size * sizeof(BaseHandle*));
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

BaseHandle* getGuiElementHandleContainer(const char* const handle_string, const GuiElementMap* const map)
{
    BaseHandle* ret_val = NULL;

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

bool isGuiElementHandleContainerKeyInMap(const char* const handle_string,
                                         const GuiElementMap* const gui_element_handles)
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

void resetGuiElementHandleContainerMap(GuiElementMap* const map, const size_t initial_size)
{
    free(map->keys);
    free(map->values);
    initGuiElementHandleContainerMap(map, initial_size);
}

// ButtonCallbackFunctionMap
void initButtonCallbackFunctionMap(ButtonCallbackFunctionMap* const map, const size_t initial_size)
{
    INIT_MAP(map, initial_size, ButtonCallbackFunction);
}

void insertElementIntoButtonCallbackFunctionMap(ButtonCallbackFunctionMap* const map,
                                                const char* const handle_string,
                                                ButtonCallbackFunction const callback_function)
{
    INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, ButtonCallbackFunction);
}

ButtonCallbackFunction getButtonCallbackFunction(const char* const handle_string,
                                                 const ButtonCallbackFunctionMap* const map)
{
    ButtonCallbackFunction ret_val = NULL;
    GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

bool isButtonCallbackFunctionKeyInMap(const char* const handle_string,
                                      const ButtonCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

void resetButtonCallbackFunctionMap(ButtonCallbackFunctionMap* const map, const size_t initial_size)
{
    RESET_MAP(map, initial_size, ButtonCallbackFunction);
}

// SliderCallbackFunctionMap
void initSliderCallbackFunctionMap(SliderCallbackFunctionMap* const map, const size_t initial_size)
{
    INIT_MAP(map, initial_size, SliderCallbackFunction);
}

void insertElementIntoSliderCallbackFunctionMap(SliderCallbackFunctionMap* const map,
                                                const char* const handle_string,
                                                SliderCallbackFunction const callback_function)
{
    INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, SliderCallbackFunction);
}

SliderCallbackFunction getSliderCallbackFunction(const char* const handle_string,
                                                 const SliderCallbackFunctionMap* const map)
{
    SliderCallbackFunction ret_val = NULL;
    GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

bool isSliderCallbackFunctionKeyInMap(const char* const handle_string,
                                      const SliderCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

void resetSliderCallbackFunctionMap(SliderCallbackFunctionMap* const map, const size_t initial_size)
{
    RESET_MAP(map, initial_size, SliderCallbackFunction);
}

// ListBoxCallbackFunctionMap
void initListBoxCallbackFunctionMap(ListBoxCallbackFunctionMap* const map, const size_t initial_size)
{
    INIT_MAP(map, initial_size, ListBoxCallbackFunction);
}

void insertElementIntoListBoxCallbackFunctionMap(ListBoxCallbackFunctionMap* const map,
                                                 const char* const handle_string,
                                                 ListBoxCallbackFunction const callback_function)
{
    INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, ListBoxCallbackFunction);
}

ListBoxCallbackFunction getListBoxCallbackFunction(const char* const handle_string,
                                                   const ListBoxCallbackFunctionMap* const map)
{
    ListBoxCallbackFunction ret_val = NULL;
    GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

bool isListBoxCallbackFunctionKeyInMap(const char* const handle_string,
                                       const ListBoxCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

void resetListBoxCallbackFunctionMap(ListBoxCallbackFunctionMap* const map, const size_t initial_size)
{
    RESET_MAP(map, initial_size, ListBoxCallbackFunction);
}

// DropDownMenuCallbackFunctionMap
void initDropDownMenuCallbackFunctionMap(DropDownMenuCallbackFunctionMap* const map, const size_t initial_size)
{
    INIT_MAP(map, initial_size, DropDownMenuCallbackFunction);
}

void insertElementIntoDropDownMenuCallbackFunctionMap(DropDownMenuCallbackFunctionMap* const map,
                                                      const char* const handle_string,
                                                      DropDownMenuCallbackFunction const callback_function)
{
    INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, DropDownMenuCallbackFunction);
}

DropDownMenuCallbackFunction getDropDownMenuCallbackFunction(const char* const handle_string,
                                                             const DropDownMenuCallbackFunctionMap* const map)
{
    DropDownMenuCallbackFunction ret_val = NULL;
    GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

bool isDropDownMenuCallbackFunctionKeyInMap(const char* const handle_string,
                                            const DropDownMenuCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

void resetDropDownMenuCallbackFunctionMap(DropDownMenuCallbackFunctionMap* const map, const size_t initial_size)
{
    RESET_MAP(map, initial_size, DropDownMenuCallbackFunction);
}

// CheckboxCallbackFunctionMap
void initCheckboxCallbackFunctionMap(CheckboxCallbackFunctionMap* const map, const size_t initial_size)
{
    INIT_MAP(map, initial_size, CheckboxCallbackFunction);
}

void insertElementIntoCheckboxCallbackFunctionMap(CheckboxCallbackFunctionMap* const map,
                                                  const char* const handle_string,
                                                  CheckboxCallbackFunction const callback_function)
{
    INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, CheckboxCallbackFunction);
}

CheckboxCallbackFunction getCheckboxCallbackFunction(const char* const handle_string,
                                                     const CheckboxCallbackFunctionMap* const map)
{
    CheckboxCallbackFunction ret_val = NULL;
    GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

bool isCheckboxCallbackFunctionKeyInMap(const char* const handle_string,
                                        const CheckboxCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

void resetCheckboxCallbackFunctionMap(CheckboxCallbackFunctionMap* const map, const size_t initial_size)
{
    RESET_MAP(map, initial_size, CheckboxCallbackFunction);
}

// RadioButtonGroupCallbackFunctionMap
void initRadioButtonGroupCallbackFunctionMap(RadioButtonGroupCallbackFunctionMap* const map, const size_t initial_size)
{
    INIT_MAP(map, initial_size, RadioButtonGroupCallbackFunction);
}

void insertElementIntoRadioButtonGroupCallbackFunctionMap(RadioButtonGroupCallbackFunctionMap* const map,
                                                          const char* const handle_string,
                                                          RadioButtonGroupCallbackFunction const callback_function)
{
    INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, RadioButtonGroupCallbackFunction);
}

RadioButtonGroupCallbackFunction getRadioButtonGroupCallbackFunction(
    const char* const handle_string, const RadioButtonGroupCallbackFunctionMap* const map)
{
    RadioButtonGroupCallbackFunction ret_val = NULL;
    GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

bool isRadioButtonGroupCallbackFunctionKeyInMap(const char* const handle_string,
                                                const RadioButtonGroupCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

void resetRadioButtonGroupCallbackFunctionMap(RadioButtonGroupCallbackFunctionMap* const map, const size_t initial_size)
{
    RESET_MAP(map, initial_size, RadioButtonGroupCallbackFunction);
}

// EditableTextCallbackFunctionMap
void initEditableTextCallbackFunctionMap(EditableTextCallbackFunctionMap* const map, const size_t initial_size)
{
    INIT_MAP(map, initial_size, EditableTextCallbackFunction);
}

void insertElementIntoEditableTextCallbackFunctionMap(EditableTextCallbackFunctionMap* const map,
                                                      const char* const handle_string,
                                                      EditableTextCallbackFunction const callback_function)
{
    INSERT_ELEMENT_INTO_MAP(map, handle_string, callback_function, EditableTextCallbackFunction);
}

EditableTextCallbackFunction getEditableTextCallbackFunction(const char* const handle_string,
                                                             const EditableTextCallbackFunctionMap* const map)
{
    EditableTextCallbackFunction ret_val = NULL;
    GET_ELEMENT_FROM_MAP(map, handle_string, ret_val);
    return ret_val;
}

bool isEditableTextCallbackFunctionKeyInMap(const char* const handle_string,
                                            const EditableTextCallbackFunctionMap* const gui_element_handles)
{
    bool is_in_map = false;

    IS_ELEMENT_IN_MAP(gui_element_handles, handle_string, is_in_map);

    return is_in_map;
}

void resetEditableTextCallbackFunctionMap(EditableTextCallbackFunctionMap* const map, const size_t initial_size)
{
    RESET_MAP(map, initial_size, EditableTextCallbackFunction);
}

#endif  // DVS_MAP_H
