#ifndef SURF_H_
#define SURF_H_

// #include <arl/utilities/color_map.h>
#include "math/math.h"

#include <string>
#include <vector>

#include "main_application/misc/color_map.h"
#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

class Surf : public PlotObjectBase
{
private:
    Dimension2D dims_;
    RGBColorMap<float> color_map_;
    GLuint buffer_idx_;

    Matrix<float> x_mat, y_mat, z_mat;
    float* points_ptr_;
    float* lines_ptr_;

    bool face_color_set_;

    void findMinMax() override;

public:
    Surf();
    Surf(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Surf();

    void visualize() override;
};

Surf::Surf(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::SURF)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    dims_ = hdr.getObjectFromType(FunctionHeaderObjectType::DIMENSION_2D).getAs<internal::Dimension2D>();

    Matrix<int8_t> x, y, z;

    x_mat.resize(dims_.rows, dims_.cols);
    y_mat.resize(dims_.rows, dims_.cols);
    z_mat.resize(dims_.rows, dims_.cols);

    x.setInternalData(reinterpret_cast<int8_t*>(data_ptr_), dims_.rows, dims_.cols);
    y.setInternalData(reinterpret_cast<int8_t*>(&(data_ptr_[num_bytes_for_one_vec_])), dims_.rows, dims_.cols);
    z.setInternalData(reinterpret_cast<int8_t*>(&(data_ptr_[2 * num_bytes_for_one_vec_])), dims_.rows, dims_.cols);

    for(size_t r = 0; r < dims_.rows; r++)
    {
        for(size_t c = 0; c < dims_.cols; c++)
        {
            x_mat(r, c) = x(r, c);
            y_mat(r, c) = y(r, c);
            z_mat(r, c) = z(r, c);
        }
    }

    points_ptr_ = new float[(dims_.rows - 1) * (dims_.cols - 1) * 4 * 3];
    size_t idx = 0;
    for(size_t r = 0; r < (dims_.rows - 1); r++)
    {
        for(size_t c = 0; c < (dims_.cols - 1); c++)
        {
            const size_t idx0_x = idx;
            const size_t idx0_y = idx + 1;
            const size_t idx0_z = idx + 2;

            const size_t idx1_x = idx + 3;
            const size_t idx1_y = idx + 4;
            const size_t idx1_z = idx + 5;

            const size_t idx2_x = idx + 6;
            const size_t idx2_y = idx + 7;
            const size_t idx2_z = idx + 8;

            const size_t idx3_x = idx + 9;
            const size_t idx3_y = idx + 10;
            const size_t idx3_z = idx + 11;
            idx = idx + 12;

            points_ptr_[idx0_x] = x_mat(r, c);
            points_ptr_[idx1_x] = x_mat(r + 1, c);
            points_ptr_[idx2_x] = x_mat(r + 1, c + 1);
            points_ptr_[idx3_x] = x_mat(r, c + 1);

            points_ptr_[idx0_y] = y_mat(r, c);
            points_ptr_[idx1_y] = y_mat(r + 1, c);
            points_ptr_[idx2_y] = y_mat(r + 1, c + 1);
            points_ptr_[idx3_y] = y_mat(r, c + 1);

            points_ptr_[idx0_z] = z_mat(r, c);
            points_ptr_[idx1_z] = z_mat(r + 1, c);
            points_ptr_[idx2_z] = z_mat(r + 1, c + 1);
            points_ptr_[idx3_z] = z_mat(r, c + 1);
        }
    }

    x.setInternalData(nullptr, 0, 0);
    y.setInternalData(nullptr, 0, 0);
    z.setInternalData(nullptr, 0, 0);

    findMinMax();
}

void Surf::findMinMax()
{
    std::tie<Vec3Dd, Vec3Dd>(min_vec, max_vec) = findMinMaxFromThreeMatrices(data_ptr_, dims_.rows, dims_.cols, num_bytes_for_one_vec_, data_type_);
}

void Surf::visualize()
{
    if(!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * (dims_.rows - 1) * (dims_.cols - 1), points_ptr_, GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    setLinewidth(line_width_);

    glPolygonOffset(1, -1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    setColor(face_color_);
    glDrawArrays(GL_QUADS, 0, 4 * (dims_.rows - 1) * (dims_.cols - 1));
    setColor(edge_color_);
    
    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_QUADS, 0, 4 * (dims_.rows - 1) * (dims_.cols - 1));
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisableVertexAttribArray(0);
}

Surf::~Surf()
{
    delete[] points_ptr_;
}

#endif
