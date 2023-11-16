#include "system_test/two_way_comm/two_way_comm.h"

#include <iostream>

#include "dvs/dvs.h"
#include "utils.h"

void printValues()
{
    // Asynchronous update
    // const float slider_value = dvs::getValue<float>("slider0")
    // const float radio_value = dvs::getValue<float>("slider0")
    // const float f = dvs::getValue("slider0");
}

/*
template <typename T> T getGuiElementHandle(const std::string& handle_string)
{
    // Returns handle of gui element, with correct type
    // Assertion on type correctness
}

template <typename T> T getGuiElementState(const std::string& handle_string)
{
    // Returns state of gui element, with correct type
    // Assertion on type correctness
}
// Can callback functions be type specific?
// E.g.
dvs::registerGuiCallback("button0", [](const dvs::ButtonElementHandle& gui_element_handle) -> void {
        // std::cout << "callback function: \"button0\" pressed!" << std::endl;
    });

template <typename T>
void registerGuiCallback_new(const std::string& handle_string, std::function<void(const GuiElementHandle&)> callback_function);

template <>
void registerGuiCallback_new(const std::string& handle_string, std::function<void(const SliderHandle&)> callback_function) {}
template <>
void registerGuiCallback_new(const std::string& handle_string, std::function<void(const ButtonHandle&)> callback_function) {}
template <>
void registerGuiCallback_new(const std::string& handle_string, std::function<void(const CheckboxHandle&)> callback_function) {}

*/



void firstTest()
{
    std::cout << "Starting GUI thread!" << std::endl;

    // Synchronous update
    // dvs::registerGuiCallback("button0", Button::Interaction::PRESSED, lambda...)
    // dvs::registerGuiCallback("button0", Button::Interaction::RELEASED, lambda...)

    // dvs::registerGuiCallback("slider0", Button::Interaction::PRESSED, lambda...)
    // dvs::registerGuiCallback("slider0", Button::Interaction::RELEASED, lambda...)
    // dvs::registerGuiCallback("slider0", Button::Interaction::DRAGGED, lambda...)

    // dvs::registerGuiCallback("button0", [](const dvs::GuiElementHandle& gui_element_handle) -> void {
    //     // std::cout << "callback function: \"button0\" pressed!" << std::endl;
    // });

    // dvs::registerGuiCallback("button1", [](const dvs::GuiElementHandle& gui_element_handle) -> void {
    //     std::cout << "callback function: \"button1\" pressed!" << std::endl;
    // });

    dvs::startGuiReceiveThread();

    std::cout << "Instructions:" << std::endl
              << "\"q\": Exits application" << std::endl
              << "\"v\" print values" << std::endl;

    // Client application loop
    while (true)
    {
        std::string input;
        std::getline(std::cin, input);

        if (input == "q")
        {
            break;
        }
        else if (input == "v")
        {
            printValues();
        }
    }
}

namespace two_way_comm
{

void addTests()
{
    addTest("cpp", "two_way_comm", "first_test", firstTest);
}

}  // namespace two_way_comm
