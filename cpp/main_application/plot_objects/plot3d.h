#ifndef PLOT3D_H_
#define PLOT3D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base.h"

class Plot3D : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;

    void findMinMax() override;

public:
    Plot3D();
    Plot3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Plot3D();

    void render() override;
};

Plot3D::Plot3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::PLOT3)
    {
        throw std::runtime_error("Invalid function type for Plot3D!");
    }

    points_ptr_ =
        convertData3DOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*float* color_data = new float[num_elements_ * 3];

    for(size_t k = 0; k < (num_elements_ * 3); k++)
    {
        color_data[k] = static_cast<float>(k) / static_cast<float>(num_elements_ * 3);
    }

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3, color_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );*/
}

void Plot3D::findMinMax()
{
    std::tie<Vec3Dd, Vec3Dd>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Plot3D::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINE_STRIP, 0, num_elements_);
    glBindVertexArray(0);
}

Plot3D::~Plot3D()
{
    delete[] points_ptr_;
}

#endif
