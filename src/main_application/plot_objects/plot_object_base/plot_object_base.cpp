#include "main_application/plot_objects/plot_object_base/plot_object_base.h"

bool isPlotDataFunction(const Function fcn)
{
    return (fcn == Function::STAIRS) || (fcn == Function::PLOT2) || (fcn == Function::PLOT3) ||
           (fcn == Function::FAST_PLOT2) || (fcn == Function::LINE_COLLECTION2) ||
           (fcn == Function::LINE_COLLECTION3) || (fcn == Function::FAST_PLOT3) || (fcn == Function::STEM) ||
           (fcn == Function::SCATTER2) || (fcn == Function::SCATTER3) || (fcn == Function::SURF) ||
           (fcn == Function::IM_SHOW) || (fcn == Function::PLOT_COLLECTION2) || (fcn == Function::PLOT_COLLECTION3) ||
           (fcn == Function::DRAW_MESH_SEPARATE_VECTORS) || (fcn == Function::DRAW_MESH) ||
           (fcn == Function::REAL_TIME_PLOT);
}

void PlotObjectBase::modifyShader()
{
    glUniform3f(glGetUniformLocation(shader_collection_.basic_plot_shader.programId(), "vertex_color"),
                color_.red,
                color_.green,
                color_.blue);
    glUseProgram(shader_collection_.scatter_shader.programId());
    glUniform3f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "vertex_color"),
                color_.red,
                color_.green,
                color_.blue);
    glUseProgram(shader_collection_.plot_2d_shader.programId());
    glUniform3f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "vertex_color"),
                color_.red,
                color_.green,
                color_.blue);
    glUseProgram(shader_collection_.plot_3d_shader.programId());
    glUniform3f(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "vertex_color"),
                color_.red,
                color_.green,
                color_.blue);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

bool PlotObjectBase::isPersistent() const
{
    return is_persistent_;
}

std::string PlotObjectBase::getName() const
{
    return name_.data;
}

std::string_view PlotObjectBase::getNameStringView() const
{
    return std::string_view(name_.data);
}

std::pair<Vec3d, Vec3d> PlotObjectBase::getMinMaxVectors()
{
    if (!min_max_calculated_)
    {
        min_max_calculated_ = true;
        findMinMax();
    }
    return std::pair<Vec3d, Vec3d>(min_vec, max_vec);
}

size_t PlotObjectBase::getNumDimensions() const
{
    return num_dimensions_;
}

PlotObjectBase::PlotObjectBase() {}

PlotObjectBase::PlotObjectBase(std::unique_ptr<const ReceivedData> received_data,
                               const CommunicationHeader& hdr,
                               const Properties& props,
                               const ShaderCollection shader_collection,
                               ColorPicker& color_picker)
    : received_data_(std::move(received_data)), shader_collection_{shader_collection}
{
    data_ptr_ = received_data_->data();

    type_ = hdr.getFunction();
    data_type_ = hdr.get(CommunicationHeaderObjectType::DATA_TYPE).as<DataType>();

    num_bytes_per_element_ = dataTypeToNumBytes(data_type_);
    num_elements_ = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();

    num_dimensions_ = getNumDimensionsFromFunction(type_);

    if (hdr.hasObjectWithType(CommunicationHeaderObjectType::ITEM_ID))
    {
        id_ = hdr.get(CommunicationHeaderObjectType::ITEM_ID).as<internal::ItemId>();
    }
    else
    {
        id_ = internal::ItemId::UNKNOWN;
    }

    has_color_ = hdr.hasObjectWithType(CommunicationHeaderObjectType::HAS_COLOR);
    num_bytes_for_one_vec_ = num_bytes_per_element_ * num_elements_;

    min_max_calculated_ = false;
    has_custom_transform_ = false;
    z_offset_ = 0.0f;

    has_face_color_ = true;
    has_edge_color_ = true;

    assignProperties(props, color_picker);
}

void PlotObjectBase::initialize(std::unique_ptr<const ReceivedData> received_data,
                                const CommunicationHeader& hdr,
                                const Properties& props)
{
    received_data_ = std::move(received_data);
    data_ptr_ = received_data_->data();

    min_max_calculated_ = false;
    has_custom_transform_ = false;
    z_offset_ = 0.0f;

    type_ = hdr.getFunction();
    data_type_ = hdr.get(CommunicationHeaderObjectType::DATA_TYPE).as<DataType>();

    num_bytes_per_element_ = dataTypeToNumBytes(data_type_);
    num_elements_ = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();

    num_dimensions_ = getNumDimensionsFromFunction(type_);

    if (hdr.hasObjectWithType(CommunicationHeaderObjectType::ITEM_ID))
    {
        id_ = hdr.get(CommunicationHeaderObjectType::ITEM_ID).as<internal::ItemId>();
    }
    else
    {
        id_ = internal::ItemId::UNKNOWN;
    }

    has_color_ = hdr.hasObjectWithType(CommunicationHeaderObjectType::HAS_COLOR);

    num_bytes_for_one_vec_ = num_bytes_per_element_ * num_elements_;

    setProperties(props);
}

