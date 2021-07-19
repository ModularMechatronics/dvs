#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dvs.h"

void testMisc()
{
    const size_t num_elements = 10;
    Vector v = createVector(num_elements, DT_FLOAT);
    VectorF v2 = toVectorF(v);
    VectorF v1 = createVectorF(num_elements);
    VectorF vq = linspaceFromPointsAndCountF(0.1, 0.8, num_elements);

    for (size_t k = 0; k < num_elements; k++)
    {
        v2.data[k] = ((float)k) * 1.1f;
        printf("Data: %f\n", vq.data[k]);
    }

    printf("Num elements: %lu\n", vq.num_elements);

    freeVector(v);
}

void testPlot()
{
    const size_t num_elements = 10;
    VectorF x = linspaceFromPointsAndCountF(0.1, 0.8, num_elements);
    VectorF y = dvsSinF(x);

    FunctionHeaderObject obj0, obj1, obj2;

    strcpy(((char*)obj0.data), "obj0");
    strcpy(((char*)obj1.data), "obj1");
    strcpy(((char*)obj2.data), "obj2");
    obj0.type = FHOT_FUNCTION;
    obj1.type = FHOT_FUNCTION;
    obj2.type = FHOT_FUNCTION;

    plot(x, y, Color(2, 220, 11));
    // plot(x, y);
}

typedef void (*TestFunction)();

typedef struct S_TestFunctionPair
{
    char* name;
    TestFunction func;
} TestFunctionPair;

// clang-format off
    const TestFunctionPair test_functions[100] = {
        {"testMisc", testMisc},
        {"testPlot", testPlot},
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
