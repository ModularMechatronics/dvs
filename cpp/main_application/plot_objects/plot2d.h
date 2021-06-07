#ifndef PLOT2D_H_
#define PLOT2D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

class Plot2D : public PlotObjectBase
{
private:
    Vectord x_vec, y_vec;

    void findMinMax() override;

public:
    Plot2D();
    Plot2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Plot2D();

    void visualize() override;
};

Plot2D::Plot2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr) : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::PLOT2)
    {
        throw std::runtime_error("Invalid function type for Plot2D!");
    }

    x_vec.setInternalData(reinterpret_cast<double*>(data_ptr_), num_elements_);
    y_vec.setInternalData(reinterpret_cast<double*>(&(data_ptr_[num_bytes_for_one_vec_])), num_elements_);
}

void Plot2D::findMinMax()
{
    Vec2Dd min_vec_2d, max_vec_2d;
    std::tie<Vec2Dd, Vec2Dd>(min_vec_2d, max_vec_2d) = findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec.x = min_vec_2d.x;
    min_vec.y = min_vec_2d.y;
    min_vec.z = -1.0;

    max_vec.x = max_vec_2d.x;
    max_vec.y = max_vec_2d.y;
    max_vec.z = 1.0;
}

void Plot2D::visualize()
{
    setColor(color_);
    setLinewidth(line_width_);
    plot(x_vec, y_vec);
}

Plot2D::~Plot2D()
{
    x_vec.setInternalData(nullptr, 0);  // Hack
    y_vec.setInternalData(nullptr, 0);
}

#endif
