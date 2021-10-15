#ifndef DRAW_ARROW_H_
#define DRAW_ARROW_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"

class DrawArrow : public PlotObjectBase
{
private:
    void findMinMax() override;

    float* points_ptr_;
    GLuint buffer_idx_;

    Point3Dd p0_;
    Point3Dd p1_;
    Point3Dd p1_left_;
    Point3Dd p1_right_;

public:
    DrawArrow();
    DrawArrow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);

    void visualize() override;
};

DrawArrow::DrawArrow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::DRAW_ARROW)
    {
        throw std::runtime_error("Invalid function type for DrawArrow!");
    }

    Vector<Point3D<double>> points;

    points.setInternalData(reinterpret_cast<Point3D<double>*>(data_ptr_), 2);

    p0_ = points(0);
    const Vec3Dd v = points(1);

    points.setInternalData(nullptr, 0);

    const double ang = 30.0 * M_PI / 180.0;

    const double ca = std::cos(ang), sa = std::sin(ang);
    const double nca = std::cos(-ang), nsa = std::sin(-ang);

    const Matrix<double> r_0 = {{ca, -sa}, {sa, ca}};
    const Matrix<double> r_1 = {{nca, -nsa}, {nsa, nca}};

    const double arrow_edge_length_ratio = 0.9;

    // const SphericalCoord spherical(arrow_edge_length_ratio * v.norm());

    const Vec3Dd vec_short = v * arrow_edge_length_ratio;
    const Vec3Dd vec_rotated0 = r_0 * vec_short;
    const Vec3Dd vec_rotated1 = r_1 * vec_short;

    p1_ = p0_ + v;
    p1_left_ = p0_ + vec_rotated0;
    p1_right_ = p0_ + vec_rotated1;

    points_ptr_ = new float[6 * 3];
    points_ptr_[0] = p0_.x;
    points_ptr_[1] = p0_.y;
    points_ptr_[2] = p0_.z;

    points_ptr_[3] = p1_.x;
    points_ptr_[4] = p1_.y;
    points_ptr_[5] = p1_.z;

    points_ptr_[6] = p1_.x;
    points_ptr_[7] = p1_.y;
    points_ptr_[8] = p1_.z;

    points_ptr_[9] = p1_left_.x;
    points_ptr_[10] = p1_left_.y;
    points_ptr_[11] = p1_left_.z;

    points_ptr_[12] = p1_.x;
    points_ptr_[13] = p1_.y;
    points_ptr_[14] = p1_.z;

    points_ptr_[15] = p1_right_.x;
    points_ptr_[16] = p1_right_.y;
    points_ptr_[17] = p1_right_.z;
}

void DrawArrow::findMinMax()
{
    min_vec.x = std::min(p0_.x, p1_.x);
    min_vec.y = std::min(p0_.y, p1_.y);
    min_vec.z = std::min(p0_.z, p1_.z);

    max_vec.x = std::max(p0_.x, p1_.x);
    max_vec.y = std::max(p0_.y, p1_.y);
    max_vec.z = std::max(p0_.z, p1_.z);
}

void DrawArrow::visualize()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6, points_ptr_, GL_STATIC_DRAW);
    }

    setColor(face_color_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    setColor(color_);
    glDrawArrays(GL_LINE_STRIP, 0, 2);

    glDisableVertexAttribArray(0);
}

#endif
