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

static constexpr size_t kDefaultBufferSize = 500U;

bool isPlotDataFunction(const Function fcn);

struct PropertiesData
{
    PropertiesData() = default;
    PropertiesData(const CommunicationHeader& hdr)
    {
        const Properties props{hdr};

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::ITEM_ID))
        {
            id = hdr.get(CommunicationHeaderObjectType::ITEM_ID).as<internal::ItemId>();
        }
        else
        {
            id = internal::ItemId::UNKNOWN;
        }

        // Flags
        is_persistent = props.hasFlag(PropertyFlag::PERSISTENT);
        interpolate_colormap = props.hasFlag(PropertyFlag::INTERPOLATE_COLORMAP);
        is_updateable = props.hasFlag(PropertyFlag::UPDATABLE);
        dynamic_or_static_usage = is_updateable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

        // Properties
        alpha = props.getPropertyOrValue<Alpha>(255.0f) / 255.0f;
        buffer_size = props.getPropertyOrValue<BufferSize>(kDefaultBufferSize);
        scatter_style = props.getPropertyOrValue<ScatterStyleContainer>(ScatterStyle::CIRCLE);
        line_width = props.getPropertyOrValue<LineWidth>(1.0f);
        point_size = props.getPropertyOrValue<PointSize>(10.0f);

        z_offset = props.getPropertyOrValue<ZOffset>(0.0f);

        if ((has_custom_transform = props.hasProperty(PropertyType::TRANSFORM)))
        {
            custom_transform = props.getProperty<Transform>();
        }

        if ((has_distance_from = props.hasProperty(PropertyType::DISTANCE_FROM)))
        {
            distance_from = props.getProperty<DistanceFrom>();
        }

        if ((has_name = props.hasProperty(PropertyType::NAME)))
        {
            name = props.getProperty<Name>().data;
        }

        if ((has_color = props.hasProperty(PropertyType::COLOR)))
        {
            const Color col = props.getProperty<Color>();
            color.red = static_cast<float>(col.red) / 255.0f;
            color.green = static_cast<float>(col.green) / 255.0f;
            color.blue = static_cast<float>(col.blue) / 255.0f;
        }

        if ((has_color_map = props.hasProperty(PropertyType::COLOR_MAP)))
        {
            color_map = props.getProperty<ColorMapContainer>().data;
            edge_color = RGBTripletf(0.0f, 0.0f, 0.0f);
        }

        if (props.hasProperty(PropertyType::EDGE_COLOR))
        {
            const EdgeColor ec = props.getProperty<EdgeColor>();

            if (ec.use_color)
            {
                edge_color.red = static_cast<float>(ec.red) / 255.0f;
                edge_color.green = static_cast<float>(ec.green) / 255.0f;
                edge_color.blue = static_cast<float>(ec.blue) / 255.0f;
                has_edge_color = true;
            }
            else
            {
                has_edge_color = false;
            }
        }
        else
        {
            edge_color = RGBTripletf(0.0f, 0.0f, 0.0f);
            has_edge_color = true;
        }

        if (props.hasProperty(PropertyType::FACE_COLOR))
        {
            const FaceColor fc = props.getProperty<FaceColor>();

            if (fc.use_color)
            {
                face_color.red = static_cast<float>(fc.red) / 255.0f;
                face_color.green = static_cast<float>(fc.green) / 255.0f;
                face_color.blue = static_cast<float>(fc.blue) / 255.0f;
                has_face_color = true;
            }
            else
            {
                has_face_color = false;
            }
        }

        if ((has_line_style = props.hasProperty(PropertyType::LINE_STYLE)))
        {
            line_style = props.getProperty<LineStyleContainer>().data;
        }
    }

    std::string name;
    bool has_name;

    RGBTripletf color;
    bool has_color;

    RGBTripletf edge_color;
    RGBTripletf face_color;
    size_t buffer_size;

    bool has_distance_from;
    DistanceFrom distance_from;

    bool has_custom_transform;
    Transform custom_transform;

    float z_offset;

    bool has_edge_color;
    bool has_face_color;

    ColorMap color_map;
    bool has_color_map;

    LineStyle line_style;
    bool has_line_style;

    ScatterStyle scatter_style;

    float alpha;
    float line_width;
    float point_size;

    bool is_persistent;
    bool is_updateable;
    GLenum dynamic_or_static_usage;
    bool interpolate_colormap;

    internal::ItemId id;
};

struct ConvertedDataBase
{
    Function function;

    virtual ~ConvertedDataBase() {}
};

struct PlotObjectAttributes
{
    PropertiesData properties_data;

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

    float z_offset;

    PlotObjectAttributes() = delete;
    PlotObjectAttributes(const CommunicationHeader& hdr) : properties_data{hdr}, function{hdr.getFunction()}
    {
        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::DATA_TYPE))
        {
            data_type = hdr.get(CommunicationHeaderObjectType::DATA_TYPE).as<DataType>();
            num_bytes_per_element = dataTypeToNumBytes(data_type);
        }

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::NUM_ELEMENTS))
        {
            num_elements = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();
        }

        num_dimensions = getNumDimensionsFromFunction(function);

        id = properties_data.id;

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

        z_offset = properties_data.z_offset;
    }
};

class PlotObjectBase
{
protected:
    ReceivedData received_data_;

    uint8_t* data_ptr_;

    size_t num_dimensions_;
    size_t num_bytes_per_element_;
    uint32_t num_elements_;
    uint64_t num_bytes_for_one_vec_;

    Function function_;
    DataType data_type_;

    Vec3d min_vec;
    Vec3d max_vec;
    bool min_max_calculated_;
    ShaderCollection shader_collection_;

    // Properties
    Name name_;

    RGBTripletf color_;
    bool has_color_;

    RGBTripletf edge_color_;
    RGBTripletf face_color_;
    size_t buffer_size_;

    bool has_distance_from_;
    DistanceFrom distance_from_;

    bool has_custom_transform_;
    glm::mat4 custom_rotation_;
    glm::mat4 custom_translation_;
    glm::mat4 custom_scale_;

    float z_offset_;

    bool has_edge_color_;
    bool has_face_color_;

    ColorMap color_map_;
    bool has_color_map_;

    LineStyle line_style_;
    bool has_line_style_;

    ScatterStyle scatter_style_;

    float alpha_;
    float line_width_;
    float point_size_;

    bool is_persistent_;
    bool is_updateable_;
    GLenum dynamic_or_static_usage_;
    bool interpolate_colormap_;

    bool has_name_;

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
    bool affectsColormapMinMax() const
    {
        return has_color_map_;
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

    virtual LegendProperties getLegendProperties() const
    {
        LegendProperties lp;
        lp.name = name_.data;

        return lp;
    }

    virtual void updateWithNewData(ReceivedData& received_data,
                                   const CommunicationHeader& hdr,
                                   const std::unique_ptr<const ConvertedDataBase>& converted_data,
                                   const Properties& props);

    bool hasName()
    {
        return has_name_;
    }

    virtual void modifyShader();

    void updateProperties(const Properties& props);
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_
