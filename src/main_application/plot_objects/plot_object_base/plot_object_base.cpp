#include "main_application/plot_objects/plot_object_base/plot_object_base.h"

PlotObjectBase::PlotObjectBase(ReceivedData& received_data,
                               const CommunicationHeader& hdr,
                               const PlotObjectAttributes& plot_object_attributes,
                               const PropertiesData& properties_data,
                               const ShaderCollection& shader_collection,
                               ColorPicker& color_picker)
    : received_data_(std::move(received_data)), shader_collection_{shader_collection}
{
    data_ptr_ = received_data_.payloadData();

    function_ = hdr.getFunction();
    data_type_ = hdr.get(CommunicationHeaderObjectType::DATA_TYPE).as<DataType>();

    num_bytes_per_element_ = dataTypeToNumBytes(data_type_);
    num_elements_ = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();

    num_dimensions_ = getNumDimensionsFromFunction(function_);

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

    assignProperties(properties_data, color_picker);
}

void PlotObjectBase::postInitialize(ReceivedData& received_data,
                                    const CommunicationHeader& hdr,
                                    const PropertiesData& properties_data)
{
    received_data_ = std::move(received_data);
    data_ptr_ = received_data_.payloadData();

    min_max_calculated_ = false;
    has_custom_transform_ = false;
    z_offset_ = 0.0f;

    function_ = hdr.getFunction();
    data_type_ = hdr.get(CommunicationHeaderObjectType::DATA_TYPE).as<DataType>();

    num_bytes_per_element_ = dataTypeToNumBytes(data_type_);
    num_elements_ = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();

    num_dimensions_ = getNumDimensionsFromFunction(function_);

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

    updateProperties(properties_data);
}

void PlotObjectBase::updateProperties(const PropertiesData& properties_data)
{
    // Flags
    is_persistent_ = is_persistent_ || properties_data.is_persistent;
    interpolate_colormap_ = interpolate_colormap_ || properties_data.interpolate_colormap;
    is_updateable_ = is_updateable_ || properties_data.is_updateable;

    dynamic_or_static_usage_ = is_updateable_ ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    // Properties
    if (!properties_data.alpha.has_default_value)
    {
        alpha_ = properties_data.alpha.data;
    }

    if (!properties_data.buffer_size.has_default_value)
    {
        buffer_size_ = properties_data.buffer_size.data;
    }

    if (!properties_data.scatter_style.has_default_value)
    {
        scatter_style_ = properties_data.scatter_style.data;
    }

    if (!properties_data.line_width.has_default_value)
    {
        line_width_ = properties_data.line_width.data;
    }

    if (!properties_data.point_size.has_default_value)
    {
        point_size_ = properties_data.point_size.data;
    }

    if (!properties_data.z_offset.has_default_value)
    {
        z_offset_ = properties_data.z_offset.data;
    }

    if (!properties_data.custom_transform.has_default_value)
    {
        has_custom_transform_ = true;
        setTransform(properties_data.custom_transform.data.rotation,
                     properties_data.custom_transform.data.translation,
                     properties_data.custom_transform.data.scale);
    }

    if (!properties_data.distance_from.has_default_value)
    {
        distance_from_ = properties_data.distance_from.data;
        has_distance_from_ = true;
    }

    if (!properties_data.name.has_default_value)
    {
        name_ = properties_data.name.data;
        has_name_ = true;
    }

    if (!properties_data.color.has_default_value)
    {
        color_ = properties_data.color.data;
    }

    if (!properties_data.color_map.has_default_value)
    {
        color_map_ = properties_data.color_map.data;
        has_color_map_ = true;
    }

    if (!properties_data.edge_color.has_default_value)
    {
        if (properties_data.no_edges)
        {
            has_edge_color_ = false;
        }
        else
        {
            edge_color_ = properties_data.edge_color.data;
            has_edge_color_ = true;
        }
    }

    if (!properties_data.face_color.has_default_value)
    {
        if (properties_data.no_faces)
        {
            has_face_color_ = false;
        }
        else
        {
            face_color_ = properties_data.face_color.data;
            has_face_color_ = true;
        }
    }
}

