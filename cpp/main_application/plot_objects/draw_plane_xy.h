#ifndef DRAW_PLANE_XY_H_
#define DRAW_PLANE_XY_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

class DrawPlaneXY : public PlotObjectBase
{
private:
    void findMinMax();

    Point3Dd p00;
    Point3Dd p11;
    Point3Dd p01;
    Point3Dd p10;
    Planed plane;

public:
    DrawPlaneXY();
    DrawPlaneXY(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);

    void visualize() override;
};

DrawPlaneXY::DrawPlaneXY(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::PLANE_XY)
    {
        throw std::runtime_error("Invalid function type for DrawPolygon4Points!");
    }

    Vector<PointXY<double>> points;
    points.setInternalData(reinterpret_cast<PointXY<double>*>(data_ptr_), 2);
    PointXY<double> p0 = points(0);
    PointXY<double> p1 = points(1);

    points.setInternalData(nullptr, 0);

    Vector<Plane<double>> planes;
    planes.setInternalData(reinterpret_cast<Plane<double>*>(data_ptr_ + sizeof(PointXY<double>) * 2), 1);
    plane = planes(0);

    planes.setInternalData(nullptr, 0);

    p00 = Point3Dd(p0.x, p0.y, plane.evalXY(p0.x, p0.y));
    p11 = Point3Dd(p1.x, p1.y, plane.evalXY(p1.x, p1.y));
    p01 = Point3Dd(p0.x, p1.y, plane.evalXY(p0.x, p1.y));
    p10 = Point3Dd(p1.x, p0.y, plane.evalXY(p1.x, p0.y));

    findMinMax();
}

void DrawPlaneXY::findMinMax()
{
    const Vectord vx = {p00.x, p01.x, p10.x, p11.x};
    const Vectord vy = {p00.y, p01.y, p10.y, p11.y};
    const Vectord vz = {p00.z, p01.z, p10.z, p11.z};

    min_vec.x = min(vx);
    min_vec.y = min(vy);
    min_vec.z = min(vz);

    max_vec.x = max(vx);
    max_vec.y = max(vy);
    max_vec.z = max(vz);
}

void DrawPlaneXY::visualize()
{
    setColor(color_);
    drawPolygon3D(p00, p01, p11, p10);
}

#endif
