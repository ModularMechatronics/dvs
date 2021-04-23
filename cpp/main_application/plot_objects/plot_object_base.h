#ifndef PLOT_OBJECT_BASE_H_
#define PLOT_OBJECT_BASE_H_

#include "math/math.h"

#include <string>
#include <vector>
#include <memory>
#include <map>

#include "received_data.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_functions/plot_functions.h"
#include "enumerations.h"
#include "plot_attributes.h"

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

class PlotObjectBase
{
protected:
    std::unique_ptr<const ReceivedData> received_data_;
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

    // Properties
    Name name_;
    RGBTripletf color_;
    RGBTripletf edge_color_;
    RGBTripletf face_color_;
    ColorMapType color_map_;
    LineStyle line_style_;
    float alpha_;
    float line_width_;
    float point_size_;

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

    const Properties props = hdr.getProperties();

    data_type_ = hdr.getObjectFromType(FunctionHeaderObjectType::DATA_TYPE).getAs<DataType>();
    /*size_t num_bytes_;
    size_t num_buffers_required_;
    size_t num_bytes_per_element_;

    Function type_;                 // Plot, surf, stem etc.
    DataStructure data_structure_;  // vector, matrix, image etc.
    Function function_;*/

    is_persistent_ = props.hasProperty(PropertyType::PERSISTENT);

    if(props.hasProperty(PropertyType::NAME))
    {
        name_ = props.getProperty<Name>();
    }
    else
    {
        name_ = Name("<undef-name>");
    }

    if(props.hasProperty(PropertyType::COLOR))
    {
        const Color col = props.getProperty<Color>();
        color_.red = static_cast<float>(col.red) / 255.0f;
        color_.green = static_cast<float>(col.green) / 255.0f;
        color_.blue = static_cast<float>(col.blue) / 255.0f;
    }
    else
    {
        color_ = RGBTripletf(0.1, 0.2, 0.1);
    }

    if(props.hasProperty(PropertyType::ALPHA))
    {
        const Alpha alp = props.getProperty<Alpha>();
        alpha_ = static_cast<float>(alp.data) / 255.0f;
    }
    else
    {
        alpha_ = 1.0f;
    }

    if(props.hasProperty(PropertyType::EDGE_COLOR))
    {
        const EdgeColor ec = props.getProperty<EdgeColor>();
        edge_color_.red = static_cast<float>(ec.red) / 255.0f;
        edge_color_.green = static_cast<float>(ec.green) / 255.0f;
        edge_color_.blue = static_cast<float>(ec.blue) / 255.0f;
    }
    else
    {
        edge_color_ = RGBTripletf(1.0f, 1.0f, 1.0f);
    }

    if(props.hasProperty(PropertyType::FACE_COLOR))
    {
        const FaceColor fc = props.getProperty<FaceColor>();
        face_color_.red = static_cast<float>(fc.red) / 255.0f;
        face_color_.green = static_cast<float>(fc.green) / 255.0f;
        face_color_.blue = static_cast<float>(fc.blue) / 255.0f;
    }
    else
    {
        face_color_ = RGBTripletf(0.1f, 0.2f, 0.3f);
    }

    if(props.hasProperty(PropertyType::COLOR_MAP))
    {
        color_map_ = props.getProperty<ColorMap>().data;
    }
    else
    {
        color_map_ = ColorMapType::JET;
    }

    if(props.hasProperty(PropertyType::LINE_WIDTH))
    {
        const LineWidth lw = props.getProperty<LineWidth>();
        line_width_ = static_cast<float>(lw.data) / 100.0f;
    }
    else
    {
        line_width_ = 1.0f;
    }

    if(props.hasProperty(PropertyType::POINT_SIZE))
    {
        const PointSize ps = props.getProperty<PointSize>();
        point_size_ = static_cast<float>(ps.data) / 100.0f;
    }
    else
    {
        point_size_ = 1.0f;
    }

    if(props.hasProperty(PropertyType::LINE_STYLE))
    {
        line_style_ = props.getProperty<LineStyle>();
    }
    else
    {
        line_style_ = LineStyle("");
    }

    std::map<ColorMapType, std::string> cms = {{ColorMapType::JET, "JET"},
                                               {ColorMapType::RAINBOW, "RAINBOW"},
                                               {ColorMapType::MAGMA, "MAGMA"},
                                               {ColorMapType::VIRIDIS, "VIRIDIS"}};

    std::cout << "Color map: " << cms[color_map_] << std::endl;
    std::cout << "Color: " << color_.red * 255.0f << ", " << color_.green * 255.0f << ", " << color_.blue * 255.0f << std::endl;
    std::cout << "EdgeColor: " << edge_color_.red * 255.0f << ", " << edge_color_.green * 255.0f << ", " << edge_color_.blue * 255.0f << std::endl;
    std::cout << "FaceColor: " << face_color_.red * 255.0f << ", " << face_color_.green * 255.0f << ", " << face_color_.blue * 255.0f << std::endl;
    std::cout << "Name: " << name_.data << std::endl;
    std::cout << "LineStyle: " << line_style_.data << std::endl;
    std::cout << "Alpha: " << alpha_ * 255.0f << std::endl;
    std::cout << "Point size: " << point_size_ * 100.0f << std::endl;
    std::cout << "Line width: " << line_width_ * 100.0f << std::endl;
    /*
    float alpha_;
    float line_width_;
    float point_size_;*/
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
    /*for (size_t k = 0; k < data_.size(); k++)
    {
        delete[] data_[k];
    }*/
}

#endif
