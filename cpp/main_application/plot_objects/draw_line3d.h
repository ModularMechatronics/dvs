#ifndef DRAW_LINE_3D_H_
#define DRAW_LINE_3D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "main_application/plot_objects/utils.h"

class DrawLine3D : public PlotObjectBase
{
private:
    void findMinMax() override;

    Point3Dd p0_;
    Point3Dd p1_;

public:
    DrawLine3D();
    DrawLine3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);

    void visualize() override;
};

DrawLine3D::DrawLine3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::DRAW_LINE3D)
    {
        throw std::runtime_error("Invalid function type for DrawPolygon4Points!");
    }

    Vector<Line3D<double>> lines;
    Vector<double> ts;

    lines.setInternalData(reinterpret_cast<Line3D<double>*>(data_ptr_), 1);
    ts.setInternalData(reinterpret_cast<double*>(&(data_ptr_[sizeof(Line3D<double>)])), 2);

    Line3D<double> line = lines(0);
    double t0 = ts(0);
    double t1 = ts(1);

    lines.setInternalData(nullptr, 0);
    ts.setInternalData(nullptr, 0);

    p0_ = line.eval(t0);
    p1_ = line.eval(t1);

    findMinMax();
}

void DrawLine3D::findMinMax()
{
    min_vec.x = std::min(p0_.x, p1_.x);
    min_vec.y = std::min(p0_.y, p1_.y);
    min_vec.z = std::min(p0_.z, p1_.z);

    max_vec.x = std::max(p0_.x, p1_.x);
    max_vec.y = std::max(p0_.y, p1_.y);
    max_vec.z = std::max(p0_.z, p1_.z);
}

void DrawLine3D::visualize()
{
    setColor(color_);
    setLinewidth(line_width_);
    drawLine3D(p0_, p1_);
}

#endif