void PlotObjectBase::setProperties(const Properties& props)
{
    // Flags
    is_persistent_ = props.hasFlag(PropertyFlag::PERSISTENT) || is_persistent_;
    interpolate_colormap_ = props.hasFlag(PropertyFlag::INTERPOLATE_COLORMAP) || interpolate_colormap_;
    is_updateable_ = props.hasFlag(PropertyFlag::UPDATABLE) || is_updateable_;

    usage_ = is_updateable_ ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // Properties
    if (props.hasProperty(PropertyType::ALPHA))
    {
        alpha_ = props.getProperty<Alpha>().data / 255.0f;
    }

    if (props.hasProperty(PropertyType::BUFFER_SIZE))
    {
        buffer_size_ = props.getProperty<BufferSize>().data;
    }

    if (props.hasProperty(PropertyType::SCATTER_STYLE))
    {
        scatter_style_ = props.getProperty<ScatterStyleContainer>().data;
    }

    if (props.hasProperty(PropertyType::LINE_WIDTH))
    {
        line_width_ = props.getProperty<LineWidth>().data;
    }

    if (props.hasProperty(PropertyType::POINT_SIZE))
    {
        point_size_ = props.getProperty<PointSize>().data;
    }

    if (props.hasProperty(PropertyType::Z_OFFSET))
    {
        z_offset_ = props.getProperty<ZOffset>().data;
    }

    if (props.hasProperty(PropertyType::TRANSFORM))
    {
        const Transform custom_transform = props.getProperty<Transform>();
        has_custom_transform_ = true;

        setTransform(custom_transform.rotation, custom_transform.translation, custom_transform.scale);
    }

    if (props.hasProperty(PropertyType::DISTANCE_FROM))
    {
        distance_from_ = props.getProperty<DistanceFrom>();
        has_distance_from_ = true;
    }

    if (props.hasProperty(PropertyType::NAME))
    {
        name_ = props.getProperty<Name>();
        has_name_ = true;
    }

    if (props.hasProperty(PropertyType::COLOR))
    {
        const Color col = props.getProperty<Color>();
        color_.red = static_cast<float>(col.red) / 255.0f;
        color_.green = static_cast<float>(col.green) / 255.0f;
        color_.blue = static_cast<float>(col.blue) / 255.0f;
    }

    if (props.hasProperty(PropertyType::COLOR_MAP))
    {
        color_map_ = props.getProperty<ColorMapContainer>().data;
        color_map_set_ = true;
        edge_color_ = RGBTripletf(0.0f, 0.0f, 0.0f);
    }

    if (props.hasProperty(PropertyType::EDGE_COLOR))
    {
        const EdgeColor ec = props.getProperty<EdgeColor>();

        if (ec.use_color)
        {
            edge_color_.red = static_cast<float>(ec.red) / 255.0f;
            edge_color_.green = static_cast<float>(ec.green) / 255.0f;
            edge_color_.blue = static_cast<float>(ec.blue) / 255.0f;
            has_edge_color_ = true;
        }
        else
        {
            has_edge_color_ = false;
        }
    }

    if (props.hasProperty(PropertyType::FACE_COLOR))
    {
        const FaceColor fc = props.getProperty<FaceColor>();

        if (fc.use_color)
        {
            face_color_.red = static_cast<float>(fc.red) / 255.0f;
            face_color_.green = static_cast<float>(fc.green) / 255.0f;
            face_color_.blue = static_cast<float>(fc.blue) / 255.0f;
            has_face_color_ = true;
        }
        else
        {
            has_face_color_ = false;
        }
    }

    if (props.hasProperty(PropertyType::LINE_STYLE))
    {
        line_style_ = props.getProperty<LineStyle>();
        is_dashed_ = 1;
    }
}

