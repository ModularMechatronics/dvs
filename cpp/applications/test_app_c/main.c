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
    VectorF vq = vector_linspaceFromPointsAndCountF(0.1, 0.8, num_elements);

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
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);

    setCurrentElement("view_00");
    clearView();
    plot(x, y, Color(255, 0, 11));
    scatter(x, y, Color(255, 0, 11));
}

void testSurf()
{
    const size_t num_rows = 100, num_cols = 80;
    const MatrixPairD mp = meshgridD(0.0, 4.0, 1.0, 5.0, num_cols, num_rows);

    const MatrixD x = mp.m0, y = mp.m1;

    const MatrixD rx = matrix_sinD(x);
    const MatrixD ry = matrix_cosD(y);
    const MatrixD z = matrix_elementWiseMultiplyD(rx, ry);

    setCurrentElement("view_00");
    clearView();
    surf(x, y, z, Color(255, 0, 11));

    free(x.data);
    free(y.data);
    free(z.data);

    free(rx.data);
    free(ry.data);
}

void testDrawPolygonFrom4Points()
{
    Point3DD p0 = {0.0, 0.0, 0.0};
    Point3DD p1 = {0.0, 1.0, 1.0};
    Point3DD p2 = {1.0, 1.0, 2.0};
    Point3DD p3 = {0.0, 2.0, 0.6};

    setCurrentElement("view_02");
    clearView();

    drawPolygonFrom4Points(p0, p1, p2, p3, Color(255, 0, 11));
}

void testPlot3()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);
    VectorF z = vector_cosF(x);

    setCurrentElement("view_00");
    clearView();
    plot3(x, y, z, Color(255, 0, 11));
    scatter3(x, y, z, Color(255, 0, 11));
}

void testDrawTriangle()
{
    Triangle3DD triangle;

    triangle.p0 = createPoint3DD(0.0, 0.0, 0.0);
    triangle.p1 = createPoint3DD(1.0, 1.0, 0.0);
    triangle.p2 = createPoint3DD(1.5, 0.0, 0.0);

    setCurrentElement("view_00");
    clearView();
    drawTriangle(triangle, Color(255, 0, 11));
}

void testDrawTriangles()
{
    const size_t num_elements = 10;

    const Point3DF p0 = createPoint3DF(0.0, 0.0, 0.0);
    const Point3DF p1 = createPoint3DF(1.0, 1.0, 0.0);
    const Point3DF p2 = createPoint3DF(1.5, 0.0, 0.0);

    Triangle3DFArray tri_array = createTriangle3DFArray(num_elements);

    Triangle3DF triangle = createTriangle3DF(p0, p1, p2);
    for (size_t k = 0; k < num_elements; k++)
    {
        triangle.p0.z += 0.1;
        triangle.p1.z += 0.15;
        triangle.p2.z += 0.12;

        tri_array.elements[k] = triangle;
    }

    setCurrentElement("view_00");
    clearView();
    drawTriangles(tri_array, Color(255, 0, 11));
}

void testDrawLine()
{
    const Point3DD p0 = createPoint3DD(0.0, 0.0, 0.0);
    const Point3DD p1 = createPoint3DD(1.0, 1.0, 1.0);

    const Line3DD line = {p0, p1};

    setCurrentElement("view_00");
    clearView();
    drawLine(line, -1.3, 3.4, Color(255, 0, 11));
}

void testDrawLine2D()
{
    const Point2DD p0 = {0.0, 0.0};
    const Point2DD p1 = {1.0, 1.5};

    const PLine2DD line = {p0, p1};

    setCurrentElement("view_00");
    clearView();
    drawLine2D(line, -1.3, 3.4, Color(255, 0, 11));
}

void testDrawPlaneXY()
{
    const PlaneD plane = {0.0, 0.2, 1.0, 0.5};
    const PointXYD p0 = {0.0, 0.0};
    const PointXYD p1 = {1.0, 1.0};

    setCurrentElement("view_00");
    clearView();
    drawPlaneXY(plane, p0, p1, Color(255, 0, 11));
}

void testDrawPlaneXZ()
{
    const PlaneD plane = {0.0, 1.0, 0.2, 0.5};
    const PointXZD p0 = {0.0, 0.0};
    const PointXZD p1 = {1.0, 1.0};

    setCurrentElement("view_01");
    clearView();
    drawPlaneXZ(plane, p0, p1, Color(255, 0, 11));
}

void testDrawPlaneYZ()
{
    const PlaneD plane = {1.0, 0.0, 0.2, 0.5};
    const PointYZD p0 = {0.0, 0.0};
    const PointYZD p1 = {1.0, 1.0};

    setCurrentElement("view_02");
    clearView();
    drawPlaneYZ(plane, p0, p1, Color(255, 0, 11));
}

void testImShow()
{
    const uint32_t num_rows = 800, num_cols = 800;
    ImageC3 img = createImageC3(num_rows, num_cols, DT_FLOAT);

    const double max_val = num_rows * num_cols;
    const size_t num_elements = num_rows * num_cols;

    for (uint32_t r = 0; r < num_rows; r++)
    {
        for (uint32_t c = 0; c < num_cols; c++)
        {
            const float xr = 3.0f * ((float)c - 300.5f) / 800.0f;
            const float yr = 3.0f * ((float)r - 400.5f) / 800.0f;
            const float rr = sqrt(xr * xr + yr * yr);

            const float xg = 2.0f * ((float)c - 500.5f) / 800.0f;
            const float yg = 2.0f * ((float)r - 350.5f) / 800.0f;
            const float rg = sqrt(xg * xg + yg * yg);

            const float xb = 4.0f * ((float)c - 200.5f) / 800.0f;
            const float yb = 4.0f * ((float)r - 600.5f) / 800.0f;
            const float rb = sqrt(xb * xb + yb * yb);

            img.data[r * img.num_cols + c] = (sin(rb) / rr + 1.0f) * 0.5f;
            img.data[num_elements + r * img.num_cols + c] = (sin(rb) / rg + 1.0f) * 0.5f;
            img.data[2 * num_elements + r * img.num_cols + c] = (sin(rb) / rb + 1.0f) * 0.5f;
        }
    }

    setCurrentElement("view_00");
    clearView();
    imShow(img, Color(255, 0, 11));
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
        {"testPlot3", testPlot3},
        {"testSurf", testSurf},
        {"testDrawTriangle", testDrawTriangle},
        {"testDrawTriangles", testDrawTriangles},
        {"testDrawLine", testDrawLine},
        {"testDrawLine2D", testDrawLine2D},
        {"testDrawPlaneXY", testDrawPlaneXY},
        {"testDrawPlaneXZ", testDrawPlaneXZ},
        {"testDrawPlaneYZ", testDrawPlaneYZ},
        {"testDrawPolygonFrom4Points", testDrawPolygonFrom4Points},
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
