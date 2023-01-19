#include <algorithm>
#include <functional>
#include <map>
#include <vector>

#include "dvs/dvs.h"
#include "tests.h"

using StringFunctionMap = std::map<std::string, std::function<void()>>;
StringFunctionMap fcns;

void displayHelp()
{
    std::cout << "Usage: ./basic-tests-app [-h, --help, help] [";
    std::map<std::string, std::function<void()>>::iterator it;

    for (it = fcns.begin(); it != fcns.end(); it++)
    {
        std::cout << it->first << ", ";
    }

    std::cout << "all]" << std::endl;
}

int main(int argc, char* argv[])
{
    fcns = StringFunctionMap{{"scatter", testScatter},
                             {"scatter3", testScatter3},
                             {"plot", testPlot},
                             {"plot3", testPlot3},
                             {"fastPlot", testFastPlot},
                             {"fastPlot3", testFastPlot3},
                             {"lineCollection", testLineCollection},
                             {"lineCollection3", testLineCollection3},
                             {"stem", testStem},
                             {"plotCollection", testPlotCollection},
                             {"plotCollection3", testPlotCollection3},
                             {"surf", testSurf},
                             {"imShow", testImShow},
                             {"axis", testAxis},
                             {"drawMesh", testDrawMesh},
                             {"stairs", testStairs},
                             {"legend", testLegend},
                             {"openProjectFile", testOpenProjectFile}};

    if (argc == 1)
    {
        std::cout << "No arguments provided!" << std::endl;
        displayHelp();
    }
    else
    {
        const std::string arg(argv[1]);

        if ((arg == "-h") || (arg == "--help") || (arg == "help"))
        {
            displayHelp();
        }
        else if (arg == "all")
        {
            std::map<std::string, std::function<void()>>::iterator it;

            for (it = fcns.begin(); it != fcns.end(); it++)
            {
                std::cout << "Running " << it->first << "..." << std::endl;
                it->second();
                std::cout << it->first << " ran successfully. Press any key to continue." << std::endl << std::endl;
                std::cin.ignore();
            }
        }
        else if (fcns.count(arg) > 0)
        {
            fcns[arg]();
        }
        else
        {
            std::cout << "ERROR: No matching arg for: " << arg << std::endl;
        }
    }
}
