#include "system_test/two_way_comm/two_way_comm.h"

#include <iostream>

#include "dvs/dvs.h"
#include "utils.h"

void printValues()
{
    // const float slider_value = dvs::getValue<float>("slider0")
    // const float radio_value = dvs::getValue<float>("slider0")
    const float f = dvs::getValue("slider0");
}

void firstTest()
{
    std::cout << "Starting GUI thread!" << std::endl;

    dvs::registerGuiCallback("button0", [](const std::string& s) -> void {
        std::cout << "callback function: \"button0\" pressed!" << std::endl;
    });
    dvs::startGuiReceiveThread();

    std::cout << "Instructions:" << std::endl
              << "\"q\": Exits application" << std::endl
              << "\"v\" print values" << std::endl;

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
