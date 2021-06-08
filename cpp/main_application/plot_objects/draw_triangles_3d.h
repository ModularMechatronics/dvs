#ifndef DRAW_TRIANGLES_3D_H_
#define DRAW_TRIANGLES_3D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "main_application/plot_objects/utils.h"

class DrawTriangles3D : public PlotObjectBase
{
private:
    Vector<Triangle3D<double>> triangles_;

    uint8_t* points_ptr_;
    GLuint buffer_idx_;

    void findMinMax() override;

public:
    DrawTriangles3D();
    DrawTriangles3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawTriangles3D();

    void visualize() override;
};

DrawTriangles3D::DrawTriangles3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::DRAW_TRIANGLES_3D)
    {
        throw std::runtime_error("Invalid function type for DrawTriangles3D!");
    }

    
    triangles_.setInternalData(reinterpret_cast<Triangle3D<double>*>(data_ptr_), num_elements_);
}

void DrawTriangles3D::findMinMax()
{
    min_vec = triangles_(0).p0;
    max_vec = triangles_(0).p0;
    for(size_t k = 0; k < num_elements_; k++)
    {
        min_vec.x = std::min(triangles_(k).p0.x, min_vec.x);
        min_vec.x = std::min(triangles_(k).p1.x, min_vec.x);
        min_vec.x = std::min(triangles_(k).p2.x, min_vec.x);

        min_vec.y = std::min(triangles_(k).p0.y, min_vec.y);
        min_vec.y = std::min(triangles_(k).p1.y, min_vec.y);
        min_vec.y = std::min(triangles_(k).p2.y, min_vec.y);

        min_vec.z = std::min(triangles_(k).p0.z, min_vec.z);
        min_vec.z = std::min(triangles_(k).p1.z, min_vec.z);
        min_vec.z = std::min(triangles_(k).p2.z, min_vec.z);

        max_vec.x = std::max(triangles_(k).p0.x, min_vec.x);
        max_vec.x = std::max(triangles_(k).p1.x, min_vec.x);
        max_vec.x = std::max(triangles_(k).p2.x, min_vec.x);

        max_vec.y = std::max(triangles_(k).p0.y, min_vec.y);
        max_vec.y = std::max(triangles_(k).p1.y, min_vec.y);
        max_vec.y = std::max(triangles_(k).p2.y, min_vec.y);

        max_vec.z = std::max(triangles_(k).p0.z, min_vec.z);
        max_vec.z = std::max(triangles_(k).p1.z, min_vec.z);
        max_vec.z = std::max(triangles_(k).p2.z, min_vec.z);
    }
}

void DrawTriangles3D::visualize()
{
    setColor(face_color_);
    for(size_t k = 0; k < num_elements_; k++)
    {
        drawTriangle3D(triangles_(k));
    }

    setLinewidth(line_width_);
    setColor(edge_color_);
    for(size_t k = 0; k < num_elements_; k++)
    {
        drawTriangleEdge3D(triangles_(k));
    }
}

DrawTriangles3D::~DrawTriangles3D()
{
    triangles_.setInternalData(nullptr, 0);
}

#endif
