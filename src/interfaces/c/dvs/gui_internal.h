#ifndef DVS_GUI_INTERNAL_H
#define DVS_GUI_INTERNAL_H

#include <arpa/inet.h>
#include <errno.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "dvs/constants.h"
#include "dvs/gui_element_map.h"
#include "dvs/internal.h"

int* internal_getTcpSocket()
{
    static int tcp_sockfd;

    return &tcp_sockfd;
}

void internal_initTcpSocket()
{
    int* tcp_sockfd = internal_getTcpSocket();
    struct sockaddr_in tcp_servaddr;

    *tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Set reuse address that's already in use (probably by exited dvs instance)
    int true_val = 1;
    setsockopt(*tcp_sockfd, SOL_SOCKET, SO_REUSEADDR, &true_val, sizeof(int));

    bzero(&tcp_servaddr, sizeof(tcp_servaddr));

    tcp_servaddr.sin_family = AF_INET;
    tcp_servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    tcp_servaddr.sin_port = htons(GUI_TCP_PORT_NUM);

    int ret_val = bind(*tcp_sockfd, (struct sockaddr*)&tcp_servaddr, sizeof(tcp_servaddr));

    if (ret_val != 0)
    {
        printf("Socket bind failed...\n");
        exit(-1);
    }

    if ((listen(*tcp_sockfd, 5)) != 0)
    {
        printf("Socket listen failed...\n");
        exit(-1);
    }
}

void internal_queryForSyncOfGuiData()
{
    CommunicationHeader hdr;
    initCommunicationHeader(&hdr, F_QUERY_FOR_SYNC_OF_GUI_DATA);

    sendHeader(getSendFunction(), &hdr);
}

typedef struct S_ReceivedGuiData
{
    uint8_t* data;
    uint64_t num_data_bytes;
} ReceivedGuiData;

ReceivedGuiData internal_receiveGuiData()
{
    const int* const tcp_sockfd = internal_getTcpSocket();

    struct sockaddr_in tcp_cli;
    socklen_t tcp_len = sizeof(tcp_cli);

    const int tcp_connfd = accept(*tcp_sockfd, (struct sockaddr*)&tcp_cli, &tcp_len);
    if (tcp_connfd < 0)
    {
        printf("Server accept failed...\n");
    }

    uint64_t num_expected_bytes;
    read(tcp_connfd, &num_expected_bytes, sizeof(uint64_t));

    ReceivedGuiData received_data;

    received_data.data = malloc(num_expected_bytes);
    received_data.num_data_bytes = num_expected_bytes;

    char* rec_buffer = (char*)(received_data.data);

    size_t total_num_received_bytes = 0U;
    size_t num_bytes_left = num_expected_bytes;

    while (true)
    {
        const ssize_t num_received_bytes = read(tcp_connfd, rec_buffer + total_num_received_bytes, num_bytes_left);

        total_num_received_bytes += num_received_bytes;
        num_bytes_left -= (size_t)(num_received_bytes);

        if (total_num_received_bytes >= num_expected_bytes)
        {
            break;
        }
    }

    close(tcp_connfd);

    return received_data;
}

GuiElementMap* getGuiElementHandles()
{
    static GuiElementMap gui_element_handles;

    return &gui_element_handles;
}

ButtonCallbackFunctionMap* getButtonCallbackFunctionMap()
{
    static ButtonCallbackFunctionMap button_callback_function_map;

    return &button_callback_function_map;
}

SliderCallbackFunctionMap* getSliderCallbackFunctionMap()
{
    static SliderCallbackFunctionMap slider_callback_function_map;

    return &slider_callback_function_map;
}

ListBoxCallbackFunctionMap* getListBoxCallbackFunctionMap()
{
    static ListBoxCallbackFunctionMap list_box_handle_callback_function_map;

    return &list_box_handle_callback_function_map;
}

DropdownMenuCallbackFunctionMap* getDropdownMenuCallbackFunctionMap()
{
    static DropdownMenuCallbackFunctionMap dropdown_menu_handle_callback_function_map;

    return &dropdown_menu_handle_callback_function_map;
}

CheckboxCallbackFunctionMap* getCheckboxCallbackFunctionMap()
{
    static CheckboxCallbackFunctionMap checkbox_handle_callback_function_map;

    return &checkbox_handle_callback_function_map;
}

