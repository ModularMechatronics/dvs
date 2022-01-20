#ifndef DRAW_TRIANGLE_2D_H_
#define DRAW_TRIANGLE_2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base.h"

class DrawTriangle3D : public PlotObjectBase
{
private:
    Triangle3D<double> triangle_;

    uint8_t* points_ptr_;
    GLuint buffer_idx_;

    void findMinMax() override;

public:
    DrawTriangle3D();
    DrawTriangle3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawTriangle3D();

    void render() override;
};

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

#endif
