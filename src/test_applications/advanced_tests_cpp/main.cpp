#include <algorithm>
#include <functional>
#include <map>
#include <vector>

#include "tests.h"

using StringFunctionMap = std::map<std::string, std::function<void()>>;

std::map<std::string, StringFunctionMap> fcns;

void displayHelp()
{
    std::cout << "Usage: ./test-app [-h, --help, help] [\033[32m<category>\033[0m \033[33m<test name>\033[0m]"
              << std::endl;
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
    // ######################### plot #########################
    StringFunctionMap plot2_functions{
        {"view", plot2::testWithVectorView},
    };

    // ######################### plot3 #########################
    StringFunctionMap plot3_functions{{"view", plot3_ns::testWithVectorView}};

    // ######################### scatter #######################
    StringFunctionMap scatter2_functions{{"view", scatter2::testWithVectorView},
                                         {"color_vec", scatter2::testWithColorVec}};

    // ######################### scatter3 ######################
    StringFunctionMap scatter3_functions{{"view", scatter3_ns::testWithVectorView},
                                         {"color_vec", scatter3_ns::testWithColorVec}};

    // ######################### surf ##########################
    StringFunctionMap surf_functions{{"view", surf_ns::testWithMatrixView}};

    // ######################### dynamic_plotting ##########################
    StringFunctionMap dynamic_plotting_functions{
        {"2d_expanding_plot", dynamic_plotting::test2DFunctionExpandingPlotObject},
        {"3d_expanding_plot", dynamic_plotting::test3DFunctionExpandingPlotObject},
        {"2d_changing_rotation", dynamic_plotting::test2DFunctionRotatingView},
        {"2d_changing_axes", dynamic_plotting::test2DFunctionChangingAxes},
        {"2d_new_data_clear", dynamic_plotting::test2DFunctionNewDataAndClear},
        {"3d_new_data_clear", dynamic_plotting::test3DFunctionNewDataAndClear},
        {"3d_manual_interaction", dynamic_plotting::test3DFunctionManualInteraction},
    };

    // ######################### updateable_plotting ##########################
    StringFunctionMap updateable_plotting_tests{{"basic_real_time", updateable_plotting::testRealTimePlotBasic},
                                                {"basic_plot2", updateable_plotting::testPlot2Basic},
                                                {"basic_surf", updateable_plotting::testSurfBasic},
                                                {"sync_real_time", updateable_plotting::testSynchronizedRealTimePlot}};

    // ######################### object_transform ##########################
    StringFunctionMap object_transform_tests{
        {"basic", object_transform::testBasic},
    };

    // ######################### object_transform ##########################
    StringFunctionMap append_properties_tests{
        {"basic", append_properties::testBasic},
    };

    fcns["plot"] = plot2_functions;
    fcns["plot3"] = plot3_functions;
    fcns["scatter"] = scatter2_functions;
    fcns["scatter3"] = scatter3_functions;
    fcns["surf"] = surf_functions;
    fcns["dynamic_plotting"] = dynamic_plotting_functions;
    fcns["updateable_plotting"] = updateable_plotting_tests;
    fcns["object_transform"] = object_transform_tests;
    fcns["append_properties"] = append_properties_tests;

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
            if (argc < 3)
            {
                std::cout << "ERROR: Missing \033[33mtest name\033[0m after provided argument \"" << first_arg << "\""
                          << std::endl;
                displayHelp();
            }
            else
            {
                StringFunctionMap& fcn_map = fcns[first_arg];
                const std::string second_arg(argv[2]);
                if (fcn_map.count(second_arg) > 0)
                {
                    std::cout << "Running test '\033[32m" << first_arg << "\033[0m::\033[33m" << second_arg
                              << "\033[0m'" << std::endl
                              << std::endl;
                    fcn_map[second_arg]();
                }
                else
                {
                    std::cout << "ERROR: Provided \033[33mtest name\033[0m \"" << second_arg
                              << "\" is not present in \033[32mcategory\033[0m \"" << first_arg << "\"" << std::endl;
                    displayHelp();
                }
            }
        }
        else
        {
            std::cout << "ERROR: No matching \033[32mcategory\033[0m for provided argument \"" << first_arg << "\""
                      << std::endl;
            displayHelp();
        }
    }
}
