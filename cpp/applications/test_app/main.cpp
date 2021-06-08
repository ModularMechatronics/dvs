#include <algorithm>

#include "dvs.h"

extern void testBasic();
extern void testNewElements();
extern void testScatter();
extern void testScatter3();
extern void testPlot3();
extern void testSurf();
extern void testImShow();
extern void testPolygonFrom4Points();
extern void testdrawXYPlane();
extern void testMultipleStuff();
extern void testAxis2D();
extern void testAxis3D();
extern void testDrawTriangles();

void displayHelp()
{
    std::cout << "Usage: ./test-app [-h, --help, help] [basic, new-elements, scatter,"
                " scatter3, plot3, surf, imshow, polygon-4-points, xy-plane"
                ", multiple, axis2d, axis3d, triangles]" << std::endl;
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
        else if(arg == "plot3")
        {
            std::cout << "Running plot3..." << std::endl;
            testPlot3();
        }
        else if(arg == "surf")
        {
            std::cout << "Running surf..." << std::endl;
            testSurf();
        }
        else if(arg == "imshow")
        {
            std::cout << "Running imshow..." << std::endl;
            testImShow();
        }
        else if(arg == "polygon-4-points")
        {
            std::cout << "Running polygonFrom4Points..." << std::endl;
            testPolygonFrom4Points();
        }
        else if(arg == "xy-plane")
        {
            std::cout << "Running xy-plane..." << std::endl;
            testdrawXYPlane();
        }
        else if(arg == "xy-plane")
        {
            std::cout << "Running xy-plane..." << std::endl;
            testdrawXYPlane();
        }
        else if(arg == "multiple")
        {
            std::cout << "Running multiple..." << std::endl;
            testMultipleStuff();
        }
        else if(arg == "axis2d")
        {
            std::cout << "Running axis2d..." << std::endl;
            testAxis2D();
        }
        else if(arg == "axis3d")
        {
            std::cout << "Running axis3d..." << std::endl;
            testAxis3D();
        }
        else if(arg == "triangles")
        {
            std::cout << "Running triangles..." << std::endl;
            testDrawTriangles();
        }
        else
        {
            std::cout << "ERROR: No matching arg!" << std::endl;
        }
        
    }
}
