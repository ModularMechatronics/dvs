#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tests.h"

typedef void (*TestFunction)();

typedef struct
{
    char* name;
    TestFunction func;
} TestFunctionPair;

// clang-format off
    const TestFunctionPair test_functions[100] = {
        {"plot", testPlot},
        {"plot3", testPlot3},
        {"scatter", testScatter},
        {"scatter3", testScatter3},
        {"surf", testSurf},
        {"drawTriangle", testDrawTriangle},
        {"drawTriangles", testDrawTriangles},
        {"drawLine3D", testDrawLine3D},
        {"drawLine2D", testDrawLine2D},
        {"drawPlaneXY", testDrawPlaneXY},
        {"drawPlaneXZ", testDrawPlaneXZ},
        {"drawPlaneYZ", testDrawPlaneYZ},
        {"drawPolygonFrom4Points", testDrawPolygonFrom4Points},
        {"drawLineBetweenPoints", testDrawLineBetweenPoints},
        {"imShow", testImShow},
        {0, 0}}; // '{0, 0}' must be last element in array
// clang-format on

void displayHelp()
{
    printf("Usage: ./test-app [-h, --help, help] [");
    int idx = 0;
    while (test_functions[idx].name)
    {
        printf("%s, ", test_functions[idx].name);
        idx++;
    }

    printf("all]\n");
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("No arguments provided!\n");
        displayHelp();
        exit(0);
    }
    else if ((strcmp(argv[1], "-h") == 0) || (strcmp(argv[1], "--help") == 0) || (strcmp(argv[1], "help") == 0))
    {
        displayHelp();
        exit(0);
    }
    else if ((strcmp(argv[1], "all") == 0))
    {
        int idx = 0;
        while (test_functions[idx].name)
        {
            printf(" - Running function: %s\n", test_functions[idx].name);
            test_functions[idx].func();
            printf("\n ------------------------\n\n");

            idx++;
        }
    }
    else
    {
        int idx = 0;
        int found_function = 0;
        while (test_functions[idx].name)
        {
            if (strcmp(argv[1], test_functions[idx].name) == 0)
            {
                printf(" - Running function: %s\n", test_functions[idx].name);
                test_functions[idx].func();
                printf("\n ------------------------\n\n");
                found_function = 1;
                break;
            }

            idx++;
        }
        if (!found_function)
        {
            printf("Error, couldn't find function \"%s\".\n", argv[1]);
            displayHelp();
            exit(0);
        }
    }
}
