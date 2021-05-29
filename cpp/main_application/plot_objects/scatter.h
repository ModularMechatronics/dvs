#ifndef SCATTER_2D_H_
#define SCATTER_2D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

class Scatter2D : public PlotObjectBase
{
private:
    Vectord x_vec, y_vec;

    void findMinMax();

public:
    Scatter2D();
    Scatter2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Scatter2D();

    void visualize() const override;
};

Scatter2D::Scatter2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::SCATTER2)
    {
        throw std::runtime_error("Invalid function type for Scatter2D!");
    }

    x_vec.setInternalData(reinterpret_cast<double*>(data_ptr_), num_elements_);
    y_vec.setInternalData(reinterpret_cast<double*>(&(data_ptr_[num_bytes_for_one_vec_])), num_elements_);

    findMinMax();
}

void Scatter2D::findMinMax()
{
    assert(x_vec.isAllocated() && "Vector not allocated when checking min/max!");
    assert(y_vec.isAllocated() && "Vector not allocated when checking min/max!");

    min_vec.x = dvs::min(x_vec);
    min_vec.y = dvs::min(y_vec);

    max_vec.x = dvs::max(x_vec);
    max_vec.y = dvs::max(y_vec);
}

void Scatter2D::visualize() const
{
    setColor(color_);
    setPointSize(point_size_);
    scatter(x_vec, y_vec);
}

Scatter2D::~Scatter2D()
{
    x_vec.setInternalData(nullptr, 0);  // Hack
    y_vec.setInternalData(nullptr, 0);
}

#endif
