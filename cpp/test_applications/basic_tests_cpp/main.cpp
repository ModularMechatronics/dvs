#include <algorithm>
#include <functional>
#include <map>
#include <vector>

#include "dvs/dvs.h"
#include "tests.h"

std::map<std::string, std::function<void()>> fcns;

void displayHelp()
{
    std::cout << "Usage: ./test-app [-h, --help, help] [";
    std::map<std::string, std::function<void()>>::iterator it;

    for (it = fcns.begin(); it != fcns.end(); it++)
    {
        std::cout << it->first << ", ";
    }

    std::cout << "all]" << std::endl;
}

int main(int argc, char* argv[])
{
    fcns["scatter"] = testScatter;
    fcns["scatter3"] = testScatter3;
    fcns["plot"] = testPlot;
    fcns["plot3"] = testPlot3;
    fcns["surf"] = testSurf;
    fcns["imShow"] = testImShow;
    fcns["axis"] = testAxis;
    fcns["drawTriangles"] = testDrawTriangles;
    fcns["drawTriangle"] = testDrawTriangle;
    fcns["drawTiles"] = testDrawTiles;
    fcns["drawMesh"] = testDrawMesh;
    fcns["drawLine3D"] = testDrawLine3D;
    fcns["drawLine2D"] = testDrawLine2D;
    fcns["drawLineBetweenPoints"] = testDrawLineBetweenPoints;
    fcns["stairs"] = testStairs;

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
            std::map<std::string, std::function<void()>>::iterator it;

            for (it = fcns.begin(); it != fcns.end(); it++)
            {
                if (arg == it->first)
                {
                    std::cout << "Running '" << it->first << "'" << std::endl << std::endl;
                    it->second();
                    break;
                }
            }
        }
        else
        {
            std::cout << "ERROR: No matching arg for: " << arg << std::endl;
        }
    }
}
