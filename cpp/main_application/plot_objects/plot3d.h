#ifndef PLOT3D_H_
#define PLOT3D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "main_application/plot_objects/utils.h"

class Plot3D : public PlotObjectBase
{
private:
    Vector<uint32_t> x_vec, y_vec, z_vec;

    uint8_t* points_ptr_;
    bool has_run_;
    GLuint buffer_idx_;

    void findMinMax();

public:
    Plot3D();
    Plot3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Plot3D();

    void visualize() override;
};

Plot3D::Plot3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::PLOT3)
    {
        throw std::runtime_error("Invalid function type for Plot3D!");
    }

    x_vec.setInternalData(reinterpret_cast<uint32_t*>(data_ptr_), num_elements_);
    y_vec.setInternalData(reinterpret_cast<uint32_t*>(&(data_ptr_[num_bytes_for_one_vec_])), num_elements_);
    z_vec.setInternalData(reinterpret_cast<uint32_t*>(&(data_ptr_[2 * num_bytes_for_one_vec_])), num_elements_);

    std::cout << x_vec << std::endl;
    std::cout << y_vec << std::endl;
    std::cout << z_vec << std::endl;

    x_vec.setInternalData(nullptr, 0);
    y_vec.setInternalData(nullptr, 0);
    z_vec.setInternalData(nullptr, 0);

    has_run_ = false;

    points_ptr_ = new uint8_t[num_elements_ * num_bytes_per_element_ * 3];

    std::cout << "Num bytes per elements: " << num_bytes_per_element_ << std::endl;

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

    findMinMax();
}

void Plot3D::findMinMax()
{
    /*assert(x_vec.isAllocated() && "Vector x not allocated when checking min/max!");
    assert(y_vec.isAllocated() && "Vector y not allocated when checking min/max!");
    assert(z_vec.isAllocated() && "Vector z not allocated when checking min/max!");

    min_vec.x = dvs::min(x_vec);
    min_vec.y = dvs::min(y_vec);
    min_vec.z = dvs::min(z_vec);

    max_vec.x = dvs::max(x_vec);
    max_vec.y = dvs::max(y_vec);
    max_vec.z = dvs::max(z_vec);*/

    min_vec.x = -1.0;
    min_vec.y = -1.0;
    min_vec.z = -1.0;

    max_vec.x = 1.0;
    max_vec.y = 1.0;
    max_vec.z = 1.0;
}

void Plot3D::visualize()
{
    if(!has_run_)
    {
        has_run_ = true;
        glGenBuffers(1, &buffer_idx_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
        glBufferData(GL_ARRAY_BUFFER, num_bytes_per_element_ * num_elements_ * 3, points_ptr_, GL_STATIC_DRAW);
    }
    setColor(color_);
    setLinewidth(line_width_);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buffer_idx_);
    glVertexAttribPointer(0, 3, dataTypeToGLInt(data_type_), GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_LINE_STRIP, 0, num_elements_);
    glDisableVertexAttribArray(0);
}

Plot3D::~Plot3D()
{
    x_vec.setInternalData(nullptr, 0);  // Hack
    y_vec.setInternalData(nullptr, 0);
    z_vec.setInternalData(nullptr, 0);
}

#endif
