#ifndef DRAW_MESH_H_
#define DRAW_MESH_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base.h"

class DrawMesh : public PlotObjectBase
{
private:
    float* points_ptr_;

    GLuint buffer_handle_;
    uint32_t num_vertices_;
    uint32_t num_indices_;

    void findMinMax() override;

public:
    DrawMesh();
    DrawMesh(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawMesh();

    void render() override;
};

DrawMesh::DrawMesh(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::DRAW_MESH)
    {
        throw std::runtime_error("Invalid function type for DrawMesh!");
    }

    num_vertices_ = hdr.get(FunctionHeaderObjectType::NUM_VERTICES).as<uint32_t>();
    num_indices_ = hdr.get(FunctionHeaderObjectType::NUM_INDICES).as<uint32_t>();

    points_ptr_ = convertVerticesDataOuter(data_ptr_, data_type_, num_vertices_, num_indices_, num_bytes_per_element_);
}

void DrawMesh::findMinMax()
{
    min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    max_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    size_t idx = 0;
    for (size_t k = 0; k < (num_indices_ * 3 * 3); k += 3)
    {
        const Point3Dd current_point(points_ptr_[k], points_ptr_[k + 1], points_ptr_[k + 2]);
        min_vec.x = std::min(current_point.x, min_vec.x);
        min_vec.y = std::min(current_point.y, min_vec.y);
        min_vec.z = std::min(current_point.z, min_vec.z);

        max_vec.x = std::max(current_point.x, max_vec.x);
        max_vec.y = std::max(current_point.y, max_vec.y);
        max_vec.z = std::max(current_point.z, max_vec.z);

        idx += 3;
    }
}

void DrawMesh::render()
{
    if (!visualize_has_run_)
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

DrawMesh::~DrawMesh() {}

#endif
