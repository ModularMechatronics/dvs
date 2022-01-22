#include "main_application/plot_objects/draw_polygon_4_points/draw_polygon_4_points.h"

DrawPolygon4Points::DrawPolygon4Points(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::POLYGON_FROM_4_POINTS)
    {
        throw std::runtime_error("Invalid function type for DrawPolygon4Points!");
    }

    Vector<Point3Dd> points;
    points.setInternalData(reinterpret_cast<Point3Dd*>(data_ptr_), 4);
    p0 = points(0);
    p1 = points(1);
    p2 = points(2);
    p3 = points(3);

    points.setInternalData(nullptr, 0);
}

void DrawPolygon4Points::findMinMax()
{
    const Vectord vx = {p0.x, p1.x, p2.x, p3.x};
    const Vectord vy = {p0.y, p1.y, p2.y, p3.y};
    const Vectord vz = {p0.z, p1.z, p2.z, p3.z};

    min_vec.x = min(vx);
    min_vec.y = min(vy);
    min_vec.z = min(vz);

    max_vec.x = max(vx);
    max_vec.y = max(vy);
    max_vec.z = max(vz);
}

void DrawPolygon4Points::render()
{
    // TODO
    setColor(face_color_);
    // drawPolygon3D(p0, p1, p2, p3);

    setLinewidth(line_width_);
    setColor(edge_color_);
    // drawLine3D(p0, p1);
    // drawLine3D(p1, p2);
    // drawLine3D(p2, p3);
    // drawLine3D(p3, p0);
}
