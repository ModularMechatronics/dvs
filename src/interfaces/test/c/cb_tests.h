#ifndef INTERFACE_TEST_C_CB_TESTS_H
#define INTERFACE_TEST_C_CB_TESTS_H

#include <assert.h>

#include "dvs/dvs.h"

void testCbInitMaps()
{
    /*const size_t num_elements = 20U;
    initDataStructures(num_elements);

    GuiElementMap* gui_element_map = getGuiElementHandles();

    assert(gui_element_map->size == 20U);

    for (size_t k = 0; k < gui_element_map->size; ++k)
    {
        assert(gui_element_map->keys[k] == NULL);
        assert(gui_element_map->values[k] == NULL);
    }*/
}

bool variable_to_manipulate;

void button0Callback(const ButtonHandle gui_element_handle)
{
    printf("Callback function: \"button0\" pressed\n");
    variable_to_manipulate = true;
}

void testRegisterButtonCallback()
{
    variable_to_manipulate = false;
    const size_t num_elements = 20U;
    initDataStructures(num_elements);

    registerButtonCallback("button0", button0Callback);

    ButtonCallbackFunctionMap* const button_callback_function_map = getButtonCallbackFunctionMap();

    assert(isButtonCallbackFunctionKeyInMap("button0", button_callback_function_map));

    ButtonCallbackFunction cb_fun = getButtonCallbackFunction("button0", button_callback_function_map);

    assert(cb_fun != NULL);

    variable_to_manipulate = false;
    ButtonHandle gui_element_handle;
    gui_element_handle.__handle = NULL;

    cb_fun(gui_element_handle);
    assert(variable_to_manipulate);
}

void runCbTests()
{
    testCbInitMaps();
    testRegisterButtonCallback();
}

#endif  // INTERFACE_TEST_C_CB_TESTS_H
