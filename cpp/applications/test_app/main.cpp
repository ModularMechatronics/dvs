#include <algorithm>

#include "dvs.h"

extern void testBasic();
extern void testNewElements();
extern void testScatter();
extern void testScatter3();
extern void testSurf();
extern void testImShow();

void displayHelp()
{
    std::cout << "Usage: ./test-app [-h, --help, help] [basic, new-elements, scatter, scatter3, surf, imshow]" << std::endl;
}

int main(int argc, char* argv[])
{
    if(argc == 0)
    {
        std::cout << "Running basic..." << std::endl;
        testBasic();
    }
    else
    {
        const std::string arg(argv[1]);

        if((arg == "-h") || (arg == "--help") || (arg == "help"))
        {
            displayHelp();
        }
        else if(arg == "basic")
        {
            std::cout << "Running basic..." << std::endl;
            testBasic();
        }
        else if(arg == "new-elements")
        {
            std::cout << "Running new-elements..." << std::endl;
            testNewElements();
        }
        else if(arg == "scatter")
        {
            std::cout << "Running scatter..." << std::endl;
            testScatter();
        }
        else if(arg == "scatter3")
        {
            std::cout << "Running scatter3..." << std::endl;
            testScatter3();
        }
        else if(arg == "imshow")
        {
            std::cout << "Running imshow..." << std::endl;
            testImShow();
        }
    }
}