RadioButtonGroupCallbackFunctionMap* getRadioButtonGroupCallbackFunctionMap()
{
    static RadioButtonGroupCallbackFunctionMap radiobutton_group_handle_callback_function_map;

    return &radiobutton_group_handle_callback_function_map;
}

EditableTextCallbackFunctionMap* getEditableTextCallbackFunctionMap()
{
    static EditableTextCallbackFunctionMap editable_text_handle_callback_function_map;

    return &editable_text_handle_callback_function_map;
}

void initDataStructures(const size_t initial_size)
{
    GuiElementMap* const gui_element_handles = getGuiElementHandles();
    initGuiElementHandleContainerMap(gui_element_handles, initial_size);

    ButtonCallbackFunctionMap* const button_callback_function_map = getButtonCallbackFunctionMap();
    initButtonCallbackFunctionMap(button_callback_function_map, initial_size);

    SliderCallbackFunctionMap* const slider_callback_function_map = getSliderCallbackFunctionMap();
    initSliderCallbackFunctionMap(slider_callback_function_map, initial_size);

    ListBoxCallbackFunctionMap* const list_box_handle_callback_function_map = getListBoxCallbackFunctionMap();
    initListBoxCallbackFunctionMap(list_box_handle_callback_function_map, initial_size);

    DropdownMenuCallbackFunctionMap* const dropdown_menu_handle_callback_function_map =
        getDropdownMenuCallbackFunctionMap();
    initDropdownMenuCallbackFunctionMap(dropdown_menu_handle_callback_function_map, initial_size);

    CheckboxCallbackFunctionMap* const checkbox_handle_callback_function_map = getCheckboxCallbackFunctionMap();
    initCheckboxCallbackFunctionMap(checkbox_handle_callback_function_map, initial_size);

    RadioButtonGroupCallbackFunctionMap* const radiobutton_group_handle_callback_function_map =
        getRadioButtonGroupCallbackFunctionMap();
    initRadioButtonGroupCallbackFunctionMap(radiobutton_group_handle_callback_function_map, initial_size);

    EditableTextCallbackFunctionMap* const editable_text_handle_callback_function_map =
        getEditableTextCallbackFunctionMap();
    initEditableTextCallbackFunctionMap(editable_text_handle_callback_function_map, initial_size);
}

void updateButtonState(ButtonInternalHandle* const handle, const UInt8Array* data_view)
{
    handle->is_pressed = data_view->data[0];
}

void updateSliderState(SliderInternalHandle* const handle, const UInt8Array* data_view)
{
    memcpy(&handle->state.min_value, data_view->data, sizeof(int32_t));
    memcpy(&handle->state.max_value, data_view->data + sizeof(int32_t), sizeof(int32_t));
    memcpy(&handle->state.step_size, data_view->data + 2U * sizeof(int32_t), sizeof(int32_t));
    memcpy(&handle->state.value, data_view->data + 3U * sizeof(int32_t), sizeof(int32_t));
}

void updateTextLabelState(TextLabelInternalHandle* const handle, const UInt8Array* data_view)
{
    const uint8_t text_length = data_view->data[0];

    handle->text = (char*)malloc(text_length + 1U);

    size_t idx = 1U;

    memcpy(handle->text, data_view->data + idx, text_length);
    handle->text[text_length] = '\0';
}

void updateListBoxState(ListBoxInternalHandle* const handle, const UInt8Array* data_view)
{
    ListBoxState* const state = &(handle->state);
    destroyListOfStrings(&(state->elements));
    free(state->selected_element);

    const uint8_t selected_element_length = data_view->data[0];

    size_t idx = 1U;

    if (selected_element_length > 0U)
    {
        state->selected_element = (char*)malloc(selected_element_length + 1U);
        memcpy(state->selected_element, data_view->data + idx, selected_element_length);
        state->selected_element[selected_element_length] = '\0';
        idx += selected_element_length;
    }
    else
    {
        state->selected_element = (char*)malloc(1U);
        state->selected_element[0] = '\0';
    }

    uint16_t num_elements;

    memcpy(&num_elements, data_view->data + idx, sizeof(uint16_t));

    idx += sizeof(uint16_t);

    state->elements = createListOfStrings(num_elements);

    for (uint16_t i = 0U; i < num_elements; i++)
    {
        const uint8_t element_length = data_view->data[idx];
        idx++;

        state->elements.elements[i] = (char*)malloc(element_length + 1U);
        memcpy(state->elements.elements[i], data_view->data + idx, element_length);
        state->elements.elements[i][element_length] = '\0';

        idx += element_length;
    }
}

