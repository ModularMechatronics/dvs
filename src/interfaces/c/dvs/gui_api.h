#ifndef DVS_GUI_API_H
#define DVS_GUI_API_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "dvs/gui_internal.h"

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

SliderState getSliderCurrentState(const SliderHandle slider_handle)
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

ListOfStrings getListBoxElements(const ListBoxHandle list_box_handle)
{
    ListOfStrings list_of_strings;

    if (list_box_handle.__handle == NULL)
    {
        printf("List box handle is NULL!\n");

        list_of_strings.elements = NULL;
        list_of_strings.size = 0;

        return list_of_strings;
    }

    return list_box_handle.__handle->state.elements;
}

char* getListBoxSelectedElement(const ListBoxHandle list_box_handle)
{
    if (list_box_handle.__handle == NULL)
    {
        printf("List box handle is NULL!\n");
        return NULL;
    }

    return list_box_handle.__handle->state.selected_element;
}

char* getDropDownMenuSelectedElement(const DropDownMenuHandle drop_down_menu_handle)
{
    if (drop_down_menu_handle.__handle == NULL)
    {
        printf("Drop down menu handle is NULL!\n");
        return NULL;
    }

    return drop_down_menu_handle.__handle->state.selected_element;
}

ListOfStrings getDropDownMenuElements(const DropDownMenuHandle drop_down_menu_handle)
{
    ListOfStrings list_of_strings;

    if (drop_down_menu_handle.__handle == NULL)
    {
        printf("Drop down menu handle is NULL!\n");

        list_of_strings.elements = NULL;
        list_of_strings.size = 0;

        return list_of_strings;
    }

    return drop_down_menu_handle.__handle->state.elements;
}

ListOfStrings getRadioButtonGroupButtonNames(const RadioButtonGroupHandle radio_button_group_handle)
{
    ListOfStrings list_of_strings;

    if (radio_button_group_handle.__handle == NULL)
    {
        printf("Radio button group handle is NULL!\n");

        list_of_strings.elements = NULL;
        list_of_strings.size = 0;

        return list_of_strings;
    }

    return radio_button_group_handle.__handle->buttons;
}

int32_t getRadioButtonGroupSelectedButtonIdx(const RadioButtonGroupHandle radio_button_group_handle)
{
    if (radio_button_group_handle.__handle == NULL)
    {
        printf("Radio button group handle is NULL!\n");
        return -1;
    }

    return radio_button_group_handle.__handle->selected_button_idx;
}

char* getEditableTextValue(const EditableTextHandle editable_text_handle)
{
    if (editable_text_handle.__handle == NULL)
    {
        printf("Editable text handle is NULL!\n");
        return NULL;
    }

    return editable_text_handle.__handle->text;
}

bool getIsCheckBoxChecked(const CheckboxHandle checkbox_handle)
{
    if (checkbox_handle.__handle == NULL)
    {
        printf("Checkbox handle is NULL!\n");
        return false;
    }

    return checkbox_handle.__handle->is_checked;
}

char* internal_getNullString()
{
    static char* null_str = "NULL";
    return null_str;
}

#define GET_HANDLE_STRING(handle) (handle.__handle == NULL ? internal_getNullString() : handle.__handle->handle_string)

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

        // updateGuiState must execute before callGuiCallbackFunction to properly
        // update the gui element state before calling the callback function.
        updateGuiState(&received_data);
        callGuiCallbackFunction(&received_data);

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
