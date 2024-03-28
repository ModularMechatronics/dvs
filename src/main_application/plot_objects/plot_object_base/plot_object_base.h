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
#include "misc/rgb_triplet.h"
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
constexpr float kDefaultZOffset{0.0f};
constexpr ScatterStyle kDefaultScatterStyle{ScatterStyle::CIRCLE};
constexpr LineStyle kDefaultLineStyle{LineStyle::SOLID};
constexpr float kDefaultAlpha{1.0f};
constexpr float kDefaultLineWidth{2.0f};
constexpr float kDefaultPointSize{10.0f};
constexpr bool kDefaultIsPersistent{false};
constexpr bool kDefaultIsAppendable{false};
constexpr bool kDefaultExcludeFromSelection{false};
constexpr bool kDefaultIsUpdateable{false};
constexpr bool kDefaultInterpolateColormap{false};
constexpr ItemId kDefaultId{ItemId::UNKNOWN};
constexpr GLenum kDefaultDynamicOrStaticUsage{GL_STATIC_DRAW};
constexpr bool kDefaultNoEdges{false};
constexpr bool kDefaultNoFaces{false};

bool isPlotDataFunction(const Function fcn);

class PropertiesData
{
public:
    template <typename T> struct OptionalParameter
    {
        bool has_default_value;
        T data;

        OptionalParameter() : has_default_value{true} {}
        OptionalParameter(const T& data_in) : has_default_value{true}, data{data_in} {}

        OptionalParameter<T>& operator=(const T& data_in)
        {
            static_assert(!std::is_same<OptionalParameter, T>::value,
                          "Assert to guard that this operator is used correctly");

            has_default_value = false;
            data = data_in;

            return *this;
        }
    };

private:
    bool has_properties_;

    template <typename T>
    void overwritePropertyFromOtherIfPresent(OptionalParameter<T>& local, const OptionalParameter<T> other)
    {
        if (!other.has_default_value)
        {
            local = other.data;
            has_properties_ = true;
        }
    }

public:
    PropertiesData() : has_properties_{false} {}

    PropertiesData(const CommunicationHeader& hdr) : has_properties_{false}
    {
        // TODO: Should check which function it is, and return if it's
        // not a function that requires PropertiesData
        const Properties props{hdr};

        // Flags
        is_persistent = props.hasFlag(PropertyFlag::PERSISTENT);
        interpolate_colormap = props.hasFlag(PropertyFlag::INTERPOLATE_COLORMAP);
        is_updateable = props.hasFlag(PropertyFlag::UPDATABLE);
        is_appendable = props.hasFlag(PropertyFlag::APPENDABLE);
        exclude_from_selection = props.hasFlag(PropertyFlag::EXCLUDE_FROM_SELECTION);
        dynamic_or_static_usage = (is_updateable || is_appendable) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

        has_properties_ = has_properties_ || is_persistent || interpolate_colormap || is_updateable || is_appendable ||
                          exclude_from_selection;

        // Properties
        if (props.hasProperty(PropertyType::ALPHA))
        {
            alpha = props.getProperty<Alpha>().data;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::BUFFER_SIZE))
        {
            buffer_size = props.getProperty<BufferSize>().data;
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::SCATTER_STYLE))
        {
            scatter_style = props.getProperty<ScatterStyle>();
            has_properties_ = true;
        }

        if (props.hasProperty(PropertyType::LINE_STYLE))
        {
            line_style = props.getProperty<LineStyle>();
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
            const internal::ColorInternal col{props.getProperty<internal::ColorInternal>()};

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
            color_map = props.getProperty<ColorMap>();
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

    void clear()
    {
        has_properties_ = false;

        name = OptionalParameter<std::string>{kDefaultName};

        scatter_style = OptionalParameter<ScatterStyle>{kDefaultScatterStyle};
        line_style = OptionalParameter<LineStyle>{kDefaultLineStyle};

        z_offset = OptionalParameter<float>{kDefaultZOffset};
        alpha = OptionalParameter<float>{kDefaultAlpha};
        line_width = OptionalParameter<float>{kDefaultLineWidth};
        point_size = OptionalParameter<float>{kDefaultPointSize};

        buffer_size = OptionalParameter<uint16_t>{kDefaultBufferSize};

        color = OptionalParameter<RGBTripletf>{};

        edge_color = OptionalParameter<RGBTripletf>{kDefaultEdgeColor};
        no_edges = kDefaultNoEdges;

        face_color = OptionalParameter<RGBTripletf>{};
        no_faces = kDefaultNoFaces;

        color_map = OptionalParameter<ColorMap>{};

        distance_from = OptionalParameter<DistanceFrom>{};

        custom_transform = OptionalParameter<Transform>{};

        is_persistent = kDefaultIsPersistent;
        is_updateable = kDefaultIsUpdateable;
        is_appendable = kDefaultIsAppendable;
        exclude_from_selection = kDefaultExcludeFromSelection;
        interpolate_colormap = kDefaultInterpolateColormap;
        dynamic_or_static_usage = kDefaultDynamicOrStaticUsage;
    }

    void appendProperties(const PropertiesData& props)
    {
        overwritePropertyFromOtherIfPresent(alpha, props.alpha);
        overwritePropertyFromOtherIfPresent(buffer_size, props.buffer_size);
        overwritePropertyFromOtherIfPresent(scatter_style, props.scatter_style);
        overwritePropertyFromOtherIfPresent(line_style, props.line_style);
        overwritePropertyFromOtherIfPresent(line_width, props.line_width);
        overwritePropertyFromOtherIfPresent(point_size, props.point_size);
        overwritePropertyFromOtherIfPresent(z_offset, props.z_offset);
        overwritePropertyFromOtherIfPresent(name, props.name);
        overwritePropertyFromOtherIfPresent(color, props.color);
        overwritePropertyFromOtherIfPresent(distance_from, props.distance_from);
        overwritePropertyFromOtherIfPresent(custom_transform, props.custom_transform);
        overwritePropertyFromOtherIfPresent(color_map, props.color_map);

        if (!props.edge_color.has_default_value)
        {
            edge_color = props.edge_color.data;
            no_edges = props.no_edges;

            has_properties_ = true;
        }

        if (!props.face_color.has_default_value)
        {
            face_color = props.face_color.data;
            no_faces = props.no_faces;

            has_properties_ = true;
        }

        is_persistent = is_persistent || props.is_persistent;
        is_updateable = is_updateable || props.is_updateable;
        is_appendable = is_appendable || props.is_appendable;
        exclude_from_selection = exclude_from_selection || props.exclude_from_selection;
        interpolate_colormap = interpolate_colormap || props.interpolate_colormap;
        dynamic_or_static_usage = (is_updateable || is_appendable) ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

        has_properties_ =
            has_properties_ || is_persistent || interpolate_colormap || is_updateable || exclude_from_selection;
    }

    bool hasProperties() const
    {
        return has_properties_;
    }

    // Properties
    OptionalParameter<std::string> name{kDefaultName};

    OptionalParameter<ScatterStyle> scatter_style{kDefaultScatterStyle};
    OptionalParameter<LineStyle> line_style{kDefaultLineStyle};

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

    bool is_persistent{kDefaultIsPersistent};
    bool is_updateable{kDefaultIsUpdateable};
    bool is_appendable{kDefaultIsAppendable};
    bool exclude_from_selection{kDefaultExcludeFromSelection};
    bool interpolate_colormap{kDefaultInterpolateColormap};
    GLenum dynamic_or_static_usage{kDefaultDynamicOrStaticUsage};
};

