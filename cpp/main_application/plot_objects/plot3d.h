#ifndef PLOT3D_H_
#define PLOT3D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "plot_objects/plot_object_base.h"

class Plot3D : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;
    GLuint buffer_idx_;

    void findMinMax() override;

public:
    Plot3D();
    Plot3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Plot3D();

    void visualize() override;
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
}

void Plot3D::findMinMax()
{
    std::tie<Vec3Dd, Vec3Dd>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Plot3D::visualize()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3, points_ptr_, GL_STATIC_DRAW);
    }
    setColor(color_);
    setLinewidth(line_width_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glDrawArrays(GL_LINE_STRIP, 0, num_elements_);
    glDisableVertexAttribArray(0);
}

Plot3D::~Plot3D()
{
    delete[] points_ptr_;
}

#endif