void updateDropdownMenuState(DropdownMenuInternalHandle* const handle, const UInt8Array* data_view)
{
    DropdownMenuState* const state = &(handle->state);
    destroyListOfStrings(&(state->elements));
    free(state->selected_element);

    const uint8_t selected_element_length = data_view->data[0];

    size_t idx = 1U;

    if (selected_element_length > 0U)
    {
        state->selected_element = (char*)malloc(selected_element_length + 1U);
        memcpy(state->selected_element, data_view->data + idx, selected_element_length);
        state->selected_element[selected_element_length] = '\0';
        idx += selected_element_length;
    }
    else
    {
        state->selected_element = (char*)malloc(1U);
        state->selected_element[0] = '\0';
    }

    uint16_t num_elements;

    memcpy(&num_elements, data_view->data + idx, sizeof(uint16_t));

    idx += sizeof(uint16_t);

    state->elements = createListOfStrings(num_elements);

    for (uint16_t i = 0U; i < num_elements; i++)
    {
        const uint8_t element_length = data_view->data[idx];
        idx++;

        state->elements.elements[i] = (char*)malloc(element_length + 1U);
        memcpy(state->elements.elements[i], data_view->data + idx, element_length);
        state->elements.elements[i][element_length] = '\0';

        idx += element_length;
    }
}

void updateCheckboxState(CheckboxInternalHandle* const handle, const UInt8Array* data_view)
{
    handle->is_checked = data_view->data[0];
}

void updateRadioButtonGroupState(RadioButtonGroupInternalHandle* const handle, const UInt8Array* data_view)
{
    size_t idx = 0U;

    memcpy(&handle->selected_button_idx, data_view->data + idx, sizeof(int32_t));
    idx += sizeof(int32_t);

    uint16_t num_buttons;

    memcpy(&num_buttons, data_view->data + idx, sizeof(uint16_t));
    idx += sizeof(uint16_t);

    handle->buttons = createListOfStrings(num_buttons);

    for (uint16_t k = 0U; k < num_buttons; k++)
    {
        const uint8_t button_length = data_view->data[idx];
        idx += sizeof(uint8_t);

        handle->buttons.elements[k] = (char*)malloc(button_length + 1U);
        memcpy(handle->buttons.elements[k], data_view->data + idx, button_length);
        handle->buttons.elements[k][button_length] = '\0';

        idx += button_length;
    }
}

void updateEditableTextState(EditableTextInternalHandle* const handle, const UInt8Array* data_view)
{
    const uint8_t* const data_ptr = data_view->data;
    handle->enter_pressed = data_ptr[0];

    const uint8_t text_length = data_ptr[1];

    handle->text = (char*)malloc(text_length + 1U);

    size_t idx = 2U;

    memcpy(handle->text, data_ptr + idx, text_length);
}

