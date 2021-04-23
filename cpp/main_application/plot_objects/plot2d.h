#ifndef PLOT2D_H_
#define PLOT2D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

using namespace plot_tool;

class Plot2D : public PlotObjectBase
{
private:
    uint32_t num_elements_;

    Vectord x_vec, y_vec;

    void findMinMax();

public:
    Plot2D();
    Plot2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~Plot2D();

    void visualize() const override;
};

Plot2D::Plot2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr) : PlotObjectBase(std::move(received_data), hdr)
{
    const DataStructure data_structure = hdr.getObjectFromType(FunctionHeaderObjectType::DATA_STRUCTURE).getAs<DataStructure>();
    if(data_structure != DataStructure::VECTOR)
    {
        throw std::runtime_error("Invalid data structure for function plot2!");
    }

    num_elements_ = hdr.getObjectFromType(FunctionHeaderObjectType::NUM_ELEMENTS).getAs<uint32_t>();
    const uint64_t num_data_bytes = received_data_->getNumDataBytes();
    if(num_data_bytes == 0)
    {
        throw std::runtime_error("No data bytes!");
    }

    const uint64_t num_bytes_for_one_vec = sizeof(double) * num_elements_;

    uint8_t* ptr = received_data_->getDataPointer();
    x_vec.setInternalData(reinterpret_cast<double*>(ptr), num_elements_);
    y_vec.setInternalData(reinterpret_cast<double*>(&(ptr[num_bytes_for_one_vec])), num_elements_);

    // line_width_ =
    //     hdr.hasType(FunctionHeaderObjectType::LINE_WIDTH); // ? rx_list.getObjectData<LinewidthRx>().data : 1.0f


    // if()
    /*ASSERT(rx_list.getObjectData<FunctionRx>() == Function::PLOT2);
    ASSERT(rx_list.getObjectData<NumBuffersRequiredRx>() == 2);
    ASSERT(rx_list.getObjectData<DataStructureRx>() == DataStructure::VECTOR);

    num_elements_ = rx_list.getObjectData<NumElementsRx>();

    x_vec.setInternalData(reinterpret_cast<double*>(data_[0]), num_elements_);
    y_vec.setInternalData(reinterpret_cast<double*>(data_[1]), num_elements_);

    line_width_ =
        rx_list.hasKey(Command::LINEWIDTH) ? rx_list.getObjectData<LinewidthRx>().data : 1.0f;*/

    // findMinMax();
}

void Plot2D::findMinMax()
{
    assert(x_vec.isAllocated() && "Vector not allocated when checking min/max!");
    assert(y_vec.isAllocated() && "Vector not allocated when checking min/max!");

    min_vec.x = dvs::min(x_vec);
    min_vec.y = dvs::min(y_vec);

    max_vec.x = dvs::max(x_vec);
    max_vec.y = dvs::max(y_vec);
}

void Plot2D::visualize() const
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
