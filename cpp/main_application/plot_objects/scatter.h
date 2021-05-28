#ifndef SCATTER_2D_H_
#define SCATTER_2D_H_

#include "math/math.h"

#include <string>
#include <vector>

#include "plot_objects/plot_object_base.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"

using namespace plot_tool;

class Scatter2D : public PlotObjectBase
{
private:
    float point_size_;

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
        throw std::runtime_error("Invalid function type for Plot2D!");
    }

    num_dimensions_ = 2;
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

    findMinMax();
    /*// num_elements is actual number of elements, not number of bytes
    ASSERT(rx_list.getObjectData<FunctionRx>() == Function::SCATTER2);
    ASSERT(rx_list.getObjectData<NumBuffersRequiredRx>() == 2);
    ASSERT(rx_list.getObjectData<DataStructureRx>() == DataStructure::VECTOR);

    num_elements_ = rx_list.getObjectData<NumElementsRx>();
    point_size_ =
        rx_list.hasKey(Command::POINT_SIZE) ? rx_list.getObjectData<PointSizeRx>().data : 1.0f;

    x_vec.setInternalData(reinterpret_cast<double*>(data_[0]), num_elements_);
    y_vec.setInternalData(reinterpret_cast<double*>(data_[1]), num_elements_);

    findMinMax();*/
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
