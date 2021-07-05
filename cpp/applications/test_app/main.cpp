#include <algorithm>
#include <vector>
#include <functional>
#include <map>

#include "dvs.h"

extern void testBasic();
extern void testNewElements();
extern void testScatter();
extern void testScatter3();
extern void testPlot2();
extern void testPlot3();
extern void testSurf();
extern void testImShow();
extern void testPolygonFrom4Points();
extern void testdrawXYPlane();
extern void testMultipleStuff();
extern void testAxis2D();
extern void testAxis3D();
extern void testDrawTriangles();
extern void testDrawMesh();
extern void testDrawMeshAdvanced();
extern void testDrawSingleTriangle();
extern void testDrawLine3D();
extern void testDrawLineBetweenPoints3D();
extern void testDrawLine2D();

std::map<std::string, std::function<void()>> fcns;

void displayHelp()
{
    std::cout << "Usage: ./test-app [-h, --help, help] [";
    std::map<std::string, std::function<void()>>::iterator it;

    for (it = fcns.begin(); it != fcns.end(); it++)
    {
        std::cout << it->first << ", ";
    }

    std::cout << "]" << std::endl;
}

int main(int argc, char* argv[])
{
    fcns["new-elements"] = testNewElements;
    fcns["scatter"] = testScatter;
    fcns["scatter3"] = testScatter3;
    fcns["plot3"] = testPlot3;
    fcns["plot2"] = testPlot2;
    fcns["surf"] = testSurf;
    fcns["imshow"] = testImShow;
    fcns["polygon-4-points"] = testPolygonFrom4Points;
    fcns["xy-plane"] = testdrawXYPlane;
    fcns["xy-plane"] = testdrawXYPlane;
    fcns["multiple"] = testMultipleStuff;
    fcns["axis2d"] = testAxis2D;
    fcns["axis3d"] = testAxis3D;
    fcns["triangles"] = testDrawTriangles;
    fcns["mesh"] = testDrawMesh;
    fcns["mesh-adv"] = testDrawMeshAdvanced;
    fcns["single-triangle"] = testDrawSingleTriangle;
    fcns["draw-line-3d"] = testDrawLine3D;
    fcns["draw-line-2d"] = testDrawLine2D;
    fcns["draw-line-between-points"] = testDrawLineBetweenPoints3D;

    if(argc == 1)
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
        else if(fcns.count(arg) > 0)
        {
            std::map<std::string, std::function<void()>>::iterator it;

            for (it = fcns.begin(); it != fcns.end(); it++)
            {
                if(arg == it->first)
                {
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
