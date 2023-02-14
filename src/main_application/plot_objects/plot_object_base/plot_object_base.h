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
#include "opengl_low_level/opengl_header.h"
#include "plot_objects/utils.h"
#include "properties.h"
#include "shader.h"

using namespace dvs;
using namespace dvs::internal;
using namespace dvs::properties;

constexpr size_t kDefaultBufferSize = 500U;
constexpr char* const kDefaultName = "";
constexpr RGBTripletf kDefaultEdgeColor{0.0f, 0.0f, 0.0f};
constexpr bool kDefaultHasDistanceFrom{false};
constexpr bool kDefaultCustomTransform{false};
constexpr float kDefaultZOffset{0.0f};
constexpr ScatterStyle kDefaultScatterStyle{ScatterStyle::CIRCLE};
constexpr float kDefaultAlpha{1.0f};
constexpr float kDefaultLineWidth{1.0f};
constexpr float kDefaultPointSize{10.0f};
constexpr bool kIsPersistent{false};
constexpr bool kIsUpdateable{false};
constexpr bool kInterpolateColormap{false};
constexpr internal::ItemId kDefaultId{internal::ItemId::UNKNOWN};
constexpr GLenum kDefaultDynamicOrStaticUsage{GL_STATIC_DRAW};
constexpr bool kDefaultNoEdges{false};
constexpr bool kDefaultNoFaces{false};

bool isPlotDataFunction(const Function fcn);

class PropertiesData
{
private:
    bool has_properties_;

public:
    template <typename T> struct OptionalParameter
    {
        bool has_default_value;
        T data;

        OptionalParameter() : has_default_value{true} {}
        OptionalParameter(const T& data_in) : has_default_value{true}, data{data_in} {}

        OptionalParameter& operator=(const T& data_in)
        {
            has_default_value = false;
            data = data_in;

            return *this;
        }
    };

    PropertiesData() : has_properties_{false} {}

