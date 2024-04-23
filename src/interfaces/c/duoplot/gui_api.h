#ifndef DUOPLOT_GUI_API_H
#define DUOPLOT_GUI_API_H

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "duoplot/gui_internal.h"
#include "duoplot/pp.h"

DUOPLOT_WEAK int32_t duoplot_getSliderValue(const duoplot_SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.value;
}

DUOPLOT_WEAK int32_t duoplot_getSliderMinValue(const duoplot_SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.min_value;
}

DUOPLOT_WEAK int32_t duoplot_getSliderMaxValue(const duoplot_SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.max_value;
}

DUOPLOT_WEAK int32_t duoplot_getSliderStepSize(const duoplot_SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        return 0;
    }

    return slider_handle.__handle->state.step_size;
}

DUOPLOT_WEAK duoplot_SliderState duoplot_getSliderCurrentState(const duoplot_SliderHandle slider_handle)
{
    if (slider_handle.__handle == NULL)
    {
        printf("Slider handle is NULL!\n");
        duoplot_SliderState slider_state;
        slider_state.value = 0;
        slider_state.min_value = 0;
        slider_state.max_value = 0;
        slider_state.step_size = 0;

        return slider_state;
    }

    return slider_handle.__handle->state;
}

DUOPLOT_WEAK duoplot_ListOfStrings duoplot_getListBoxElements(const duoplot_ListBoxHandle list_box_handle)
{
    duoplot_ListOfStrings list_of_strings;

    if (list_box_handle.__handle == NULL)
    {
        printf("List box handle is NULL!\n");

        list_of_strings.elements = NULL;
        list_of_strings.size = 0;

        return list_of_strings;
    }

    return list_box_handle.__handle->state.elements;
}

DUOPLOT_WEAK char* duoplot_getListBoxSelectedElement(const duoplot_ListBoxHandle list_box_handle)
{
    if (list_box_handle.__handle == NULL)
    {
        printf("List box handle is NULL!\n");
        return NULL;
    }

    return list_box_handle.__handle->state.selected_element;
}

DUOPLOT_WEAK char* duoplot_getDropdownMenuSelectedElement(const duoplot_DropdownMenuHandle drop_down_menu_handle)
{
    if (drop_down_menu_handle.__handle == NULL)
    {
        printf("Drop down menu handle is NULL!\n");
        return NULL;
    }

    return drop_down_menu_handle.__handle->state.selected_element;
}

DUOPLOT_WEAK duoplot_ListOfStrings
duoplot_getDropdownMenuElements(const duoplot_DropdownMenuHandle drop_down_menu_handle)
{
    duoplot_ListOfStrings list_of_strings;

    if (drop_down_menu_handle.__handle == NULL)
    {
        printf("Drop down menu handle is NULL!\n");

        list_of_strings.elements = NULL;
        list_of_strings.size = 0;

        return list_of_strings;
    }

    return drop_down_menu_handle.__handle->state.elements;
}

DUOPLOT_WEAK duoplot_ListOfStrings
duoplot_getRadioButtonGroupButtonNames(const duoplot_RadioButtonGroupHandle radio_button_group_handle)
{
    duoplot_ListOfStrings list_of_strings;

    if (radio_button_group_handle.__handle == NULL)
    {
        printf("Radio button group handle is NULL!\n");

        list_of_strings.elements = NULL;
        list_of_strings.size = 0;

        return list_of_strings;
    }

    return radio_button_group_handle.__handle->buttons;
}

DUOPLOT_WEAK int32_t
duoplot_getRadioButtonGroupSelectedButtonIdx(const duoplot_RadioButtonGroupHandle radio_button_group_handle)
{
    if (radio_button_group_handle.__handle == NULL)
    {
        printf("Radio button group handle is NULL!\n");
        return -1;
    }

    return radio_button_group_handle.__handle->selected_button_idx;
}

DUOPLOT_WEAK char* duoplot_getEditableTextValue(const duoplot_EditableTextHandle editable_text_handle)
{
    if (editable_text_handle.__handle == NULL)
    {
        printf("Editable text handle is NULL!\n");
        return NULL;
    }

    return editable_text_handle.__handle->text;
}

DUOPLOT_WEAK char* duoplot_getTextLabelValue(const duoplot_TextLabelHandle text_label_handle)
{
    if (text_label_handle.__handle == NULL)
    {
        printf("Text label handle is NULL!\n");
        return NULL;
    }

    return text_label_handle.__handle->text;
}

DUOPLOT_WEAK bool duoplot_getIsCheckBoxChecked(const duoplot_CheckboxHandle checkbox_handle)
{
    if (checkbox_handle.__handle == NULL)
    {
        printf("Checkbox handle is NULL!\n");
        return false;
    }

    return checkbox_handle.__handle->is_checked;
}