BaseHandle* internal_createButton(const char* const handle_string, const UInt8Array* const data_view)
{
    ButtonInternalHandle* const button = (ButtonInternalHandle*)malloc(sizeof(ButtonInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    button->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(button->handle_string, handle_string);
    button->handle_string[handle_string_length] = '\0';  // Null-terminate string

    button->type = GUI_ET_BUTTON;

    updateButtonState(button, data_view);

    BaseHandle* const handle = (BaseHandle*)button;

    return handle;
}

BaseHandle* internal_createSlider(const char* const handle_string, const UInt8Array* const data_view)
{
    SliderInternalHandle* const slider = (SliderInternalHandle*)malloc(sizeof(SliderInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    slider->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(slider->handle_string, handle_string);
    slider->handle_string[handle_string_length] = '\0';  // Null-terminate string

    slider->type = GUI_ET_SLIDER;

    updateSliderState(slider, data_view);

    BaseHandle* const handle = (BaseHandle*)slider;

    return handle;
}

BaseHandle* internal_createListBox(const char* const handle_string, const UInt8Array* const data_view)
{
    ListBoxInternalHandle* const list_box = (ListBoxInternalHandle*)malloc(sizeof(ListBoxInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    list_box->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(list_box->handle_string, handle_string);
    list_box->handle_string[handle_string_length] = '\0';  // Null-terminate string

    list_box->type = GUI_ET_LIST_BOX;

    BaseHandle* const handle = (BaseHandle*)list_box;

    list_box->state.elements = createListOfStrings(10U);
    list_box->state.selected_element = (char*)malloc(1U);
    list_box->state.selected_element[0] = '\0';

    updateListBoxState((ListBoxInternalHandle*)handle, data_view);

    return handle;
}

BaseHandle* internal_createDropdownMenu(const char* const handle_string, const UInt8Array* const data_view)
{
    DropdownMenuInternalHandle* const dropdown_menu =
        (DropdownMenuInternalHandle*)malloc(sizeof(DropdownMenuInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    dropdown_menu->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(dropdown_menu->handle_string, handle_string);
    dropdown_menu->handle_string[handle_string_length] = '\0';  // Null-terminate string

    dropdown_menu->type = GUI_ET_DROPDOWN_MENU;

    BaseHandle* const handle = (BaseHandle*)dropdown_menu;

    dropdown_menu->state.elements = createListOfStrings(10U);
    dropdown_menu->state.selected_element = (char*)malloc(1U);
    dropdown_menu->state.selected_element[0] = '\0';

    updateDropdownMenuState((DropdownMenuInternalHandle*)handle, data_view);

    return handle;
}

BaseHandle* internal_createCheckbox(const char* const handle_string, const UInt8Array* const data_view)
{
    CheckboxInternalHandle* const checkbox = (CheckboxInternalHandle*)malloc(sizeof(CheckboxInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    checkbox->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(checkbox->handle_string, handle_string);
    checkbox->handle_string[handle_string_length] = '\0';  // Null-terminate string

    checkbox->type = GUI_ET_CHECKBOX;

    BaseHandle* const handle = (BaseHandle*)checkbox;

    updateCheckboxState((CheckboxInternalHandle*)handle, data_view);

    return handle;
}

BaseHandle* internal_createRadioButtonGroup(const char* const handle_string, const UInt8Array* const data_view)
{
    RadioButtonGroupInternalHandle* const radiobutton_group =
        (RadioButtonGroupInternalHandle*)malloc(sizeof(RadioButtonGroupInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    radiobutton_group->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(radiobutton_group->handle_string, handle_string);
    radiobutton_group->handle_string[handle_string_length] = '\0';  // Null-terminate string

    radiobutton_group->type = GUI_ET_RADIO_BUTTON_GROUP;

    BaseHandle* const handle = (BaseHandle*)radiobutton_group;

    updateRadioButtonGroupState((RadioButtonGroupInternalHandle*)handle, data_view);

    return handle;
}

BaseHandle* internal_createEditableText(const char* const handle_string, const UInt8Array* const data_view)
{
    EditableTextInternalHandle* const editable_text =
        (EditableTextInternalHandle*)malloc(sizeof(EditableTextInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    editable_text->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(editable_text->handle_string, handle_string);
    editable_text->handle_string[handle_string_length] = '\0';  // Null-terminate string

    editable_text->type = GUI_ET_EDITABLE_TEXT;

    BaseHandle* const handle = (BaseHandle*)editable_text;

    updateEditableTextState((EditableTextInternalHandle*)handle, data_view);

    return handle;
}

BaseHandle* internal_createTextLabel(const char* const handle_string, const UInt8Array* const data_view)
{
    TextLabelInternalHandle* const text_label = (TextLabelInternalHandle*)malloc(sizeof(TextLabelInternalHandle));

    size_t handle_string_length = strlen(handle_string);

    text_label->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(text_label->handle_string, handle_string);
    text_label->handle_string[handle_string_length] = '\0';  // Null-terminate string

    text_label->type = GUI_ET_TEXT_LABEL;

    BaseHandle* const handle = (BaseHandle*)text_label;

    updateTextLabelState((TextLabelInternalHandle*)handle, data_view);

    return handle;
}

void populateGuiElementWithData(const GuiElementType type,
                                const char* const handle_string,
                                const UInt8Array* const data_view)
{
    GuiElementMap* gui_element_handles = getGuiElementHandles();

    if (isGuiElementHandleContainerKeyInMap(handle_string, gui_element_handles))
    {
        // Key is already in map, update state
        BaseHandle* const handle = (BaseHandle*)getGuiElementHandleContainer(handle_string, gui_element_handles);
        if (handle != NULL)
        {
            if (handle->type != type)
            {
                printf("Gui element with handle string %s already exists, but has different type!\n", handle_string);
                return;
            }

            if (type == GUI_ET_BUTTON)
            {
                updateButtonState((ButtonInternalHandle*)handle, data_view);
            }
            else if (type == GUI_ET_CHECKBOX)
            {
                updateCheckboxState((CheckboxInternalHandle*)handle, data_view);
            }
            else if (type == GUI_ET_SLIDER)
            {
                updateSliderState((SliderInternalHandle*)handle, data_view);
            }
            else if (type == GUI_ET_TEXT_LABEL)
            {
                updateTextLabelState((TextLabelInternalHandle*)handle, data_view);
            }
            else if (type == GUI_ET_LIST_BOX)
            {
                updateListBoxState((ListBoxInternalHandle*)handle, data_view);
            }
            else if (type == GUI_ET_EDITABLE_TEXT)
            {
                updateEditableTextState((EditableTextInternalHandle*)handle, data_view);
            }
            else if (type == GUI_ET_DROPDOWN_MENU)
            {
                updateDropdownMenuState((DropdownMenuInternalHandle*)handle, data_view);
            }
            else if (type == GUI_ET_RADIO_BUTTON_GROUP)
            {
                updateRadioButtonGroupState((RadioButtonGroupInternalHandle*)handle, data_view);
            }
        }
    }
    else
    {
        BaseHandle* handle = NULL;
        // Key is not in map, create new element
        if (type == GUI_ET_BUTTON)
        {
            handle = (BaseHandle*)internal_createButton(handle_string, data_view);
        }
        else if (type == GUI_ET_SLIDER)
        {
            handle = (BaseHandle*)internal_createSlider(handle_string, data_view);
        }
        else if (type == GUI_ET_LIST_BOX)
        {
            handle = (BaseHandle*)internal_createListBox(handle_string, data_view);
        }
        else if (type == GUI_ET_DROPDOWN_MENU)
        {
            handle = (BaseHandle*)internal_createDropdownMenu(handle_string, data_view);
        }
        else if (type == GUI_ET_CHECKBOX)
        {
            handle = (BaseHandle*)internal_createCheckbox(handle_string, data_view);
        }
        else if (type == GUI_ET_RADIO_BUTTON_GROUP)
        {
            handle = (BaseHandle*)internal_createRadioButtonGroup(handle_string, data_view);
        }
        else if (type == GUI_ET_EDITABLE_TEXT)
        {
            handle = (BaseHandle*)internal_createEditableText(handle_string, data_view);
        }
        else if (type == GUI_ET_TEXT_LABEL)
        {
            handle = (BaseHandle*)internal_createTextLabel(handle_string, data_view);
        }

        if (handle != NULL)
        {
            insertElementIntoGuiElementHandleContainerMap(gui_element_handles, handle_string, handle);
        }
    }
}

void updateGuiState(const ReceivedGuiData* received_gui_data)
{
    size_t idx = 0U;

    uint8_t* const raw_data = received_gui_data->data;

    // Receive[1]: Gui element type (uint8_t)
    const GuiElementType type = (GuiElementType)(raw_data[idx]);
    idx += sizeof(uint8_t);

    // Receive[2]: Handle string length (uint8_t)
    const uint8_t handle_string_length = (uint8_t)(raw_data[idx]);
    idx += sizeof(uint8_t);

    char* const handle_string = (char*)malloc(handle_string_length + 1U);

    // Receive[3]: Handle string (variable)
    for (uint8_t i = 0; i < handle_string_length; i++)
    {
        handle_string[i] = raw_data[idx];
        idx++;
    }
    handle_string[handle_string_length] = '\0';  // Null-terminate string

    uint32_t size_of_current_gui_element;

    // Receive[4]: Current gui element payload size (uint32_t)
    memcpy(&size_of_current_gui_element, raw_data + idx, sizeof(uint32_t));
    idx += sizeof(uint32_t);

    const UInt8Array gui_element_data = {raw_data + idx, size_of_current_gui_element};

    // Receive[5]: Gui element data (variable)
    populateGuiElementWithData(type, handle_string, &gui_element_data);

    free(handle_string);
}

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

ListBoxHandle getListBoxHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    ListBoxHandle list_box_handle;
    list_box_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return list_box_handle;
    }

    ListBoxInternalHandle* const gui_elem =
        (ListBoxInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_LIST_BOX)
    {
        printf("Gui element with handle string %s is not a slider!\n", handle_string);
        return list_box_handle;
    }

    list_box_handle.__handle = gui_elem;

    return list_box_handle;
}

DropdownMenuHandle getDropdownMenuHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    DropdownMenuHandle dropdown_menu_handle;
    dropdown_menu_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return dropdown_menu_handle;
    }

    DropdownMenuInternalHandle* const gui_elem =
        (DropdownMenuInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_DROPDOWN_MENU)
    {
        printf("Gui element with handle string %s is not a slider!\n", handle_string);
        return dropdown_menu_handle;
    }

    dropdown_menu_handle.__handle = gui_elem;

    return dropdown_menu_handle;
}

CheckboxHandle getCheckboxHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    CheckboxHandle checkbox_handle;
    checkbox_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return checkbox_handle;
    }

    CheckboxInternalHandle* const gui_elem =
        (CheckboxInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_CHECKBOX)
    {
        printf("Gui element with handle string %s is not a slider!\n", handle_string);
        return checkbox_handle;
    }

    checkbox_handle.__handle = gui_elem;

    return checkbox_handle;
}

RadioButtonGroupHandle getRadioButtonGroupHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    RadioButtonGroupHandle radiobutton_group_handle;
    radiobutton_group_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return radiobutton_group_handle;
    }

    RadioButtonGroupInternalHandle* const gui_elem =
        (RadioButtonGroupInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_RADIO_BUTTON_GROUP)
    {
        printf("Gui element with handle string %s is not a slider!\n", handle_string);
        return radiobutton_group_handle;
    }

    radiobutton_group_handle.__handle = gui_elem;

    return radiobutton_group_handle;
}

EditableTextHandle getEditableTextHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    EditableTextHandle editable_text_handle;
    editable_text_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return editable_text_handle;
    }

    EditableTextInternalHandle* const gui_elem =
        (EditableTextInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_EDITABLE_TEXT)
    {
        printf("Gui element with handle string %s is not a slider!\n", handle_string);
        return editable_text_handle;
    }

    editable_text_handle.__handle = gui_elem;

    return editable_text_handle;
}

TextLabelHandle getTextLabelHandle(const char* const handle_string)
{
    GuiElementMap* const gui_element_map = getGuiElementHandles();

    TextLabelHandle text_label_handle;
    text_label_handle.__handle = NULL;

    if (!isGuiElementHandleContainerKeyInMap(handle_string, gui_element_map))
    {
        printf("Gui element with handle string %s does not exist!\n", handle_string);
        return text_label_handle;
    }

    TextLabelInternalHandle* const gui_elem =
        (TextLabelInternalHandle*)getGuiElementHandleContainer(handle_string, gui_element_map);

    if (gui_elem->type != GUI_ET_TEXT_LABEL)
    {
        printf("Gui element with handle string %s is not a slider!\n", handle_string);
        return text_label_handle;
    }

    text_label_handle.__handle = gui_elem;

    return text_label_handle;
}

void callGuiCallbackFunction(const ReceivedGuiData* received_gui_data)
{
    size_t idx = 0U;

    uint8_t* const raw_data = received_gui_data->data;

    const GuiElementType type = (GuiElementType)(raw_data[idx]);
    idx += sizeof(uint8_t);

    const uint8_t handle_string_length = (uint8_t)(raw_data[idx]);
    idx += sizeof(uint8_t);

    char* const handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(handle_string, (char*)(raw_data + idx));
    handle_string[handle_string_length] = '\0';  // Null-terminate string
    idx += handle_string_length;

    uint32_t payload_size;
    memcpy(&payload_size, raw_data + idx, sizeof(uint32_t));
    idx += sizeof(uint32_t);

    UInt8Array payload_data_view;
    payload_data_view.data = raw_data + idx;
    payload_data_view.size = payload_size;

    if (type == GUI_ET_BUTTON)
    {
        ButtonCallbackFunctionMap* const button_callback_function_map = getButtonCallbackFunctionMap();

        if (isButtonCallbackFunctionKeyInMap(handle_string, button_callback_function_map))
        {
            ButtonCallbackFunction cb_fun = getButtonCallbackFunction(handle_string, button_callback_function_map);
            cb_fun(getButtonHandle(handle_string));
        }
    }
    else if (type == GUI_ET_SLIDER)
    {
        SliderCallbackFunctionMap* const slider_callback_function_map = getSliderCallbackFunctionMap();

        if (isSliderCallbackFunctionKeyInMap(handle_string, slider_callback_function_map))
        {
            SliderCallbackFunction cb_fun = getSliderCallbackFunction(handle_string, slider_callback_function_map);
            cb_fun(getSliderHandle(handle_string));
        }
    }
    else if (type == GUI_ET_LIST_BOX)
    {
        ListBoxCallbackFunctionMap* const list_box_callback_function_map = getListBoxCallbackFunctionMap();

        if (isListBoxCallbackFunctionKeyInMap(handle_string, list_box_callback_function_map))
        {
            ListBoxCallbackFunction cb_fun = getListBoxCallbackFunction(handle_string, list_box_callback_function_map);
            cb_fun(getListBoxHandle(handle_string));
        }
    }
    else if (type == GUI_ET_DROPDOWN_MENU)
    {
        DropdownMenuCallbackFunctionMap* const dropdown_menu_callback_function_map =
            getDropdownMenuCallbackFunctionMap();

        if (isDropdownMenuCallbackFunctionKeyInMap(handle_string, dropdown_menu_callback_function_map))
        {
            DropdownMenuCallbackFunction cb_fun =
                getDropdownMenuCallbackFunction(handle_string, dropdown_menu_callback_function_map);
            cb_fun(getDropdownMenuHandle(handle_string));
        }
    }
    else if (type == GUI_ET_CHECKBOX)
    {
        CheckboxCallbackFunctionMap* const checkbox_callback_function_map = getCheckboxCallbackFunctionMap();

        if (isCheckboxCallbackFunctionKeyInMap(handle_string, checkbox_callback_function_map))
        {
            CheckboxCallbackFunction cb_fun =
                getCheckboxCallbackFunction(handle_string, checkbox_callback_function_map);
            cb_fun(getCheckboxHandle(handle_string));
        }
    }
    else if (type == GUI_ET_RADIO_BUTTON_GROUP)
    {
        RadioButtonGroupCallbackFunctionMap* const radiobutton_group_callback_function_map =
            getRadioButtonGroupCallbackFunctionMap();

        if (isRadioButtonGroupCallbackFunctionKeyInMap(handle_string, radiobutton_group_callback_function_map))
        {
            RadioButtonGroupCallbackFunction cb_fun =
                getRadioButtonGroupCallbackFunction(handle_string, radiobutton_group_callback_function_map);
            cb_fun(getRadioButtonGroupHandle(handle_string));
        }
    }
    else if (type == GUI_ET_EDITABLE_TEXT)
    {
        EditableTextCallbackFunctionMap* const editable_text_callback_function_map =
            getEditableTextCallbackFunctionMap();

        if (isEditableTextCallbackFunctionKeyInMap(handle_string, editable_text_callback_function_map))
        {
            EditableTextCallbackFunction cb_fun =
                getEditableTextCallbackFunction(handle_string, editable_text_callback_function_map);
            cb_fun(getEditableTextHandle(handle_string));
        }
    }
}

void internal_waitForSyncForAllGuiElements()
{
    printf("Waiting for DVS application to send GUI state...\n");
    const ReceivedGuiData received_data = internal_receiveGuiData();

    printf("GUI state received!\n");

    size_t idx = 1U;

    uint8_t* const raw_data = received_data.data;

    // Receive[0]: Number of gui objects (std::uint8_t)
    const size_t num_gui_objects = (size_t)(raw_data[0]);

    for (size_t k = 0; k < num_gui_objects; k++)
    {
        // Receive[1]: Gui element type (uint8_t)
        const GuiElementType type = (GuiElementType)(raw_data[idx]);
        idx += sizeof(uint8_t);

        // Receive[2]: Handle string length (uint8_t)
        const uint8_t handle_string_length = (uint8_t)(raw_data[idx]);
        idx += sizeof(uint8_t);

        char* const handle_string = (char*)malloc(handle_string_length + 1U);

        // Receive[3]: Handle string (variable)
        for (uint8_t i = 0; i < handle_string_length; i++)
        {
            handle_string[i] = raw_data[idx];
            idx++;
        }
        handle_string[handle_string_length] = '\0';  // Null-terminate string

        uint32_t size_of_current_gui_element;

        // Receive[4]: Current gui element payload size (uint32_t)
        memcpy(&size_of_current_gui_element, raw_data + idx, sizeof(uint32_t));
        idx += sizeof(uint32_t);

        const UInt8Array gui_element_data = {raw_data + idx, size_of_current_gui_element};

        // Receive[5]: Gui element data (variable)
        populateGuiElementWithData(type, handle_string, &gui_element_data);

        free(handle_string);

        idx += size_of_current_gui_element;
    }

    free(received_data.data);
}

void registerButtonCallback(const char* const handle_string, void (*button_callback_function)(const ButtonHandle))
{
    ButtonCallbackFunctionMap* const button_callback_function_map = getButtonCallbackFunctionMap();

    if (isButtonCallbackFunctionKeyInMap(handle_string, button_callback_function_map))
    {
        printf("Button callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoButtonCallbackFunctionMap(button_callback_function_map, handle_string, button_callback_function);
}

void registerSliderCallback(const char* const handle_string, void (*slider_callback_function)(const SliderHandle))
{
    SliderCallbackFunctionMap* const slider_callback_function_map = getSliderCallbackFunctionMap();

    if (isSliderCallbackFunctionKeyInMap(handle_string, slider_callback_function_map))
    {
        printf("Slider callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoSliderCallbackFunctionMap(slider_callback_function_map, handle_string, slider_callback_function);
}

void registerListBoxCallback(const char* const handle_string, void (*list_box_callback_function)(const ListBoxHandle))
{
    ListBoxCallbackFunctionMap* const list_box_callback_function_map = getListBoxCallbackFunctionMap();

    if (isListBoxCallbackFunctionKeyInMap(handle_string, list_box_callback_function_map))
    {
        printf("List box callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoListBoxCallbackFunctionMap(
        list_box_callback_function_map, handle_string, list_box_callback_function);
}

void registerDropdownMenuCallback(const char* const handle_string,
                                  void (*dropdown_menu_callback_function)(const DropdownMenuHandle))
{
    DropdownMenuCallbackFunctionMap* const dropdown_menu_callback_function_map = getDropdownMenuCallbackFunctionMap();

    if (isDropdownMenuCallbackFunctionKeyInMap(handle_string, dropdown_menu_callback_function_map))
    {
        printf("Drop down menu callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoDropdownMenuCallbackFunctionMap(
        dropdown_menu_callback_function_map, handle_string, dropdown_menu_callback_function);
}

void registerCheckboxCallback(const char* const handle_string, void (*checkbox_callback_function)(const CheckboxHandle))
{
    CheckboxCallbackFunctionMap* const checkbox_callback_function_map = getCheckboxCallbackFunctionMap();

    if (isCheckboxCallbackFunctionKeyInMap(handle_string, checkbox_callback_function_map))
    {
        printf("Checkbox callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoCheckboxCallbackFunctionMap(
        checkbox_callback_function_map, handle_string, checkbox_callback_function);
}

void registerRadioButtonGroupCallback(const char* const handle_string,
                                      void (*radiobutton_group_callback_function)(const RadioButtonGroupHandle))
{
    RadioButtonGroupCallbackFunctionMap* const radiobutton_group_callback_function_map =
        getRadioButtonGroupCallbackFunctionMap();

    if (isRadioButtonGroupCallbackFunctionKeyInMap(handle_string, radiobutton_group_callback_function_map))
    {
        printf("Radio button group callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoRadioButtonGroupCallbackFunctionMap(
        radiobutton_group_callback_function_map, handle_string, radiobutton_group_callback_function);
}

void registerEditableTextCallback(const char* const handle_string,
                                  void (*editable_text_callback_function)(const EditableTextHandle))
{
    EditableTextCallbackFunctionMap* const editable_text_callback_function_map = getEditableTextCallbackFunctionMap();

    if (isEditableTextCallbackFunctionKeyInMap(handle_string, editable_text_callback_function_map))
    {
        printf("Editable text callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoEditableTextCallbackFunctionMap(
        editable_text_callback_function_map, handle_string, editable_text_callback_function);
}

#endif  // DVS_GUI_INTERNAL_H