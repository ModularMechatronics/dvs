#ifndef PLOT_OBJECT_BASE_H_
#define PLOT_OBJECT_BASE_H_

#include "math/math.h"

#include <string>
#include <vector>
#include <memory>

#include "received_data.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "enumerations.h"
#include "plot_attributes.h"

using namespace dvs;
using namespace dvs::internal;


class PlotObjectBase
{
protected:
    std::unique_ptr<const ReceivedData> received_data_;
    std::vector<char*> data_;
    size_t num_bytes_;
    size_t num_buffers_required_;
    size_t num_bytes_per_element_;

    Function type_;                 // Plot, surf, stem etc.
    DataType data_type_;            // float, int, double etc.
    DataStructure data_structure_;  // vector, matrix, image etc.
    Function function_;
    bool is_persistent_;

    Vec3Dd min_vec;
    Vec3Dd max_vec;

    properties::Name name_;

    RGBTripletf color_;

public:
    size_t getNumDimensions() const;
    virtual ~PlotObjectBase();
    PlotObjectBase();
    // PlotObjectBase(const plot_tool::RxList& rx_list, const std::vector<char*> data_vec);
    PlotObjectBase(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    virtual void visualize() const = 0;
    std::pair<Vec3Dd, Vec3Dd> getMinMaxVectors() const;
    bool isPersistent() const;
    properties::Name getName() const;
};

bool PlotObjectBase::isPersistent() const
{
    return is_persistent_;
}

properties::Name PlotObjectBase::getName() const
{
    return name_;
}

std::pair<Vec3Dd, Vec3Dd> PlotObjectBase::getMinMaxVectors() const
{
    return std::pair<Vec3Dd, Vec3Dd>(min_vec, max_vec);
}

size_t PlotObjectBase::getNumDimensions() const
{
    return num_buffers_required_;
}

PlotObjectBase::PlotObjectBase() {}

PlotObjectBase::PlotObjectBase(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
{
    received_data_ = std::move(received_data);
}

/*PlotObjectBase::PlotObjectBase(const plot_tool::RxList& rx_list, const std::vector<char*> data_vec)
{
    type_ = rx_list.getObjectData<FunctionRx>();
    num_bytes_ = rx_list.getObjectData<NumBytesRx>();
    num_buffers_required_ = rx_list.getObjectData<NumBuffersRequiredRx>();
    num_bytes_per_element_ = rx_list.getObjectData<BytesPerElementRx>();
    data_type_ = rx_list.getObjectData<DataTypeRx>();

    for (size_t k = 0; k < num_buffers_required_; k++)
    {
        char* new_data = new char[num_bytes_];
        char* incoming_data = data_vec[k];

        for (size_t i = 0; i < num_bytes_; i++)
        {
            new_data[i] = incoming_data[i];
        }
        data_.push_back(new_data);
    }

    is_persistent_ = rx_list.hasKey(Command::PERSISTENT) ? true : false;

    name_ = rx_list.hasKey(Command::NAME) ? rx_list.getObjectData<NameRx>() : Name("");

    color_ = RGBTripletf(0.1, 0.2, 0.1);

    if (rx_list.hasKey(Command::COLOR))
    {
        Color c = rx_list.getObjectData<ColorRx>();
        color_.red = c.red;
        color_.green = c.green;
        color_.blue = c.blue;
    }
}*/

PlotObjectBase::~PlotObjectBase()
{
    for (size_t k = 0; k < data_.size(); k++)
    {
        delete[] data_[k];
    }
}

#endif
