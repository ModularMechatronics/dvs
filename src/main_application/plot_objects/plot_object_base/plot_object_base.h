#ifndef MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "axes/legend_properties.h"
#include "color_picker.h"
#include "communication/received_data.h"
#include "dvs/enumerations.h"
#include "dvs/math/math.h"
#include "dvs/plot_properties.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/utils.h"
#include "properties.h"
#include "shader.h"

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

bool isPlotDataFunction(const Function fcn);

struct ConvertedDataBase
{
    Function function;

    virtual ~ConvertedDataBase() {}
};

struct PlotObjectAttributes
{
    Function function;
    DataType data_type;

    size_t num_bytes_per_element;
    size_t num_elements;
    size_t num_dimensions;

    ItemId id;

    bool has_color;

    uint64_t num_bytes_for_one_vec;

    Dimension2D dims;

    uint32_t num_vertices;
    uint32_t num_indices;

    uint8_t num_channels;

    uint32_t num_objects;

    PlotObjectAttributes() = delete;
    PlotObjectAttributes(const CommunicationHeader& hdr)
    {
        function = hdr.getFunction();
        data_type = hdr.get(CommunicationHeaderObjectType::DATA_TYPE).as<DataType>();

        num_bytes_per_element = dataTypeToNumBytes(data_type);
        num_elements = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();

        num_dimensions = getNumDimensionsFromFunction(function);

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::ITEM_ID))
        {
            id = hdr.get(CommunicationHeaderObjectType::ITEM_ID).as<internal::ItemId>();
        }
        else
        {
            id = internal::ItemId::UNKNOWN;
        }

        has_color = hdr.hasObjectWithType(CommunicationHeaderObjectType::HAS_COLOR);
        num_bytes_for_one_vec = num_bytes_per_element * num_elements;

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::DIMENSION_2D))
        {
            dims = hdr.get(CommunicationHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();
        }

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::NUM_VERTICES))
        {
            num_vertices = hdr.get(CommunicationHeaderObjectType::NUM_VERTICES).as<uint32_t>();
        }

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::NUM_INDICES))
        {
            num_indices = hdr.get(CommunicationHeaderObjectType::NUM_INDICES).as<uint32_t>();
        }

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::NUM_CHANNELS))
        {
            num_channels = hdr.get(CommunicationHeaderObjectType::NUM_CHANNELS).as<uint8_t>();
        }

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::NUM_OBJECTS))
        {
            num_objects = hdr.get(CommunicationHeaderObjectType::NUM_OBJECTS).as<uint32_t>();
        }
    }
};

class PlotObjectBase
{
protected:
    static constexpr size_t kDefaultBufferSize = 500U;
    ReceivedData received_data_;
    size_t num_dimensions_;
    size_t num_bytes_per_element_;
    uint32_t num_elements_;
    uint64_t num_bytes_for_one_vec_;
    uint8_t* data_ptr_;
    bool has_color_;
    bool has_distance_from_;
    DistanceFrom distance_from_;
    bool has_custom_transform_;

    float z_offset_;

    glm::mat4 custom_rotation_;
    glm::mat4 custom_translation_;
    glm::mat4 custom_scale_;

    Function type_;
    DataType data_type_;

    Vec3d min_vec;
    Vec3d max_vec;

    // Properties
    Name name_;
    RGBTripletf color_;
    RGBTripletf edge_color_;
    RGBTripletf face_color_;
    size_t buffer_size_;

    bool has_edge_color_;
    bool has_face_color_;

    ColorMap color_map_;
    LineStyle line_style_;
    int is_dashed_;
    ScatterStyle scatter_style_;
    float alpha_;
    float line_width_;
    float point_size_;
    bool is_persistent_;
    bool is_updateable_;
    GLenum usage_;
    bool interpolate_colormap_;
    bool min_max_calculated_;
    bool has_name_;
    bool color_map_set_;
    ShaderCollection shader_collection_;
    internal::ItemId id_;

    void assignProperties(const Properties& props, ColorPicker& color_picker);
    virtual void findMinMax() = 0;

    void postInitialize(ReceivedData& received_data, const CommunicationHeader& hdr, const Properties& props);
    void throwIfNotUpdateable() const;

public:
    size_t getNumDimensions() const;
    virtual ~PlotObjectBase();
    PlotObjectBase();
    PlotObjectBase(ReceivedData& received_data,
                   const CommunicationHeader& hdr,
                   const Properties& props,
                   const ShaderCollection shader_collection,
                   ColorPicker& color_picker);
    virtual void render() = 0;
    void preRender(const Shader shader_to_use);
    virtual bool affectsColormapMinMax() const
    {
        return false;  // TODO: Should just return "color_map_set_"
    }

    void setTransform(const MatrixFixed<double, 3, 3>& rotation,
                      const Vec3<double>& translation,
                      const MatrixFixed<double, 3, 3>& scale);

    internal::ItemId getId() const
    {
        return id_;
    }

    std::pair<Vec3d, Vec3d> getMinMaxVectors();

    bool isPersistent() const;
    std::string getName() const;
    std::string_view getNameStringView() const;

    virtual LegendProperties getLegendProperties() const
    {
        LegendProperties lp;
        lp.name = name_.data;

        return lp;
    }

    virtual void updateWithNewData(ReceivedData& received_data,
                                   const CommunicationHeader& hdr,
                                   const Properties& props);

    Function getType() const
    {
        return type_;
    }

    RGBTripletf getColor() const
    {
        return color_;
    }

    bool hasName()
    {
        return has_name_;
    }

    virtual void modifyShader();

    void setProperties(const Properties& props);
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_
