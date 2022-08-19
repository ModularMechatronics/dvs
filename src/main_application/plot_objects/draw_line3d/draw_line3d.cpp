#include "main_application/plot_objects/draw_line3d/draw_line3d.h"


DrawLine3D::DrawLine3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
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

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
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

void DrawLine3D::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, 2);
    glBindVertexArray(0);
}
