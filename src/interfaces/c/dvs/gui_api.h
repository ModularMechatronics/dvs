#ifndef DVS_GUI_API_H
#define DVS_GUI_API_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "dvs/gui_internal.h"

ButtonHandle getButtonHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    ButtonHandle button_handle;
    button_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return button_handle;
    }

    ButtonInternalHandle* const gui_elem =
        (ButtonInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_BUTTON)
    {
        printf("Gui element with handle string %s is not a button!\n", handle_string);
        return button_handle;
    }

    button_handle.__handle = gui_elem;

    return button_handle;
}

SliderHandle getSliderHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    SliderHandle slider_handle;
    slider_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return slider_handle;
    }

    SliderInternalHandle* const gui_elem =
        (SliderInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_SLIDER)
    {
        printf("Gui element with handle string %s is not a slider!\n", handle_string);
        return slider_handle;
    }

    slider_handle.__handle = gui_elem;

    return slider_handle;
}

int32_t getSliderValue(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.value;
}

int32_t getSliderMinValue(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.min_value;
}

int32_t getSliderMaxValue(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.max_value;
}

int32_t getSliderStepSize(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.step_size;
}

SliderState getSliderState(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        SliderState slider_state;
        slider_state.value = 0;
        slider_state.min_value = 0;
        slider_state.max_value = 0;
        slider_state.step_size = 0;

        return slider_state;
    }

    return slider_handle.__handle->state;
}

void* queryThreadFunction(void* vargp)
{
    usleep(1000U * 100U);
    internal_queryForSyncOfGuiData();

    return NULL;
}

void* receiveThreadFunction(void* vargp)
{
    // TODO: Fix case where client app has already been updated once,
    // and then dvs restarts and tries to submit gui data again.
    // Preferebly that should just be handled by this loop, as
    // any normal gui element update.

    while (true)
    {
        // internal_receiveGuiData is a blocking method
        const ReceivedGuiData received_data = internal_receiveGuiData();
        updateGuiState(&received_data);
        /*const internal::ReceivedGuiData received_data{internal::receiveGuiData()};

        // updateGuiState must execute before callGuiCallbackFunction to properly
        // update the gui element state before calling the callback function.
        internal::updateGuiState(received_data);
        internal::callGuiCallbackFunction(received_data);*/

        free(received_data.data);
    }

    return NULL;
}

__attribute__((weak)) void startGuiReceiveThread()
{
    initDataStructures(20U);

    internal_initTcpSocket();

    if (internal_isDvsRunning())
    {
        printf("DVS is running, starting GUI receive thread!\n");
        pthread_t query_thread;

        pthread_create(&query_thread, NULL, queryThreadFunction, NULL);
        pthread_detach(query_thread);
    }
    else
    {
        printf("DVS is not running, not starting GUI receive thread...\n");
    }

    internal_waitForSyncForAllGuiElements();

    pthread_t receive_thread;

    pthread_create(&receive_thread, NULL, receiveThreadFunction, NULL);
    pthread_detach(receive_thread);
}

#endif  // DVS_GUI_API_H
