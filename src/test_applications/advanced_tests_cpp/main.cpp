#include <algorithm>
#include <functional>
#include <map>
#include <vector>

#include "dvs/dvs.h"
#include "tests/tests.h"

using StringFunctionMap = std::map<std::string, std::function<void()>>;

std::map<std::string, StringFunctionMap> fcns;

void displayHelp()
{
    std::cout << "Usage: ./test-app [-h, --help, help] [\033[32m<category>\033[0m \033[33m<test name>\033[0m]" << std::endl;
    std::map<std::string, StringFunctionMap>::iterator outer_it;

    for (outer_it = fcns.begin(); outer_it != fcns.end(); outer_it++)
    {
        std::cout << " - \033[32m" << outer_it->first << "\033[0m" << std::endl;
        StringFunctionMap::iterator it;
        for (it = outer_it->second.begin(); it != outer_it->second.end(); it++)
        {
            std::cout << "    * \033[33m" << it->first << "\033[0m" << std::endl;
        }
        
    }
}

int main(int argc, char* argv[])
{
    std::map<std::string, std::function<void()>> plot2_functions;
    plot2_functions["basic0"] = plot2::testBasic;
    plot2_functions["basic1"] = plot2::testBasic;
    plot2_functions["basic2"] = plot2::testBasic;
    fcns["plot"] = plot2_functions;
    fcns["scatter"] = plot2_functions;
    fcns["surf"] = plot2_functions;

    if (argc == 1)
    {
        std::cout << "No arguments provided!" << std::endl;
        displayHelp();
    }
    else
    {
        const std::string first_arg(argv[1]);

        if ((first_arg == "-h") || (first_arg == "--help") || (first_arg == "help"))
        {
            displayHelp();
        }
        else if (fcns.count(first_arg) > 0)
        {
            if(argc < 3)
            {
                std::cout << "ERROR: Missing \033[33mtest name\033[0m after provided argument \"" << first_arg <<
                "\"" << std::endl;
                displayHelp();
            }
            else
            {
                StringFunctionMap& fcn_map = fcns[first_arg];
                const std::string second_arg(argv[2]);
                if(fcn_map.count(second_arg) > 0)
                {
                    std::cout << "Running test '\033[32m" << first_arg << "\033[0m::\033[33m" << second_arg << "\033[0m'" << std::endl << std::endl;
                    fcn_map[second_arg]();
                }
                else
                {
                    std::cout << "ERROR: Provided \033[33mtest name\033[0m \"" << second_arg << "\" is not present in \033[32mcategory\033[0m \""
                    << first_arg << "\"" << std::endl;
                    displayHelp();
                }
            }
        }
        else
        {
            std::cout << "ERROR: No matching \033[32mcategory\033[0m for provided argument \"" << first_arg << "\"" << std::endl;
            displayHelp();
        }
    }
}
