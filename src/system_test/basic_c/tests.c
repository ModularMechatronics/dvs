#include "basic_c/tests.h"

#include <stdio.h>

#include "duoplot/duoplot.h"

void testPlot()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    const Vec3D min_vec = {-1.0, -1.0, -1.0};
    const Vec3D max_vec = {1.0, 1.0, 2.0};
    duoplot_axis(min_vec, max_vec);

    duoplot_plot(x, y, duoplot_Color(0, 0, 255), duoplot_LineWidth(53));
}

void testPlot3()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);
    VectorF z = vector_cosF(x);

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    duoplot_plot3(x, y, z, duoplot_Color(255, 0, 11));
}

void testScatter()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    duoplot_scatter(x, y, duoplot_Color(0, 0, 255), duoplot_PointSize(10));
}

void testScatter3()
{
    const size_t num_elements = 100;
    VectorF x = vector_linspaceFromPointsAndCountF(0.1, 10.8, num_elements);
    VectorF y = vector_sinF(x);
    VectorF z = vector_cosF(x);

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    duoplot_scatter3(x, y, z, duoplot_Color(255, 0, 11));
}

void testSurf()
{
    const size_t num_rows = 100, num_cols = 80;
    const MatrixPairD mp = meshgridD(0.0, 4.0, 1.0, 5.0, num_cols, num_rows);

    const MatrixD x = mp.m0, y = mp.m1;

    const MatrixD rx = matrix_sinD(x);
    const MatrixD ry = matrix_cosD(y);
    const MatrixD z = matrix_elementWiseMultiplyD(rx, ry);

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();

    duoplot_surf(x, y, z, duoplot_Alpha(0.7f));

    free(x.data);
    free(y.data);
    free(z.data);

    free(rx.data);
    free(ry.data);
}

/*void testDrawPolygonFrom4Points()
{
    Point3D p0 = {0.0, 0.0, 0.0};
    Point3D p1 = {0.0, 1.0, 1.0};
    Point3D p2 = {1.0, 1.0, 2.0};
    Point3D p3 = {0.0, 2.0, 0.6};

    duoplot_setCurrentElement("view_02");
    duoplot_clearView();

    drawPolygonFrom4Points(p0, p1, p2, p3, duoplot_Color(255, 0, 11));
}

void testDrawLine3D()
{
    const Point3D p0 = createPoint3D(0.0, 0.0, 0.0);
    const Point3D p1 = createPoint3D(1.0, 1.0, 1.0);

    const Line3DD line = {p0, p1};

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    drawLine3D(line, -1.3, 3.4, duoplot_Color(255, 0, 11));
}

void testDrawLine2D()
{
    const Point2d p0 = {0.0, 0.0};
    const Point2d p1 = {1.0, 1.5};

    const PLine2DD line = {p0, p1};

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    drawLine2D(line, -1.3, 3.4, duoplot_Color(255, 0, 11));
}

void testDrawPlaneXY()
{
    const PlaneD plane = {0.0, 0.2, 1.0, 0.5};
    const PointXYd p0 = {0.0, 0.0};
    const PointXYd p1 = {1.0, 1.0};

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    drawPlaneXY(plane, p0, p1, duoplot_Color(255, 0, 11));
}

void testDrawPlaneXZ()
{
    const PlaneD plane = {0.0, 1.0, 0.2, 0.5};
    const PointXZd p0 = {0.0, 0.0};
    const PointXZd p1 = {1.0, 1.0};

    duoplot_setCurrentElement("view_01");
    duoplot_clearView();
    drawPlaneXZ(plane, p0, p1, duoplot_Color(255, 0, 11));
}

void testDrawPlaneYZ()
{
    const PlaneD plane = {1.0, 0.0, 0.2, 0.5};
    const PointYZd p0 = {0.0, 0.0};
    const PointYZd p1 = {1.0, 1.0};

    duoplot_setCurrentElement("view_02");
    duoplot_clearView();
    drawPlaneYZ(plane, p0, p1, duoplot_Color(255, 0, 11));
}*/

void testImShow()
{
    const uint32_t num_rows = 800, num_cols = 800;
    ImageC3F img3 = createImageC3F(num_rows, num_cols);
    ImageC1F img1 = createImageC1F(num_rows, num_cols);

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

            img3.data[r * img3.num_cols + c] = (sin(rb) / rr + 1.0f) * 0.5f;
            img3.data[num_elements + r * img3.num_cols + c] = (sin(rb) / rg + 1.0f) * 0.5f;
            img3.data[2 * num_elements + r * img3.num_cols + c] = (sin(rb) / rb + 1.0f) * 0.5f;

            // img1.data[num_elements + r * img3.num_cols + c] = (sin(rb) / rb + 1.0f) * 0.5f;
        }
    }

    const duoplot_Property p = duoplot_Alpha(0.3);

    duoplot_setCurrentElement("p_view_0");
    duoplot_clearView();
    duoplot_imShow(img3, p);

    // TODO:
    // duoplot_setCurrentElement("view_01");
    // duoplot_clearView();
    // duoplot_imShow(img1, duoplot_Color(255, 0, 11));
}

/*void testDrawLineBetweenPoints()
{
    const Point3D p0 = {0.1, 0.1, 0.1};
    const Point3D p1 = {1.0, 2.0, 3.0};

    const Point2d p0_2 = {p0.x, p0.y};
    const Point2d p1_2 = {p1.x, p1.y};

    duoplot_setCurrentElement("p_view_0");  // TODO: <- Move to setup
    duoplot_clearView();
    drawLineBetweenPoints(p0, p1, duoplot_Color(255, 0, 11));
    drawLineBetween2DPoints(p0_2, p1_2, duoplot_Color(255, 0, 11));
}*/
