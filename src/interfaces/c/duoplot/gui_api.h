#ifndef DUOPLOT_GUI_API_H
#define DUOPLOT_GUI_API_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "duoplot/gui_internal.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK int32_t getSliderValue(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.value;
}

DUOPLOT_WEAK int32_t getSliderMinValue(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.min_value;
}

DUOPLOT_WEAK int32_t getSliderMaxValue(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.max_value;
}

DUOPLOT_WEAK int32_t getSliderStepSize(const SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.step_size;
}

DUOPLOT_WEAK SliderState getSliderCurrentState(const SliderHandle slider_handle)
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

DUOPLOT_WEAK ListOfStrings getListBoxElements(const ListBoxHandle list_box_handle)
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

DUOPLOT_WEAK char* getListBoxSelectedElement(const ListBoxHandle list_box_handle)
{
    if (list_box_handle.__handle == NULL)
    {
        printf("List box handle is NULL!\n");
        return NULL;
    }

    return list_box_handle.__handle->state.selected_element;
}

DUOPLOT_WEAK char* getDropdownMenuSelectedElement(const DropdownMenuHandle drop_down_menu_handle)
{
    if (drop_down_menu_handle.__handle == NULL)
    {
        printf("Drop down menu handle is NULL!\n");
        return NULL;
    }

    return drop_down_menu_handle.__handle->state.selected_element;
}

DUOPLOT_WEAK ListOfStrings getDropdownMenuElements(const DropdownMenuHandle drop_down_menu_handle)
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

DUOPLOT_WEAK ListOfStrings getRadioButtonGroupButtonNames(const RadioButtonGroupHandle radio_button_group_handle)
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

DUOPLOT_WEAK int32_t getRadioButtonGroupSelectedButtonIdx(const RadioButtonGroupHandle radio_button_group_handle)
{
    if (radio_button_group_handle.__handle == NULL)
    {
        printf("Radio button group handle is NULL!\n");
        return -1;
    }

    return radio_button_group_handle.__handle->selected_button_idx;
}

DUOPLOT_WEAK char* getEditableTextValue(const EditableTextHandle editable_text_handle)
{
    if (editable_text_handle.__handle == NULL)
    {
        printf("Editable text handle is NULL!\n");
        return NULL;
    }

    return editable_text_handle.__handle->text;
}

DUOPLOT_WEAK char* getTextLabelValue(const TextLabelHandle text_label_handle)
{
    if (text_label_handle.__handle == NULL)
    {
        printf("Text label handle is NULL!\n");
        return NULL;
    }

    return text_label_handle.__handle->text;
}

DUOPLOT_WEAK bool getIsCheckBoxChecked(const CheckboxHandle checkbox_handle)
{
    if (checkbox_handle.__handle == NULL)
    {
        printf("Checkbox handle is NULL!\n");
        return false;
    }

    return checkbox_handle.__handle->is_checked;
}

DUOPLOT_WEAK void setTextLabelValue(const TextLabelHandle text_label_handle, const char* const new_value)
{
    if (text_label_handle.__handle == NULL)
    {
        printf("Text label handle is NULL!\n");
        return;
    }
    else if (new_value == NULL)
    {
        printf("New value is NULL!\n");
        return;
    }

    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_SET_GUI_ELEMENT_LABEL);

    {
        CommunicationHeaderObject* const current_obj = hdr.objects + hdr.obj_idx;

        current_obj->type = CHOT_LABEL;

        const size_t label_length = strnlen(new_value, 100U);
        current_obj->num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + (uint8_t)label_length;

        memset(current_obj->data, 0, kMaxNumFunctionHeaderBytes);

        current_obj->data[0U] = PT_NAME;
        current_obj->data[1U] = label_length;
        memcpy(current_obj->data + 2U, new_value, label_length);

        appendObjectIndexToCommunicationHeaderObjectLookupTable(&(hdr.objects_lut), CHOT_LABEL, hdr.obj_idx);

        hdr.obj_idx += 1;
    }

    {
        CommunicationHeaderObject* const current_obj = hdr.objects + hdr.obj_idx;

        current_obj->type = CHOT_HANDLE_STRING;

        const size_t handle_string_length = strnlen(text_label_handle.__handle->handle_string, 100U);
        current_obj->num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + (uint8_t)handle_string_length;

        memset(current_obj->data, 0, kMaxNumFunctionHeaderBytes);

        current_obj->data[0U] = PT_NAME;
        current_obj->data[1U] = handle_string_length;
        memcpy(current_obj->data + 2U, text_label_handle.__handle->handle_string, handle_string_length);

        appendObjectIndexToCommunicationHeaderObjectLookupTable(&(hdr.objects_lut), CHOT_HANDLE_STRING, hdr.obj_idx);

        hdr.obj_idx += 1;
    }

    sendHeader(getSendFunction(), &hdr);
}

DUOPLOT_WEAK char* internal_getNullString()
{
    static char* null_str = "NULL";
    return null_str;
}

#define GET_HANDLE_STRING(handle) (handle.__handle == NULL ? internal_getNullString() : handle.__handle->handle_string)

DUOPLOT_WEAK void* queryThreadFunction(void* vargp)
{
    usleep(1000U * 100U);
    internal_queryForSyncOfGuiData();

    return NULL;
}

DUOPLOT_WEAK void* receiveThreadFunction(void* vargp)
{
    // TODO: Fix case where client app has already been updated once,
    // and then duoplot restarts and tries to submit gui data again.
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

DUOPLOT_WEAK void startGuiReceiveThread()
{
    initDataStructures(20U);

    internal_initTcpSocket();

    if (internal_isDuoplotRunning())
    {
        printf("duoplot is running, starting GUI receive thread!\n");
        pthread_t query_thread;

        pthread_create(&query_thread, NULL, queryThreadFunction, NULL);
        pthread_detach(query_thread);
    }
    else
    {
        printf("duoplot is not running, not starting GUI receive thread...\n");
    }

    internal_waitForSyncForAllGuiElements();

    pthread_t receive_thread;

    pthread_create(&receive_thread, NULL, receiveThreadFunction, NULL);
    pthread_detach(receive_thread);
}

#endif  // DUOPLOT_GUI_API_H
