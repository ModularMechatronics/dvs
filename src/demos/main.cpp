#include <algorithm>
#include <functional>
#include <map>
#include <vector>

#include "debug_value_args.h"
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
    debug_value_args::parseArgs(argc, argv);

    // ######################### klein #########################
    StringFunctionMap klein_functions{
        {"basic", klein::testBasic}, {"adv0", klein::testAdvanced0}, {"adv1", klein::testAdvanced1}};

    StringFunctionMap car_functions{{"basic", car::testBasic}};

    StringFunctionMap wave_equation_functions{{"basic", wave_equation::testBasic}};

    StringFunctionMap spring_structure_functions{{"basic", spring_structure::testBasic}};

    StringFunctionMap sphere_functions{{"basic", sphere::testBasic}};

    StringFunctionMap particles_functions{{"basic", particles::testBasic}};

    StringFunctionMap fake_fem_functions{{"basic", fake_fem::testBasic}};

    StringFunctionMap imu_functions{{"basic", imu::testBasic}};

    StringFunctionMap ad_dataset_functions{{"basic", ad_dataset::testBasic}};

    StringFunctionMap joints_functions{{"basic", joints::testBasic}};

    StringFunctionMap small_functions{{"lorenz", small::testLorenz},
                                      {"bump", small::testBump},
                                      {"scatter_color_map", small::testScatterColorMap},
                                      {"scatter_samples", small::testScatterSamples},
                                      {"fake_contour", small::testFakeContour},
                                      {"stocks", small::testStocks},
                                      {"scatter_cluster", small::testScatterCluster},
                                      {"scatter_varying_size", small::testScatterVaryingSize},
                                      {"scatter_small_points", small::testScatterSmallPoints},
                                      {"transparent_fill_below_plot", small::testTransparentFillBelowPlot},
                                      {"test_transitioning_surfs", small::testTransitioningSurfs},
                                      {"iso_surfaces", small::testIsoSurfaces},
                                      {"sling_plot", sling_plot::testSlingPlot},
                                      {"hyperboloid", small::testHyperboloid},
                                      {"sphere", small::testSphere},
                                      {"lissajous", small::testLissaJous},
                                      {"lines_and_dots", small::testLinesAndDots},
                                      {"cube_image", small::testCubeImage},
                                      {"filtering_with_slider", small::testFilteringWithSliders},
                                      {"pid_tuner", small::testPidTuner},
                                      {"three_body_problem", small::testThreeBodyProblem},
                                      {"swirls", small::testSwirls},
                                      {"bouncing_balls", small::testBouncingBalls},
                                      {"series", small::testSeries},
                                      {"chaos", small::testChaos}};

    fcns["klein"] = klein_functions;
    fcns["car"] = car_functions;
    fcns["wave_equation"] = wave_equation_functions;
    fcns["spring_structure"] = spring_structure_functions;
    fcns["sphere"] = sphere_functions;
    fcns["particles"] = particles_functions;
    fcns["fake_fem"] = fake_fem_functions;
    fcns["imu"] = imu_functions;
    fcns["ad_dataset"] = ad_dataset_functions;
    fcns["joints"] = joints_functions;
    fcns["boxes"] = StringFunctionMap{{"basic", boxes::testBasic}};
    fcns["hexagon"] = StringFunctionMap{{"basic", hexagon_rods::testBasic}};
    fcns["particle_field"] = StringFunctionMap{{"basic", particle_field::testBasic}};
    fcns["slam"] = StringFunctionMap{{"basic", slam::testBasic}};
    fcns["gui"] = StringFunctionMap{{"basic", gui_test::testBasic}, {"dynamic_system", gui_test::testDynamicSystem}};
    fcns["marbles"] = StringFunctionMap{{"basic", marbles::testBasic}};
    fcns["small"] = small_functions;

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