void PlotObjectBase::preRender(const Shader shader_to_use)
{
    if (has_custom_transform_)
    {
        glUniform1i(glGetUniformLocation(shader_to_use.programId(), "has_custom_transform"), static_cast<int>(1));
        glUniformMatrix4fv(glGetUniformLocation(shader_to_use.programId(), "custom_translation_mat"),
                           1,
                           GL_FALSE,
                           &custom_translation_[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(shader_to_use.programId(), "custom_rotation_mat"),
                           1,
                           GL_FALSE,
                           &custom_rotation_[0][0]);
        glUniformMatrix4fv(
            glGetUniformLocation(shader_to_use.programId(), "custom_scale_mat"), 1, GL_FALSE, &custom_scale_[0][0]);
    }
    else
    {
        glUniform1i(glGetUniformLocation(shader_to_use.programId(), "has_custom_transform"), static_cast<int>(0));
    }
}

void PlotObjectBase::setTransform(const MatrixFixed<double, 3, 3>& rotation,
                                  const Vec3<double>& translation,
                                  const MatrixFixed<double, 3, 3>& scale)
{
    has_custom_transform_ = true;

    custom_scale_ = glm::mat4(1.0f);

    custom_translation_ = glm::translate(glm::mat4(1.0f), glm::vec3(translation.x, translation.y, translation.z));

    custom_rotation_ = glm::mat4(1.0f);

    for (size_t r = 0; r < 3; r++)
    {
        for (size_t c = 0; c < 3; c++)
        {
            custom_rotation_[r][c] = rotation(r, c);
            custom_scale_[r][c] = scale(r, c);
        }
    }
}

void PlotObjectBase::assignProperties(const Properties& props, ColorPicker& color_picker)
{
    // Flags
    is_persistent_ = props.hasFlag(PropertyFlag::PERSISTENT);
    interpolate_colormap_ = props.hasFlag(PropertyFlag::INTERPOLATE_COLORMAP);
    is_updateable_ = props.hasFlag(PropertyFlag::UPDATABLE);

    usage_ = is_updateable_ ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // Properties
    alpha_ = props.getPropertyOrValue<Alpha>(255.0f) / 255.0f;
    buffer_size_ = props.getPropertyOrValue<BufferSize>(kDefaultBufferSize);
    scatter_style_ = props.getPropertyOrValue<ScatterStyleContainer>(ScatterStyle::CIRCLE);
    line_width_ = props.getPropertyOrValue<LineWidth>(1.0f);
    point_size_ = props.getPropertyOrValue<PointSize>(10.0f);

    z_offset_ = props.getPropertyOrValue<ZOffset>(0.0f);

    if (props.hasProperty(PropertyType::TRANSFORM))
    {
        const Transform custom_transform = props.getProperty<Transform>();
        has_custom_transform_ = true;

        setTransform(custom_transform.rotation, custom_transform.translation, custom_transform.scale);
    }

    if (props.hasProperty(PropertyType::DISTANCE_FROM))
    {
        distance_from_ = props.getProperty<DistanceFrom>();
        has_distance_from_ = true;
    }
    else
    {
        has_distance_from_ = false;
    }

    if (props.hasProperty(PropertyType::NAME))
    {
        name_ = props.getProperty<Name>();
        has_name_ = true;
    }
    else
    {
        has_name_ = false;
    }

    if (props.hasProperty(PropertyType::COLOR))
    {
        const Color col = props.getProperty<Color>();
        color_.red = static_cast<float>(col.red) / 255.0f;
        color_.green = static_cast<float>(col.green) / 255.0f;
        color_.blue = static_cast<float>(col.blue) / 255.0f;
    }
    else
    {
        color_ = color_picker.getNextColor();
    }

    if (props.hasProperty(PropertyType::COLOR_MAP))
    {
        color_map_ = props.getProperty<ColorMapContainer>().data;
        color_map_set_ = true;
        edge_color_ = RGBTripletf(0.0f, 0.0f, 0.0f);
    }
    else
    {
        color_map_set_ = false;
    }

    if (props.hasProperty(PropertyType::EDGE_COLOR))
    {
        const EdgeColor ec = props.getProperty<EdgeColor>();

        if (ec.use_color)
        {
            edge_color_.red = static_cast<float>(ec.red) / 255.0f;
            edge_color_.green = static_cast<float>(ec.green) / 255.0f;
            edge_color_.blue = static_cast<float>(ec.blue) / 255.0f;
            has_edge_color_ = true;
        }
        else
        {
            has_edge_color_ = false;
        }
    }
    else
    {
        edge_color_ = RGBTripletf(0.0f, 0.0f, 0.0f);
        has_edge_color_ = true;
    }

    if (props.hasProperty(PropertyType::FACE_COLOR))
    {
        const FaceColor fc = props.getProperty<FaceColor>();

        if (fc.use_color)
        {
            face_color_.red = static_cast<float>(fc.red) / 255.0f;
            face_color_.green = static_cast<float>(fc.green) / 255.0f;
            face_color_.blue = static_cast<float>(fc.blue) / 255.0f;
            has_face_color_ = true;
        }
        else
        {
            has_face_color_ = false;
        }
    }
    else
    {
        face_color_ = color_picker.getNextFaceColor();
    }

    if (props.hasProperty(PropertyType::LINE_STYLE))
    {
        line_style_ = props.getProperty<LineStyleContainer>().data;
        is_dashed_ = 1;
    }
    else
    {
        is_dashed_ = 0;
    }
}

void PlotObjectBase::updateWithNewData(std::unique_ptr<const ReceivedData> received_data,
                                       const CommunicationHeader& hdr,
                                       const Properties& props)
{
    static_cast<void>(received_data);
    static_cast<void>(hdr);
    static_cast<void>(props);
}

void PlotObjectBase::throwIfNotUpdateable() const
{
    if (!is_updateable_)
    {
        throw std::runtime_error("Tried to update non updateable object!");
    }
}

PlotObjectBase::~PlotObjectBase() {}