    PropertiesData(const CommunicationHeader& hdr) : has_properties_{false}
    {
        const Properties props{hdr};

        // Flags
        is_persistent = props.hasFlag(PropertyFlag::PERSISTENT);
        interpolate_colormap = props.hasFlag(PropertyFlag::INTERPOLATE_COLORMAP);
        is_updateable = props.hasFlag(PropertyFlag::UPDATABLE);
        dynamic_or_static_usage = is_updateable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

        has_properties_ = has_properties_ || is_persistent || interpolate_colormap || is_updateable;

        // Properties
        if (props.hasProperty(PropertyType::ALPHA))
        {
            alpha = props.getProperty<Alpha>().data / 255.0f;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::BUFFER_SIZE))
        {
            buffer_size = props.getProperty<BufferSize>().data;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::SCATTER_STYLE))
        {
            const ScatterStyleContainer ssc = props.getProperty<ScatterStyleContainer>();
            scatter_style = ssc.data;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::LINE_WIDTH))
        {
            line_width = props.getProperty<LineWidth>().data;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::POINT_SIZE))
        {
            point_size = props.getProperty<PointSize>().data;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::Z_OFFSET))
        {
            z_offset = props.getProperty<ZOffset>().data;
            has_properties_ = true;
        }

        if ((props.hasProperty(PropertyType::NAME)))
        {
            name = std::string(props.getProperty<Name>().data);
            has_properties_ = true;
        }

        if ((props.hasProperty(PropertyType::COLOR)))
        {
            const Color col = props.getProperty<Color>();
            color = RGBTripletf{static_cast<float>(col.red) / 255.0f,
                                static_cast<float>(col.green) / 255.0f,
                                static_cast<float>(col.blue) / 255.0f};
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::DISTANCE_FROM))
        {
            distance_from = props.getProperty<DistanceFrom>();
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::TRANSFORM))
        {
            custom_transform = props.getProperty<Transform>();
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::COLOR_MAP))
        {
            color_map = props.getProperty<ColorMapContainer>().data;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::EDGE_COLOR))
        {
            const EdgeColor ec = props.getProperty<EdgeColor>();

            edge_color = RGBTripletf{static_cast<float>(ec.red) / 255.0f,
                                     static_cast<float>(ec.green) / 255.0f,
                                     static_cast<float>(ec.blue) / 255.0f};

            if (!ec.use_color)
            {
                no_edges = true;
            }
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::FACE_COLOR))
        {
            const FaceColor fc = props.getProperty<FaceColor>();

            face_color = RGBTripletf{static_cast<float>(fc.red) / 255.0f,
                                     static_cast<float>(fc.green) / 255.0f,
                                     static_cast<float>(fc.blue) / 255.0f};

            if (!fc.use_color)
            {
                no_faces = true;
            }
            has_properties_ = true;
        }
    }

    template <typename T>
    void overwritePropertyFromOtherIfPresent(OptionalParameter<T>& local, const OptionalParameter<T> other)
    {
        if (!other.has_default_value)
        {
            local = other.data;
            has_properties_ = true;
        }
    }

    void appendProperties(const PropertiesData& props)
    {
        overwritePropertyFromOtherIfPresent(name, props.name);

        //
        overwritePropertyFromOtherIfPresent(alpha, props.alpha);
        overwritePropertyFromOtherIfPresent(buffer_size, props.buffer_size);
        overwritePropertyFromOtherIfPresent(scatter_style, props.scatter_style);
        overwritePropertyFromOtherIfPresent(line_width, props.line_width);
        overwritePropertyFromOtherIfPresent(point_size, props.point_size);
        overwritePropertyFromOtherIfPresent(z_offset, props.z_offset);
        overwritePropertyFromOtherIfPresent(name, props.name);

        /*

        if ((props.hasProperty(PropertyType::COLOR)))
        {
            const Color col = props.getProperty<Color>();
            color = RGBTripletf{static_cast<float>(col.red) / 255.0f,
                                static_cast<float>(col.green) / 255.0f,
                                static_cast<float>(col.blue) / 255.0f};
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::DISTANCE_FROM))
        {
            distance_from = props.getProperty<DistanceFrom>();
            has_distance_from = true;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::TRANSFORM))
        {
            custom_transform = props.getProperty<Transform>();
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::COLOR_MAP))
        {
            color_map = props.getProperty<ColorMapContainer>().data;
            has_color_map = true;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::EDGE_COLOR))
        {
            const EdgeColor ec = props.getProperty<EdgeColor>();

            edge_color = RGBTripletf{static_cast<float>(ec.red) / 255.0f,
                                     static_cast<float>(ec.green) / 255.0f,
                                     static_cast<float>(ec.blue) / 255.0f};

            OptionalParameter<RGBTripletf> edge_color;
            OptionalParameter<bool> no_edges{kDefaultNoEdges};
            if (!ec.use_color)
            {
                no_edges = true;
            }
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::FACE_COLOR))
        {
            const FaceColor fc = props.getProperty<FaceColor>();

            face_color = RGBTripletf{static_cast<float>(fc.red) / 255.0f,
                                     static_cast<float>(fc.green) / 255.0f,
                                     static_cast<float>(fc.blue) / 255.0f};

            if (!fc.use_color)
            {
                no_faces = true;
            }
            has_properties_ = true;
        }
        */

        //

        is_persistent = is_persistent || props.is_persistent;
        is_updateable = is_updateable || props.is_updateable;
        interpolate_colormap = interpolate_colormap || props.interpolate_colormap;
        dynamic_or_static_usage = is_updateable ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
    }

    // Properties
    OptionalParameter<std::string> name{kDefaultName};

    OptionalParameter<ScatterStyle> scatter_style{kDefaultScatterStyle};

    OptionalParameter<float> z_offset{kDefaultZOffset};
    OptionalParameter<float> alpha{kDefaultAlpha};
    OptionalParameter<float> line_width{kDefaultLineWidth};
    OptionalParameter<float> point_size{kDefaultPointSize};

    OptionalParameter<uint16_t> buffer_size{kDefaultBufferSize};

    OptionalParameter<RGBTripletf> color;

    OptionalParameter<RGBTripletf> edge_color{kDefaultEdgeColor};
    bool no_edges{kDefaultNoEdges};

    OptionalParameter<RGBTripletf> face_color;
    bool no_faces{kDefaultNoFaces};

    OptionalParameter<ColorMap> color_map;

    OptionalParameter<DistanceFrom> distance_from;

    OptionalParameter<Transform> custom_transform;

    bool is_persistent{kIsPersistent};
    bool is_updateable{kIsUpdateable};
    bool interpolate_colormap{kInterpolateColormap};
    GLenum dynamic_or_static_usage{kDefaultDynamicOrStaticUsage};
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

    PlotObjectAttributes() {}
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

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::ITEM_ID))
        {
            id = hdr.get(CommunicationHeaderObjectType::ITEM_ID).as<internal::ItemId>();
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

        z_offset = properties_data.z_offset.data;
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
    std::string name_;

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

    // LineStyle line_style_;
    // bool has_line_style_;

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

    void assignProperties(const PropertiesData& properties_data, ColorPicker& color_picker);
    virtual void findMinMax() = 0;

    void postInitialize(ReceivedData& received_data, const CommunicationHeader& hdr, const Properties& props);
    void throwIfNotUpdateable() const;

public:
    size_t getNumDimensions() const;
    virtual ~PlotObjectBase();
    PlotObjectBase() = default;
    PlotObjectBase(ReceivedData& received_data,
                   const CommunicationHeader& hdr,
                   const PlotObjectAttributes& plot_object_attributes,
                   const PropertiesData& properties_data,
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
        lp.name = name_;

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
