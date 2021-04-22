#ifndef DRAW_PLANE_XY_H_
#define DRAW_PLANE_XY_H_

#include <arl/math/math.h>

#include <string>
#include <vector>

#include "communication/rx_list.h"
#include "main_application/plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

using namespace plot_tool;

class DrawPlaneXY : public PlotObjectBase
{
private:
    void findMinMax();
    void setupInternalData();

    arl::Point3Dd p00;
    arl::Point3Dd p11;
    arl::Point3Dd p01;
    arl::Point3Dd p10;

public:
    DrawPlaneXY();
    DrawPlaneXY(const plot_tool::RxList& rx_list, const std::vector<char*> data_vec);

    void visualize() const override;
};

DrawPlaneXY::DrawPlaneXY(const plot_tool::RxList& rx_list, const std::vector<char*> data_vec)
    : PlotObjectBase(rx_list, data_vec)
{
    ASSERT(rx_list.getObjectData<FunctionRx>() == Function::PLANE_XY);
    ASSERT(rx_list.getObjectData<NumBuffersRequiredRx>() == 1);

    setupInternalData();

    findMinMax();
}

void DrawPlaneXY::setupInternalData()
{
    arl::Planed plane;
    arl::PointXY<double> p0;
    arl::PointXY<double> p1;

    const char* const data_ptr = data_[0];

    fillObjectsFromBuffer(data_ptr, plane, p0, p1);

    p00 = arl::Point3Dd(p0.x, p0.y, plane.evalXY(p0.x, p0.y));
    p11 = arl::Point3Dd(p1.x, p1.y, plane.evalXY(p1.x, p1.y));
    p01 = arl::Point3Dd(p0.x, p1.y, plane.evalXY(p0.x, p1.y));
    p10 = arl::Point3Dd(p1.x, p0.y, plane.evalXY(p1.x, p0.y));
}

void DrawPlaneXY::findMinMax()
{
    const arl::Vectord vx = {p00.x, p01.x, p10.x, p11.x};
    const arl::Vectord vy = {p00.y, p01.y, p10.y, p11.y};
    const arl::Vectord vz = {p00.z, p01.z, p10.z, p11.z};

    min_vec.x = arl::min(vx);
    min_vec.y = arl::min(vy);
    min_vec.z = arl::min(vz);

    max_vec.x = arl::max(vx);
    max_vec.y = arl::max(vy);
    max_vec.z = arl::max(vz);
}

void DrawPlaneXY::visualize() const
{
    setColor(color_);
    drawPolygon3D(p00, p01, p11, p10);
}

#endif
