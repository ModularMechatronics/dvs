#ifndef DRAW_LINE_3D_H_
#define DRAW_LINE_3D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"

class DrawLine3D : public PlotObjectBase
{
private:
    void findMinMax() override;

    float* points_ptr_;
    GLuint buffer_idx_;

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
    if ((type_ != Function::DRAW_LINE3D) && (type_ != Function::DRAW_LINE_BETWEEN_POINTS_3D))
    {
        throw std::runtime_error("Invalid function type for DrawLine3D!");
    }

    Vector<Point3D<double>> points;

    points.setInternalData(reinterpret_cast<Point3D<double>*>(data_ptr_), 2);

    p0_ = points(0);
    p1_ = points(1);

    points.setInternalData(nullptr, 0);

    points_ptr_ = new float[2 * 3];
    points_ptr_[0] = p0_.x;
    points_ptr_[1] = p0_.y;
    points_ptr_[2] = p0_.z;

    points_ptr_[3] = p1_.x;
    points_ptr_[4] = p1_.y;
    points_ptr_[5] = p1_.z;
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
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2, points_ptr_, GL_STATIC_DRAW);
    }

    setColor(face_color_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    setColor(color_);
    glDrawArrays(GL_LINES, 0, 2);

    glDisableVertexAttribArray(0);
}

#endif
