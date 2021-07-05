#ifndef DRAW_MESH_H_
#define DRAW_MESH_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "main_application/plot_objects/utils.h"

class DrawMesh : public PlotObjectBase
{
private:
    Vector<Triangle3D<double>> triangles_;

    float* points_ptr_;

    GLuint buffer_handle_;
    uint32_t num_vertices_;
    uint32_t num_indices_;

    void findMinMax() override;

public:
    DrawMesh();
    DrawMesh(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawMesh();

    void visualize() override;
};

DrawMesh::DrawMesh(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::DRAW_MESH)
    {
        throw std::runtime_error("Invalid function type for DrawMesh!");
    }

    num_vertices_ = hdr.getObjectFromType(FunctionHeaderObjectType::NUM_VERTICES).getAs<uint32_t>();
    num_indices_ = hdr.getObjectFromType(FunctionHeaderObjectType::NUM_INDICES).getAs<uint32_t>();

    triangles_.resize(num_indices_);

    Vector<Point3D<double>> vertices;
    Vector<IndexTriplet> indices;

    vertices.setInternalData(reinterpret_cast<Point3D<double>*>(data_ptr_), num_vertices_);
    indices.setInternalData(reinterpret_cast<IndexTriplet*>(&(data_ptr_[num_vertices_ * sizeof(Point3D<double>)])), num_indices_);

    points_ptr_ = new float[num_indices_ * 3 * 3];
    size_t idx = 0;

    for(size_t k = 0; k < num_indices_; k++)
    {
        const Point3D<double> p0 = vertices(indices(k).i0);
        const Point3D<double> p1 = vertices(indices(k).i1);
        const Point3D<double> p2 = vertices(indices(k).i2);

        points_ptr_[idx] = p0.x;
        points_ptr_[idx + 1] = p0.y;
        points_ptr_[idx + 2] = p0.z;

        points_ptr_[idx + 3] = p1.x;
        points_ptr_[idx + 4] = p1.y;
        points_ptr_[idx + 5] = p1.z;

        points_ptr_[idx + 6] = p2.x;
        points_ptr_[idx + 7] = p2.y;
        points_ptr_[idx + 8] = p2.z;
        idx += 9;
    }

    for(size_t k = 0; k < num_indices_; k++)
    {
        Triangle3D<double> tri(vertices(indices(k).i0), vertices(indices(k).i1), vertices(indices(k).i2));
        triangles_(k) = tri;
    }

    vertices.setInternalData(nullptr, 0);
    indices.setInternalData(nullptr, 0);

}

void DrawMesh::findMinMax()
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

void DrawMesh::visualize()
{
    if(!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_handle_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_indices_ * 3 * 3, points_ptr_, GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    setLinewidth(line_width_);

    glPolygonOffset(1, -1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    setColor(face_color_);
    glDrawArrays(GL_TRIANGLES, 0, num_indices_ * 3);
    setColor(edge_color_);

    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_TRIANGLES, 0, num_indices_ * 3);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisableVertexAttribArray(0);
}

DrawMesh::~DrawMesh()
{
}

#endif