DUOPLOT_WEAK void duoplot_setTextLabelValue(const duoplot_TextLabelHandle text_label_handle,
                                            const char* const new_value)
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

    duoplot_internal_CommunicationHeader hdr;
    duoplot_internal_initCommunicationHeader(&hdr, DUOPLOT_INTERNAL_F_SET_GUI_ELEMENT_LABEL);

    {
        duoplot_internal_CommunicationHeaderObject* const current_obj = hdr.objects + hdr.obj_idx;

        current_obj->type = DUOPLOT_INTERNAL_CHOT_LABEL;

        const size_t label_length = strnlen(new_value, 100U);
        current_obj->num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + (uint8_t)label_length;

        memset(current_obj->data, 0, DUOPLOT_INTERNAL_MAX_NUM_FUNCTION_HEADER_BYTES);

        current_obj->data[0U] = DUOPLOT_INTERNAL_PT_NAME;
        current_obj->data[1U] = label_length;
        memcpy(current_obj->data + 2U, new_value, label_length);

        duoplot_internal_appendObjectIndexToCommunicationHeaderObjectLookupTable(
            &(hdr.objects_lut), DUOPLOT_INTERNAL_CHOT_LABEL, hdr.obj_idx);

        hdr.obj_idx += 1;
    }

    {
        duoplot_internal_CommunicationHeaderObject* const current_obj = hdr.objects + hdr.obj_idx;

        current_obj->type = DUOPLOT_INTERNAL_CHOT_HANDLE_STRING;

        const size_t handle_string_length = strnlen(text_label_handle.__handle->handle_string, 100U);
        current_obj->num_bytes = sizeof(uint8_t) + sizeof(uint8_t) + (uint8_t)handle_string_length;

        memset(current_obj->data, 0, DUOPLOT_INTERNAL_MAX_NUM_FUNCTION_HEADER_BYTES);

        current_obj->data[0U] = DUOPLOT_INTERNAL_PT_NAME;
        current_obj->data[1U] = handle_string_length;
        memcpy(current_obj->data + 2U, text_label_handle.__handle->handle_string, handle_string_length);

        duoplot_internal_appendObjectIndexToCommunicationHeaderObjectLookupTable(
            &(hdr.objects_lut), DUOPLOT_INTERNAL_CHOT_HANDLE_STRING, hdr.obj_idx);

        hdr.obj_idx += 1;
    }

    duoplot_internal_sendHeader(duoplot_internal_getSendFunction(), &hdr);
}

DUOPLOT_WEAK char* duoplot_internal_getNullString()
{
    static char* null_str = "NULL";
    return null_str;
}

#define DUOPLOT_GET_HANDLE_STRING(handle) \
    (handle.__handle == NULL ? duoplot_internal_getNullString() : handle.__handle->handle_string)

DUOPLOT_WEAK void* duoplot_internal_queryThreadFunction(void* vargp)
{
    usleep(1000U * 100U);
    duoplot_internal_queryForSyncOfGuiData();

    return NULL;
}

DUOPLOT_WEAK void* duoplot_internal_receiveThreadFunction(void* vargp)
{
    // TODO: Fix case where client app has already been updated once,
    // and then duoplot restarts and tries to submit gui data again.
    // Preferebly that should just be handled by this loop, as
    // any normal gui element update.

    while (true)
    {
        // duoplot_internal_receiveGuiData is a blocking method
        const duoplot_internal_ReceivedGuiData received_data = duoplot_internal_receiveGuiData();

        // duoplot_internal_updateGuiState must execute before duoplot_internal_callGuiCallbackFunction to properly
        // update the gui element state before calling the callback function.
        duoplot_internal_updateGuiState(&received_data);
        duoplot_internal_callGuiCallbackFunction(&received_data);

        free(received_data.data);
    }

    return NULL;
}

DUOPLOT_WEAK void duoplot_startGuiReceiveThread()
{
    duoplot_internal_initDataStructures(20U);

    duoplot_internal_initTcpSocket();

    if (duoplot_internal_isDuoplotRunning())
    {
        printf("duoplot is running, starting GUI receive thread!\n");
        pthread_t query_thread;

        pthread_create(&query_thread, NULL, duoplot_internal_queryThreadFunction, NULL);
        pthread_detach(query_thread);
    }
    else
    {
        printf("duoplot is not running, not starting GUI receive thread...\n");
    }

    duoplot_internal_waitForSyncForAllGuiElements();

    pthread_t receive_thread;

    pthread_create(&receive_thread, NULL, duoplot_internal_receiveThreadFunction, NULL);
    pthread_detach(receive_thread);
}

#endif  // DUOPLOT_GUI_API_H