struct ConvertedDataBase
{
    Function function;

    virtual ~ConvertedDataBase() {}
    virtual std::pair<dvs::Vec3<double>, double> getClosestPoint(const Line3D<double>& line) const
    {
        std::cout << "Called base function!" << std::endl;
        return {Vec3<double>{0.0, 0.0, 0.0}, std::numeric_limits<double>::max()};
    }
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
    bool has_point_sizes;

    uint64_t num_bytes_for_one_vec;

    Dimension2D dims;

    uint32_t num_vertices;
    uint32_t num_indices;

    uint8_t num_channels;

    uint32_t num_objects;

    PlotObjectAttributes() {}
    PlotObjectAttributes(const CommunicationHeader& hdr) : function{hdr.getFunction()}
    {
        num_dimensions = getNumDimensionsFromFunction(function);

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::NUM_ELEMENTS))
        {
            num_elements = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();
        }

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::DATA_TYPE))
        {
            data_type = hdr.get(CommunicationHeaderObjectType::DATA_TYPE).as<DataType>();
            num_bytes_per_element = dataTypeToNumBytes(data_type);
            num_bytes_for_one_vec = num_bytes_per_element * num_elements;
        }

        if (hdr.hasObjectWithType(CommunicationHeaderObjectType::ITEM_ID))
        {
            id = hdr.get(CommunicationHeaderObjectType::ITEM_ID).as<ItemId>();
        }

        has_color = hdr.hasObjectWithType(CommunicationHeaderObjectType::HAS_COLOR);
        has_point_sizes = hdr.hasObjectWithType(CommunicationHeaderObjectType::HAS_POINT_SIZES);

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
    ReceivedData received_data_;

    uint8_t* data_ptr_;

    size_t num_dimensions_;
    size_t num_bytes_per_element_;
    uint32_t num_elements_;
    uint64_t num_bytes_for_one_vec_;
    size_t num_added_elements_;

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
    bool has_point_sizes_;

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
    bool is_appendable_;

    bool has_name_;

    ItemId id_;

    void assignProperties(const PropertiesData& properties_data, ColorPicker& color_picker);
    virtual void findMinMax() = 0;

    void postInitialize(ReceivedData& received_data,
                        const CommunicationHeader& hdr,
                        const PropertiesData& properties_data);
    void throwIfNotUpdateable() const;

public:
    size_t getNumDimensions() const;
    virtual ~PlotObjectBase();
    PlotObjectBase() = default;
    PlotObjectBase(ReceivedData& received_data,
                   const CommunicationHeader& hdr,
                   const PlotObjectAttributes& plot_object_attributes,
                   const PropertiesData& properties_data,
                   const ShaderCollection& shader_collection,
                   ColorPicker& color_picker);
    virtual void render() = 0;
    void preRender(const ShaderBase* const shader_to_use);
    bool affectsColormapMinMax() const
    {
        return has_color_map_;
    }

    virtual void appendNewData(ReceivedData& received_data,
                               const CommunicationHeader& hdr,
                               const std::shared_ptr<const ConvertedDataBase>& converted_data,
                               const PropertiesData& properties_data);

    bool isAppendable() const;
    bool isUpdateable() const;

    void setTransform(const MatrixFixed<double, 3, 3>& rotation,
                      const Vec3<double>& translation,
                      const MatrixFixed<double, 3, 3>& scale);

    ItemId getId() const
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
                                   const std::shared_ptr<const ConvertedDataBase>& converted_data,
                                   const PropertiesData& properties_data);

    bool hasName()
    {
        return has_name_;
    }

    virtual void modifyShader();

    void updateProperties(const PropertiesData& properties_data);
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_PLOT_OBJECT_BASE_PLOT_OBJECT_BASE_H_