void PlotObjectBase::preRender(const ShaderBase* const shader_to_use)
{
    if (has_custom_transform_)
    {
        glUniform1i(shader_to_use->uniform_handles.has_custom_transform, static_cast<int>(1));
        glUniformMatrix4fv(
            shader_to_use->uniform_handles.custom_translation_mat, 1, GL_FALSE, &custom_translation_[0][0]);
        glUniformMatrix4fv(shader_to_use->uniform_handles.custom_rotation_mat, 1, GL_FALSE, &custom_rotation_[0][0]);
        glUniformMatrix4fv(shader_to_use->uniform_handles.custom_scale_mat, 1, GL_FALSE, &custom_scale_[0][0]);
    }
    else
    {
        glUniform1i(shader_to_use->uniform_handles.has_custom_transform, static_cast<int>(0));
    }
}

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
    return name_;
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

void PlotObjectBase::assignProperties(const PropertiesData& properties_data, ColorPicker& color_picker)
{
    // Flags
    is_persistent_ = properties_data.is_persistent;
    interpolate_colormap_ = properties_data.interpolate_colormap;
    is_updateable_ = properties_data.is_updateable;

    dynamic_or_static_usage_ = properties_data.dynamic_or_static_usage;

    // Properties
    alpha_ = properties_data.alpha.data;
    buffer_size_ = properties_data.buffer_size.data;
    scatter_style_ = properties_data.scatter_style.data;
    line_width_ = properties_data.line_width.data;
    point_size_ = properties_data.point_size.data;

    z_offset_ = properties_data.z_offset.data;

    if (!properties_data.custom_transform.has_default_value)
    {
        has_custom_transform_ = true;
        setTransform(properties_data.custom_transform.data.rotation,
                     properties_data.custom_transform.data.translation,
                     properties_data.custom_transform.data.scale);
    }
    else
    {
        has_custom_transform_ = false;
    }

    if (!properties_data.distance_from.has_default_value)
    {
        has_distance_from_ = true;
        distance_from_ = properties_data.distance_from.data;
    }
    else
    {
        has_distance_from_ = false;
    }

    if (!properties_data.name.has_default_value)
    {
        name_ = properties_data.name.data;
        has_name_ = true;
    }
    else
    {
        has_name_ = false;
    }

    if (!properties_data.color.has_default_value)
    {
        color_ = properties_data.color.data;
    }
    else
    {
        color_ = color_picker.getNextColor();
    }

    if (!properties_data.color_map.has_default_value)
    {
        color_map_ = properties_data.color_map.data;
        has_color_map_ = true;
    }
    else
    {
        has_color_map_ = false;
    }

    if (!properties_data.edge_color.has_default_value)
    {
        if (properties_data.no_edges)
        {
            has_edge_color_ = false;
        }
        else
        {
            has_edge_color_ = true;
            edge_color_ = properties_data.edge_color.data;
        }
    }
    else
    {
        edge_color_ = RGBTripletf(0.0f, 0.0f, 0.0f);
        has_edge_color_ = true;
    }

    if (!properties_data.face_color.has_default_value)
    {
        if (properties_data.no_faces)
        {
            has_face_color_ = false;
        }
        else
        {
            has_face_color_ = true;
            face_color_ = properties_data.face_color.data;
        }
    }
    else
    {
        face_color_ = color_picker.getNextFaceColor();
        has_face_color_ = true;
    }
}

void PlotObjectBase::updateWithNewData(ReceivedData& received_data,
                                       const CommunicationHeader& hdr,
                                       const std::unique_ptr<const ConvertedDataBase>& converted_data,
                                       const PropertiesData& properties_data)
{
    static_cast<void>(received_data);
    static_cast<void>(hdr);
    static_cast<void>(properties_data);
}

void PlotObjectBase::throwIfNotUpdateable() const
{
    if (!is_updateable_)
    {
        throw std::runtime_error("Tried to update non updateable object!");
    }
}

PlotObjectBase::~PlotObjectBase() {}
