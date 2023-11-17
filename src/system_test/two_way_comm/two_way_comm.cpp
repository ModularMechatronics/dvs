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

    dvs::SliderHandle slider = dvs::getGuiElementHandle<dvs::SliderHandle>("slider0");
    dvs::ButtonHandle button0 = dvs::getGuiElementHandle<dvs::ButtonHandle>("button0");
    dvs::ButtonHandle button1 = dvs::getGuiElementHandle<dvs::ButtonHandle>("button1");
    dvs::CheckboxHandle checkbox = dvs::getGuiElementHandle<dvs::CheckboxHandle>("checkbox0");

    std::cout << "slider value: " << slider.getValue() << std::endl;
    std::cout << "checkbox is checked: " << checkbox.getIsChecked() << std::endl;

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

*/

void firstTest()
{
    std::cout << "Starting GUI thread!" << std::endl;

    // Synchronous update
    // dvs::registerGuiCallback("button0", Button::InteractionType::PRESSED, lambda...)
    // dvs::registerGuiCallback("button0", Button::InteractionType::RELEASED, lambda...)

    // dvs::registerGuiCallback("slider0", Button::InteractionType::PRESSED, lambda...)
    // dvs::registerGuiCallback("slider0", Button::InteractionType::RELEASED, lambda...)
    // dvs::registerGuiCallback("slider0", Button::InteractionType::DRAGGED, lambda...)

    dvs::registerGuiCallback_new("slider0", [](const dvs::SliderHandle& gui_element_handle) -> void {
        std::cout << "callback function: \"slider0\" value: " << gui_element_handle.getValue() << std::endl;
    });

    dvs::registerGuiCallback_new("button0", [](const dvs::ButtonHandle& gui_element_handle) -> void {
        std::cout << "callback function: \"button0\" pressed!" << std::endl;
    });

    dvs::registerGuiCallback_new("checkbox0", [](const dvs::CheckboxHandle& gui_element_handle) -> void {
        std::cout << "callback function: \"checkbox0\" pressed!" << std::endl;
    });

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
