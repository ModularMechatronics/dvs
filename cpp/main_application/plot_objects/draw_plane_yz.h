#ifndef DRAW_PLANE_YZ_H_
#define DRAW_PLANE_YZ_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"

class DrawPlaneYZ : public PlotObjectBase
{
private:
    Point3Dd p00;
    Point3Dd p11;
    Point3Dd p01;
    Point3Dd p10;

    GLuint buffer_idx_;
    float* points_ptr_;

    void findMinMax() override;

public:
    DrawPlaneYZ();
    DrawPlaneYZ(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawPlaneYZ();

    void visualize() override;
};

DrawPlaneYZ::DrawPlaneYZ(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::PLANE_YZ)
    {
        throw std::runtime_error("Invalid function type for DrawPolygon4Points!");
    }

    Vector<PointYZ<double>> points;
    points.setInternalData(reinterpret_cast<PointYZ<double>*>(data_ptr_), 2);
    PointYZ<double> p0 = points(0);
    PointYZ<double> p1 = points(1);

    points.setInternalData(nullptr, 0);

    Vector<Plane<double>> planes;
    planes.setInternalData(reinterpret_cast<Plane<double>*>(data_ptr_ + sizeof(PointYZ<double>) * 2), 1);
    const Planed plane = planes(0);

    planes.setInternalData(nullptr, 0);

    p00 = Point3Dd(plane.evalYZ(p0.y, p0.z), p0.y, p0.z);
    p11 = Point3Dd(plane.evalYZ(p1.y, p1.z), p1.y, p1.z);
    p01 = Point3Dd(plane.evalYZ(p0.y, p1.z), p0.y, p1.z);
    p10 = Point3Dd(plane.evalYZ(p1.y, p0.z), p1.y, p0.z);

    points_ptr_ = new float[4 * 3];
    points_ptr_[0] = p00.x;
    points_ptr_[1] = p00.y;
    points_ptr_[2] = p00.z;

    points_ptr_[3] = p01.x;
    points_ptr_[4] = p01.y;
    points_ptr_[5] = p01.z;

    points_ptr_[6] = p11.x;
    points_ptr_[7] = p11.y;
    points_ptr_[8] = p11.z;

    points_ptr_[9] = p10.x;
    points_ptr_[10] = p10.y;
    points_ptr_[11] = p10.z;
}

void DrawPlaneYZ::findMinMax()
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

void DrawPlaneYZ::visualize()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4, points_ptr_, GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    setColor(face_color_);
    glDrawArrays(GL_QUADS, 0, 4 * 3);
    setColor(edge_color_);
    glDrawArrays(GL_LINE_LOOP, 0, 4);

    glDisableVertexAttribArray(0);
}

DrawPlaneYZ::~DrawPlaneYZ()
{
    delete[] points_ptr_;
}

#endif
