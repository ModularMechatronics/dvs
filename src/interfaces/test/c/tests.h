#ifndef INTERFACE_TEST_C_TESTS_H
#define INTERFACE_TEST_C_TESTS_H

#include <assert.h>

#include "dvs/dvs.h"

void testInitMaps()
{
    const size_t num_elements = 20U;
    initDataStructures(num_elements);

    GuiElementMap* gui_element_map = getGuiElementHandles();

    assert(gui_element_map->size == 20U);

    for (size_t k = 0; k < gui_element_map->size; ++k)
    {
        assert(gui_element_map->keys[k] == NULL);
        assert(gui_element_map->values[k] == NULL);
    }
}

void testInsertElementIntoMap()
{
    const size_t num_elements = 20U;
    initDataStructures(num_elements);

    GuiElementMap* gui_element_map = getGuiElementHandles();

    GuiElementHandleContainer container;

    insertElementIntoGuiElementHandleContainerMap(gui_element_map, "first_element", &container);

    assert(isGuiElementHandleContainerKeyInMap("first_element", gui_element_map));
    assert(!isGuiElementHandleContainerKeyInMap("some_non_existent", gui_element_map));

    GuiElementHandleContainer* container_from_map = getGuiElementHandleContainer("first_element", gui_element_map);
    assert(container_from_map == &container);
}

void testExpandingMap()
{
    const size_t num_elements = 5U;
    initDataStructures(num_elements);

    GuiElementMap* gui_element_map = getGuiElementHandles();

    GuiElementHandleContainer container;

    size_t expected_map_size = 5U;

    for (size_t k = 0; k < 100; ++k)
    {
        char key[20];

        sprintf(key, "element_%zu", k);
        insertElementIntoGuiElementHandleContainerMap(gui_element_map, key, &container);

        const size_t num_inserted = k + 1;

        if (num_inserted > expected_map_size)
        {
            expected_map_size *= 2U;
            assert(gui_element_map->size == expected_map_size);
        }
        assert(strcmp(gui_element_map->keys[k], key) == 0);
    }

    for (size_t k = 0; k < 100; ++k)
    {
        char key[20];
        sprintf(key, "element_%zu", k);
        assert(isGuiElementHandleContainerKeyInMap(key, gui_element_map));
        GuiElementHandleContainer* container_from_map = getGuiElementHandleContainer(key, gui_element_map);
        assert(container_from_map == &container);
    }
}

void testResetMap()
{
    const size_t num_elements = 5U;
    initDataStructures(num_elements);

    GuiElementMap* gui_element_map = getGuiElementHandles();

    GuiElementHandleContainer container;

    for (size_t k = 0; k < 6; ++k)
    {
        char key[20];

        sprintf(key, "element_%zu", k);
        insertElementIntoGuiElementHandleContainerMap(gui_element_map, key, &container);
    }

    assert(gui_element_map->size == 10U);

    resetGuiElementHandleContainerMap(gui_element_map, 7U);

    assert(gui_element_map->size == 7U);
}

void testInsertSlider()
{
    const size_t num_elements = 5U;
    initDataStructures(num_elements);

    // Create slider data
    const char* const handle_string = "slider_1";

    const uint32_t min_value = 0;
    const uint32_t max_value = 100;
    const uint32_t step_size = 3;
    const uint32_t value = 5;

    UInt8Array data_view;
    data_view.data = (uint8_t*)malloc(sizeof(int32_t) * 4U);
    data_view.size = sizeof(int32_t) * 4U;

    memcpy(data_view.data, &min_value, sizeof(int32_t));
    memcpy(data_view.data + sizeof(int32_t), &max_value, sizeof(int32_t));
    memcpy(data_view.data + 2U * sizeof(int32_t), &step_size, sizeof(int32_t));
    memcpy(data_view.data + 3U * sizeof(int32_t), &value, sizeof(int32_t));

    // Create slider
    GuiElementHandleContainer* gui_element = internal_createSlider(handle_string, &data_view);

    // Push into map
    GuiElementMap* gui_element_map = getGuiElementHandles();
    insertElementIntoGuiElementHandleContainerMap(gui_element_map, handle_string, gui_element);

    // Get from map
    GuiElementHandleContainer* container_from_map = getGuiElementHandleContainer(handle_string, gui_element_map);
    SliderHandle* slider = (SliderHandle*)container_from_map->data;

    assert(container_from_map == gui_element);
    assert(container_from_map->type == GUI_ET_SLIDER);

    assert(min_value == slider->min_value);
    assert(max_value == slider->max_value);
    assert(step_size == slider->step_size);
    assert(value == slider->value);

    assert(strcmp(slider->handle_string, handle_string) == 0);
}

void runTests()
{
    testInitMaps();
    testInsertElementIntoMap();
    testResetMap();
    testExpandingMap();
    testInsertSlider();
}

#endif  // INTERFACE_TEST_C_TESTS_H
