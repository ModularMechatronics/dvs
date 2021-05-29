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

    num_dimensions_ = 3;
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
    dims_ = hdr.getObjectFromType(FunctionHeaderObjectType::DIMENSION_2D).getAs<internal::Dimension2D>();

    color_map_ = color_maps::rainbowf;

    uint8_t* const ptr = received_data_->getDataPointer();
    x_mat.setInternalData(reinterpret_cast<double*>(ptr), dims_.rows, dims_.cols);
    y_mat.setInternalData(reinterpret_cast<double*>(&(ptr[num_bytes_for_one_vec])), dims_.rows, dims_.cols);
    z_mat.setInternalData(reinterpret_cast<double*>(&(ptr[2 * num_bytes_for_one_vec])), dims_.rows, dims_.cols);
    face_color_set_ = true;

    findMinMax();
    /*// num_elements is actual number of elements, not number of bytes
    ASSERT(rx_list.getObjectData<FunctionRx>() == Function::SURF);
    ASSERT(rx_list.getObjectData<NumBuffersRequiredRx>() == 3);
    ASSERT(rx_list.getObjectData<DataStructureRx>() == DataStructure::MATRIX);

    num_elements_ = rx_list.getObjectData<NumElementsRx>();
    face_color_set_ = false;
    color_map_ = arl::color_maps::jetf;

    face_color_ = RGBTripletf(0.1, 0.2, 0.1);
    edge_color_ = RGBTripletf(0.0, 0.0, 0.0);

    ASSERT(rx_list.hasKey(Command::DIMENSION_2D)) << "Missing dimensions message!";

    dim_ = rx_list.getObjectData<Dimension2dRx>();

    if (rx_list.hasKey(Command::FACE_COLOR))
    {
        FaceColor c = rx_list.getObjectData<FaceColorRx>();
        face_color_.red = c.red;
        face_color_.green = c.green;
        face_color_.blue = c.blue;
        face_color_set_ = true;
    }
    if (rx_list.hasKey(Command::EDGE_COLOR))
    {
        EdgeColor c = rx_list.getObjectData<EdgeColorRx>();
        edge_color_.red = c.red;
        edge_color_.green = c.green;
        edge_color_.blue = c.blue;
    }
    if (rx_list.hasKey(Command::COLOR_MAP))
    {
        face_color_set_ = false;
        ColorMap cm = rx_list.getObjectData<ColorMapRx>();
        switch (cm.data)
        {
            case ColorMap::RAINBOW:
                color_map_ = arl::color_maps::rainbowf;
                break;
            case ColorMap::MAGMA:
                color_map_ = arl::color_maps::magmaf;
                break;
            case ColorMap::VIRIDIS:
                color_map_ = arl::color_maps::viridisf;
                break;
            case ColorMap::JET:
                color_map_ = arl::color_maps::jetf;
                break;
            default:
                color_map_ = arl::color_maps::jetf;
                break;
        }
    }

    line_width_ =
        rx_list.hasKey(Command::LINEWIDTH) ? rx_list.getObjectData<LinewidthRx>().data : 1.0f;

    x_mat.setInternalData(reinterpret_cast<double*>(data_[0]), dim_.rows, dim_.cols);
    y_mat.setInternalData(reinterpret_cast<double*>(data_[1]), dim_.rows, dim_.cols);
    z_mat.setInternalData(reinterpret_cast<double*>(data_[2]), dim_.rows, dim_.cols);

    findMinMax();*/
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
