#include "main_application/plot_objects/draw_triangle_3d/draw_triangle_3d.h"

DrawTriangle3D::DrawTriangle3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::DRAW_TRIANGLE_3D)
    {
        throw std::runtime_error("Invalid function type for DrawTriangle3D!");
    }

    Vector<Triangle3D<double>> triangles;

    triangles.setInternalData(reinterpret_cast<Triangle3D<double>*>(data_ptr_), 1);
    triangle_ = triangles(0);
}

void DrawTriangle3D::findMinMax()
{
    min_vec.x = std::min(triangle_.p0.x, min_vec.x);
    min_vec.x = std::min(triangle_.p1.x, min_vec.x);
    min_vec.x = std::min(triangle_.p2.x, min_vec.x);

    min_vec.y = std::min(triangle_.p0.y, min_vec.y);
    min_vec.y = std::min(triangle_.p1.y, min_vec.y);
    min_vec.y = std::min(triangle_.p2.y, min_vec.y);

    min_vec.z = std::min(triangle_.p0.z, min_vec.z);
    min_vec.z = std::min(triangle_.p1.z, min_vec.z);
    min_vec.z = std::min(triangle_.p2.z, min_vec.z);

    max_vec.x = std::max(triangle_.p0.x, min_vec.x);
    max_vec.x = std::max(triangle_.p1.x, min_vec.x);
    max_vec.x = std::max(triangle_.p2.x, min_vec.x);

    max_vec.y = std::max(triangle_.p0.y, min_vec.y);
    max_vec.y = std::max(triangle_.p1.y, min_vec.y);
    max_vec.y = std::max(triangle_.p2.y, min_vec.y);

    max_vec.z = std::max(triangle_.p0.z, min_vec.z);
    max_vec.z = std::max(triangle_.p1.z, min_vec.z);
    max_vec.z = std::max(triangle_.p2.z, min_vec.z);
}

void DrawTriangle3D::render()
{
    // TODO
    setColor(face_color_);
    // drawTriangle3D(triangle_);

    setLinewidth(line_width_);
    setColor(edge_color_);
    // drawTriangleEdge3D(triangle_);
}

DrawTriangle3D::~DrawTriangle3D() {}
