#ifndef SCATTER_3D_H_
#define SCATTER_3D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "main_application/plot_objects/utils.h"

class Scatter3D : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;
    GLuint buffer_idx_;

    void findMinMax() override;

public:
    Scatter3D();
    Scatter3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Scatter3D();

    void visualize() override;
};

Scatter3D::Scatter3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::SCATTER3)
    {
        throw std::runtime_error("Invalid function type for Scatter3D!");
    }

    points_ptr_ = new uint8_t[num_elements_ * num_bytes_per_element_ * 3];

    size_t idx_x = 0;
    size_t idx_y = num_bytes_per_element_;
    size_t idx_z = 2 * num_bytes_per_element_;

    for(size_t k = 0; k < num_elements_; k++)
    {
        const size_t idx_0 = k * num_bytes_per_element_;
        const size_t idx_1 = num_bytes_for_one_vec_ + k * num_bytes_per_element_;;
        const size_t idx_2 = num_bytes_for_one_vec_ * 2 + k * num_bytes_per_element_;
        const uint8_t* const tmp_ptr_0 = &(data_ptr_[idx_0]);
        const uint8_t* const tmp_ptr_1 = &(data_ptr_[idx_1]);
        const uint8_t* const tmp_ptr_2 = &(data_ptr_[idx_2]);

        for(size_t i = 0; i < num_bytes_per_element_; i++)
        {
            points_ptr_[idx_x + i] = tmp_ptr_0[i];
            points_ptr_[idx_y + i] = tmp_ptr_1[i];
            points_ptr_[idx_z + i] = tmp_ptr_2[i];
        }
        idx_x += num_bytes_per_element_ * 3;
        idx_y += num_bytes_per_element_ * 3;
        idx_z += num_bytes_per_element_ * 3;
    }
}

void Scatter3D::findMinMax()
{
    std::tie<Vec3Dd, Vec3Dd>(min_vec, max_vec) = findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Scatter3D::visualize()
{
    if(!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, num_bytes_per_element_ * num_elements_ * 3, points_ptr_, GL_STATIC_DRAW);
    }
    setColor(color_);
    setPointSize(point_size_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_POINTS, 0, num_elements_);
    glDisableVertexAttribArray(0);
}

Scatter3D::~Scatter3D()
{
    delete[] points_ptr_;
}

#endif
