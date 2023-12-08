#include "system_test/two_way_comm/two_way_comm.h"

#include <iostream>

#include "dvs/dvs.h"
#include "utils.h"

void printValues()
{
    const dvs::gui::SliderHandle slider = dvs::gui::getGuiElementHandle<dvs::gui::SliderHandle>("slider0");
    const dvs::gui::ButtonHandle button0 = dvs::gui::getGuiElementHandle<dvs::gui::ButtonHandle>("button0");
    const dvs::gui::ButtonHandle button1 = dvs::gui::getGuiElementHandle<dvs::gui::ButtonHandle>("button1");
    const dvs::gui::CheckboxHandle checkbox = dvs::gui::getGuiElementHandle<dvs::gui::CheckboxHandle>("checkbox0");

    std::cout << "slider value: " << slider.getValue() << std::endl;
    std::cout << "checkbox is checked: " << checkbox.getIsChecked() << std::endl;
}

void changeGuiFromClientApp()
{
    const dvs::gui::ButtonHandle button0 = dvs::gui::getGuiElementHandle<dvs::gui::ButtonHandle>("button0");
    button0.setLabel("New label");
}

void firstTest()
{
    std::cout << "Registering callbacks!" << std::endl;
    dvs::gui::registerGuiCallback("slider0", [](const dvs::gui::SliderHandle& gui_element_handle) -> void {
        std::cout << "Callback function: \"slider0\" value: " << gui_element_handle.getValue() << std::endl;
    });

    dvs::gui::registerGuiCallback("button0", [](const dvs::gui::ButtonHandle& gui_element_handle) -> void {
        std::cout << "Callback function: \"button0\" pressed: " << gui_element_handle.getIsPressed() << std::endl;
    });

    dvs::gui::registerGuiCallback("button1", [](const dvs::gui::ButtonHandle& gui_element_handle) -> void {
        std::cout << "Callback function: \"button1\" pressed: " << gui_element_handle.getIsPressed() << std::endl;
    });

    dvs::gui::registerGuiCallback("checkbox0", [](const dvs::gui::CheckboxHandle& gui_element_handle) -> void {
        std::cout << "Callback function: \"checkbox0\" pressed: " << gui_element_handle.getIsChecked() << std::endl;
    });

    std::cout << "Starting GUI thread!" << std::endl;
    dvs::gui::startGuiReceiveThread();

    std::cout << "Instructions:" << std::endl
              << "\"q\": Exits application" << std::endl
              << "\"v\" print values" << std::endl
              << "\"c\" change gui" << std::endl;

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
        else if (input == "c")
        {
            changeGuiFromClientApp();
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
