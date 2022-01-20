#ifndef DRAW_TILES_H_
#define DRAW_TILES_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "dvs/utils.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base.h"

class DrawTiles : public PlotObjectBase
{
private:
    float* points_ptr_;
    Dimension2D dims_;
    GLuint buffer_handle_;
    Vec2Dd tile_size_;

    void findMinMax() override;

public:
    DrawTiles();
    DrawTiles(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawTiles();

    void render() override;
};

DrawTiles::DrawTiles(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::DRAW_TILES)
    {
        throw std::runtime_error("Invalid function type for DrawTiles!");
    }

    dims_ = hdr.get(FunctionHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();
    const size_t num_bytes_for_matrix = num_bytes_per_element_ * dims_.rows * dims_.cols;

    fillObjectsFromBuffer(&(data_ptr_[num_bytes_for_matrix]), tile_size_);

    points_ptr_ = convertSingleMatrixDataOuter(data_ptr_, data_type_, dims_, tile_size_);
}

void DrawTiles::findMinMax()
{
    min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    max_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    // TODO: Broken? Also, find x-y-min/max from tile_size_ and dimensions, and iterate only over z
    size_t idx = 0;
    for (size_t k = 0; k < (dims_.rows * dims_.cols * 3); k++)
    {
        const Point3Dd current_point(points_ptr_[idx], points_ptr_[idx + 1], points_ptr_[idx + 2]);
        min_vec.x = std::min(current_point.x, min_vec.x);
        min_vec.y = std::min(current_point.y, min_vec.y);
        min_vec.z = std::min(current_point.z, min_vec.z);

        max_vec.x = std::max(current_point.x, max_vec.x);
        max_vec.y = std::max(current_point.y, max_vec.y);
        max_vec.z = std::max(current_point.z, max_vec.z);

        idx += 3;
    }
}

void DrawTiles::render()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_handle_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * dims_.rows * dims_.cols, points_ptr_, GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    setLinewidth(line_width_);

    glPolygonOffset(1, -1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    setColor(face_color_);
    glDrawArrays(GL_QUADS, 0, 4 * dims_.rows * dims_.cols);
    setColor(edge_color_);

    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_QUADS, 0, 4 * dims_.rows * dims_.cols);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisableVertexAttribArray(0);
}

DrawTiles::~DrawTiles()
{
    delete[] points_ptr_;
}

#endif
