#ifndef PLOT3D_H_
#define PLOT3D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

class Plot3D : public PlotObjectBase
{
private:
    size_t num_elements_;
    float line_width_;

    Vectord x_vec, y_vec, z_vec;

    void findMinMax();

public:
    Plot3D();
    Plot3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Plot3D();

    void visualize() const override;
};

Plot3D::Plot3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::PLOT3)
    {
        throw std::runtime_error("Invalid function type for Plot2D!");
    }

    num_dimensions_ = 3;
    num_elements_ = hdr.getObjectFromType(FunctionHeaderObjectType::NUM_ELEMENTS).getAs<uint32_t>();
    const uint64_t num_data_bytes = received_data_->getNumDataBytes();
    if(num_data_bytes == 0)
    {
        throw std::runtime_error("No data bytes!");
    }

    const uint64_t num_bytes_for_one_vec = num_bytes_per_element_ * num_elements_;

    if((num_dimensions_ * num_bytes_for_one_vec) != num_data_bytes)
    {
        throw std::runtime_error("Expected number of bytes does not match the actual number of bytes!");
    }

    uint8_t* const ptr = received_data_->getDataPointer();
    x_vec.setInternalData(reinterpret_cast<double*>(ptr), num_elements_);
    y_vec.setInternalData(reinterpret_cast<double*>(&(ptr[num_bytes_for_one_vec])), num_elements_);
    z_vec.setInternalData(reinterpret_cast<double*>(&(ptr[2 * num_bytes_for_one_vec])), num_elements_);

    findMinMax();
}

void Plot3D::findMinMax()
{
    assert(x_vec.isAllocated() && "Vector x not allocated when checking min/max!");
    assert(y_vec.isAllocated() && "Vector y not allocated when checking min/max!");
    assert(z_vec.isAllocated() && "Vector z not allocated when checking min/max!");

    min_vec.x = dvs::min(x_vec);
    min_vec.y = dvs::min(y_vec);
    min_vec.z = dvs::min(z_vec);

    max_vec.x = dvs::max(x_vec);
    max_vec.y = dvs::max(y_vec);
    max_vec.z = dvs::max(z_vec);
}

void Plot3D::visualize() const
{
    setColor(color_);
    setLinewidth(line_width_);
    plot3(x_vec, y_vec, z_vec);
}

Plot3D::~Plot3D()
{
    x_vec.setInternalData(nullptr, 0);  // Hack
    y_vec.setInternalData(nullptr, 0);
    z_vec.setInternalData(nullptr, 0);
}

#endif
