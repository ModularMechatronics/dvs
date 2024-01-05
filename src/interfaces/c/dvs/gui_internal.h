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

void initDataStructures(const size_t initial_size)
{
    GuiElementMap* const gui_element_handles = getGuiElementHandles();
    initGuiElementHandleContainerMap(gui_element_handles, initial_size);

    ButtonCallbackFunctionMap* const button_callback_function_map = getButtonCallbackFunctionMap();
    initButtonCallbackFunctionMap(button_callback_function_map, initial_size);
}

GuiElementHandleContainer* internal_createButton(const char* const handle_string, const UInt8Array* const data_view)
{
    ButtonHandle* const button = (ButtonHandle*)malloc(sizeof(ButtonHandle));

    size_t handle_string_length = strlen(handle_string);

    button->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(button->handle_string, handle_string);
    button->handle_string[handle_string_length] = '\0';  // Null-terminate string

    button->is_pressed = (bool)(data_view->data[0]);

    GuiElementHandleContainer* const container = (GuiElementHandleContainer*)malloc(sizeof(GuiElementHandleContainer));

    container->type = GUI_ET_BUTTON;
    container->data = (uint8_t*)button;

    return container;
}

GuiElementHandleContainer* internal_createSlider(const char* const handle_string, const UInt8Array* const data_view)
{
    SliderHandle* const slider = (SliderHandle*)malloc(sizeof(SliderHandle));

    size_t handle_string_length = strlen(handle_string);

    slider->handle_string = (char*)malloc(handle_string_length + 1U);
    strcpy(slider->handle_string, handle_string);
    slider->handle_string[handle_string_length] = '\0';  // Null-terminate string

    memcpy(&slider->min_value, data_view->data, sizeof(int32_t));
    memcpy(&slider->max_value, data_view->data + sizeof(int32_t), sizeof(int32_t));
    memcpy(&slider->step_size, data_view->data + 2U * sizeof(int32_t), sizeof(int32_t));
    memcpy(&slider->value, data_view->data + 3U * sizeof(int32_t), sizeof(int32_t));

    GuiElementHandleContainer* const container = (GuiElementHandleContainer*)malloc(sizeof(GuiElementHandleContainer));

    container->type = GUI_ET_SLIDER;
    container->data = (uint8_t*)slider;

    return container;
}

void updateContainerState(GuiElementHandleContainer* const container, const UInt8Array* data_view) {}

void populateGuiElementWithData(const GuiElementType type,
                                const char* const handle_string,
                                const UInt8Array* const data_view)
{
    GuiElementMap* gui_element_handles = getGuiElementHandles();

    if (isGuiElementHandleContainerKeyInMap(handle_string, gui_element_handles))
    {
        // Key is already in map, update state
        GuiElementHandleContainer* const container = getGuiElementHandleContainer(handle_string, gui_element_handles);
        if (container != NULL)
        {
            if (container->type != type)
            {
                printf("Gui element with handle string %s already exists, but has different type!\n", handle_string);
                return;
            }
            updateContainerState(container, data_view);
        }
    }
    else
    {
        GuiElementHandleContainer* container = NULL;
        // Key is not in map, create new element
        if (type == GUI_ET_BUTTON)
        {
            container = internal_createButton(handle_string, data_view);
        }
        else if (type == GUI_ET_CHECKBOX) {}
        else if (type == GUI_ET_SLIDER)
        {
            container = internal_createSlider(handle_string, data_view);
        }
        else if (type == GUI_ET_TEXT_LABEL) {}
        else if (type == GUI_ET_LIST_BOX) {}
        else if (type == GUI_ET_EDITABLE_TEXT) {}
        else if (type == GUI_ET_DROPDOWN_MENU) {}
        else if (type == GUI_ET_RADIO_BUTTON_GROUP) {}

        if (container != NULL)
        {
            insertElementIntoGuiElementHandleContainerMap(gui_element_handles, handle_string, container);
        }
    }
}

void internal_waitForSyncForAllGuiElements()
{
    printf("Waiting for DVS application to send GUI state...\n");
    const ReceivedGuiData received_data = internal_receiveGuiData();

    printf("GUI state received!");

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

void registerButtonCallback(const char* const handle_string, ButtonCallbackFunction button_callback_function)
{
    ButtonCallbackFunctionMap* const button_callback_function_map = getButtonCallbackFunctionMap();

    if (isButtonCallbackFunctionKeyInMap(handle_string, button_callback_function_map))
    {
        printf("Button callback with name %s already exists! Overwriting old callback...\n", handle_string);
    }

    insertElementIntoButtonCallbackFunctionMap(button_callback_function_map, handle_string, button_callback_function);
}

void registerSliderCallback(const char* const handle_string,
                            void (*slider_callback_function)(const SliderHandle* const))
{
}

#endif  // DVS_GUI_INTERNAL_H