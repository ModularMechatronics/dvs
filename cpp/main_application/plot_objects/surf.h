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

using namespace plot_tool;

class Surf : public PlotObjectBase
{
private:
    Dimension2D dims_;
    RGBColorMap<float> color_map_;

    Matrixd x_mat, y_mat, z_mat;

    bool face_color_set_;

    void findMinMax();

public:
    Surf();
    Surf(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Surf();

    void visualize() const override;
};

Surf::Surf(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if(type_ != Function::SURF)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    dims_ = hdr.getObjectFromType(FunctionHeaderObjectType::DIMENSION_2D).getAs<internal::Dimension2D>();

    color_map_ = color_maps::rainbowf;

    x_mat.setInternalData(reinterpret_cast<double*>(data_ptr_), dims_.rows, dims_.cols);
    y_mat.setInternalData(reinterpret_cast<double*>(&(data_ptr_[num_bytes_for_one_vec_])), dims_.rows, dims_.cols);
    z_mat.setInternalData(reinterpret_cast<double*>(&(data_ptr_[2 * num_bytes_for_one_vec_])), dims_.rows, dims_.cols);
    face_color_set_ = true;

    findMinMax();
}

void Surf::findMinMax()
{
    assert(x_mat.isAllocated() && "Matrix not allocated when checking min/max!");
    assert(y_mat.isAllocated() && "Matrix not allocated when checking min/max!");
    assert(z_mat.isAllocated() && "Matrix not allocated when checking min/max!");

    min_vec.x = dvs::min(x_mat);
    min_vec.y = dvs::min(y_mat);
    min_vec.z = dvs::min(z_mat);

    max_vec.x = dvs::max(x_mat);
    max_vec.y = dvs::max(y_mat);
    max_vec.z = dvs::max(z_mat);
}

void Surf::visualize() const
{
    if (face_color_set_)
    {
        setColor(face_color_);
        surf(x_mat, y_mat, z_mat);
    }
    else
    {
        surfInternal(x_mat, y_mat, z_mat, {min_vec.y, max_vec.y}, color_map_);
    }

    setColor(edge_color_);
    setLinewidth(line_width_);
    drawGrid3D(x_mat, y_mat, z_mat);
}

Surf::~Surf()
{
    x_mat.setInternalData(nullptr, 0, 0);  // Hack
    y_mat.setInternalData(nullptr, 0, 0);
    z_mat.setInternalData(nullptr, 0, 0);
}

#endif
