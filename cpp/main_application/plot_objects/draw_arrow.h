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

    Point2Dd p0_;
    Point2Dd p1_;
    Point2Dd p1_left_;
    Point2Dd p1_right_;

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

    Vector<Point2D<double>> points;

    points.setInternalData(reinterpret_cast<Point2D<double>*>(data_ptr_), 2);

    p0_ = points(0);
    const Vec2Dd v = points(1);

    points.setInternalData(nullptr, 0);

    const double ang = 10.0 * M_PI / 180.0;

    const double ca = std::cos(ang), sa = std::sin(ang);
    const double nca = std::cos(-ang), nsa = std::sin(-ang);

    const Matrix<double> r_0 = {{ca, -sa}, {sa, ca}};
    const Matrix<double> r_1 = {{nca, -nsa}, {nsa, nca}};

    const double arrow_edge_length_ratio = 0.9;

    const Vec2Dd vec_short = v * arrow_edge_length_ratio;
    const Vec2Dd vec_rotated0 = r_0 * vec_short;
    const Vec2Dd vec_rotated1 = r_1 * vec_short;

    p1_ = p0_ + v;
    p1_left_ = p0_ + vec_rotated0;
    p1_right_ = p0_ + vec_rotated1;

    points_ptr_ = new float[6 * 2];
    points_ptr_[0] = p0_.x;
    points_ptr_[1] = p0_.y;

    points_ptr_[2] = p1_.x;
    points_ptr_[3] = p1_.y;

    points_ptr_[4] = p1_.x;
    points_ptr_[5] = p1_.y;

    points_ptr_[6] = p1_left_.x;
    points_ptr_[7] = p1_left_.y;

    points_ptr_[8] = p1_.x;
    points_ptr_[9] = p1_.y;

    points_ptr_[10] = p1_right_.x;
    points_ptr_[11] = p1_right_.y;
}

void DrawArrow::findMinMax()
{
    min_vec.x = std::min(p0_.x, p1_.x);
    min_vec.y = std::min(p0_.y, p1_.y);
    min_vec.z = -1.0;

    max_vec.x = std::max(p0_.x, p1_.x);
    max_vec.y = std::max(p0_.y, p1_.y);
    max_vec.z = 1.0;
}

void DrawArrow::visualize()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6, points_ptr_, GL_STATIC_DRAW);
    }

    setColor(face_color_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    setColor(color_);
    glDrawArrays(GL_LINES, 0, 2 * 6);

    glDisableVertexAttribArray(0);
}

#endif
