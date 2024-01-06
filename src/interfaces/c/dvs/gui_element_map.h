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
    GuiElementHandleContainer** values;

    size_t size;
} GuiElementMap;

typedef struct S_ButtonCallbackMap
{
    char** keys;
    ButtonCallbackFunction* values;

    size_t size;
} ButtonCallbackFunctionMap;

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
    map->values = (GuiElementHandleContainer**)malloc(initial_size * sizeof(GuiElementHandleContainer*));

    map->size = initial_size;

    for (size_t k = 0U; k < initial_size; k++)
    {
        map->keys[k] = NULL;
        map->values[k] = NULL;
    }
}

void insertElementIntoGuiElementHandleContainerMap(GuiElementMap* const map,
                                                   const char* const handle_string,
                                                   GuiElementHandleContainer* container)
{
    for (size_t k = 0; k < map->size; k++)
    {
        if (map->keys[k] == NULL)
        {
            map->keys[k] = (char*)malloc((strlen(handle_string) + 1) * sizeof(char));
            map->keys[k] = strcpy(map->keys[k], handle_string);
            map->values[k] = container;
            return;
        }
    }
    size_t new_size = map->size * 2U;
    char** tmp_keys = (char**)malloc(new_size * sizeof(char*));
    GuiElementHandleContainer** tmp_values =
        (GuiElementHandleContainer**)malloc(new_size * sizeof(GuiElementHandleContainer*));
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
    tmp_values[map->size] = container;
    free(map->keys);
    free(map->values);
    map->keys = tmp_keys;
    map->values = tmp_values;
    map->size = new_size;
}

GuiElementHandleContainer* getGuiElementHandleContainer(const char* const handle_string, const GuiElementMap* const map)
{
    GuiElementHandleContainer* ret_val = NULL;

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

#endif  // DVS_MAP_H
