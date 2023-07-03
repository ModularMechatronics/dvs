#include "main_application/plot_objects/scatter3/scatter3.h"

#include "outer_converter.h"

namespace
{

struct InputParams
{
    size_t num_elements;
    size_t num_bytes_per_element;
    size_t num_bytes_for_one_vec;
    bool has_color;
    bool has_point_sizes;

    InputParams() = default;
    InputParams(const size_t num_elements_,
                const size_t num_bytes_per_element_,
                const size_t num_bytes_for_one_vec_,
                const bool has_color_,
                const bool has_points_sizes_)
        : num_elements{num_elements_},
          num_bytes_per_element{num_bytes_per_element_},
          num_bytes_for_one_vec{num_bytes_for_one_vec_},
          has_color{has_color_},
          has_point_sizes{has_points_sizes_}
    {
    }
};

struct ConvertedData : ConvertedDataBase
{
    float* points_ptr;
    float* color_ptr;
    float* point_sizes_data;

    ConvertedData() : points_ptr{nullptr}, color_ptr{nullptr}, point_sizes_data{nullptr} {}

    ConvertedData(const ConvertedData& other) = delete;
    ConvertedData& operator=(const ConvertedData& other) = delete;
    ConvertedData(ConvertedData&& other) = delete;
    ConvertedData& operator=(ConvertedData&& other) = delete;

    ~ConvertedData() override
    {
        delete[] points_ptr;
        delete[] color_ptr;
        delete[] point_sizes_data;
    }
};

template <typename T>
std::unique_ptr<ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params);

struct Converter
{
    template <class T>
    std::unique_ptr<ConvertedData> convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};

}  // namespace

Scatter3D::Scatter3D(const CommunicationHeader& hdr,
                     ReceivedData& received_data,
                     const std::unique_ptr<const ConvertedDataBase>& converted_data,

                     const PlotObjectAttributes& plot_object_attributes,
                     const PropertiesData& properties_data,
                     const ShaderCollection& shader_collection,
                     ColorPicker& color_picker)
    : PlotObjectBase(received_data, hdr, plot_object_attributes, properties_data, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::POINTS}
{
    if (function_ != Function::SCATTER3)
    {
        throw std::runtime_error("Invalid function type for Scatter3D!");
    }

    const ConvertedData* const converted_data_local = static_cast<const ConvertedData* const>(converted_data.get());

    num_added_elements_ = 0;

    if (properties_data.is_appendable)
    {
        vertex_buffer_.addExpandableBuffer<float>(properties_data.buffer_size.data, 3);

        vertex_buffer_.updateBufferData(0U, converted_data_local->points_ptr, num_elements_, 3U, num_added_elements_);

        if (has_color_)
        {
            vertex_buffer_.addExpandableBuffer<float>(properties_data.buffer_size.data, 3);
            vertex_buffer_.updateBufferData(
                1U, converted_data_local->color_ptr, num_elements_, 3U, num_added_elements_);
        }

        num_added_elements_ += num_elements_;
    }
    else
    {
        vertex_buffer_.addBuffer(converted_data_local->points_ptr, num_elements_, 3);

        num_added_elements_ = num_elements_;

        if (has_color_)
        {
            vertex_buffer_.addBuffer(converted_data_local->color_ptr, num_elements_, 3);
        }

        if (has_point_sizes_)
        {
            vertex_buffer_.addBuffer(converted_data_local->point_sizes_data, num_elements_, 1, GL_STATIC_DRAW, 2);
        }
    }
}

void Scatter3D::appendNewData(ReceivedData& received_data,
                              const CommunicationHeader& hdr,
                              const std::unique_ptr<const ConvertedDataBase>& converted_data,
                              const PropertiesData& properties_data)
{
    const ConvertedData* const converted_data_local = static_cast<const ConvertedData* const>(converted_data.get());

    num_elements_ = hdr.get(CommunicationHeaderObjectType::NUM_ELEMENTS).as<uint32_t>();

    if ((num_added_elements_ + num_elements_) > buffer_size_)
    {
        DVS_LOG_ERROR() << "Buffer overflow!";
        return;
    }

    vertex_buffer_.updateBufferData(0U, converted_data_local->points_ptr, num_elements_, 3U, num_added_elements_);

    if (has_color_)
    {
        vertex_buffer_.updateBufferData(1U, converted_data_local->color_ptr, num_elements_, 3U, num_added_elements_);
    }

    num_added_elements_ += num_elements_;
}

std::unique_ptr<const ConvertedDataBase> Scatter3D::convertRawData(const CommunicationHeader& hdr,
                                                                   const PlotObjectAttributes& attributes,
                                                                   const PropertiesData& properties_data,
                                                                   const uint8_t* const data_ptr)
{
    const InputParams input_params{attributes.num_elements,
                                   attributes.num_bytes_per_element,
                                   attributes.num_bytes_for_one_vec,
                                   attributes.has_color,
                                   attributes.has_point_sizes};

    std::unique_ptr<const ConvertedDataBase> converted_data_base{
        applyConverter<ConvertedData>(data_ptr, attributes.data_type, Converter{}, input_params)};

    return converted_data_base;
}

LegendProperties Scatter3D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::DOT;
    lp.color = color_;
    lp.scatter_style = scatter_style_;
    lp.point_size = point_size_;

    return lp;
}

void Scatter3D::modifyShader()
{
    PlotObjectBase::modifyShader();
    shader_collection_.scatter_shader.use();
    shader_collection_.scatter_shader.base_uniform_handles.point_size.setFloat(point_size_);
    shader_collection_.scatter_shader.base_uniform_handles.scatter_mode.setInt(static_cast<int>(scatter_style_));

    if (has_point_sizes_)
    {
        shader_collection_.scatter_shader.base_uniform_handles.has_point_sizes_vec.setInt(1);
    }
    else
    {
        shader_collection_.scatter_shader.base_uniform_handles.has_point_sizes_vec.setInt(0);
    }

    if (has_color_)
    {
        shader_collection_.scatter_shader.base_uniform_handles.has_color_vec.setInt(1);
    }
    else if (has_distance_from_)
    {
        shader_collection_.scatter_shader.uniform_handles.distance_from_point.setVec(distance_from_.getPoint());
        shader_collection_.scatter_shader.uniform_handles.min_dist.setFloat(distance_from_.getMinDist());
        shader_collection_.scatter_shader.uniform_handles.max_dist.setFloat(distance_from_.getMaxDist());
        shader_collection_.scatter_shader.uniform_handles.has_distance_from.setInt(1);
        shader_collection_.scatter_shader.uniform_handles.distance_from_type.setInt(
            static_cast<int>(distance_from_.getDistFromType()));
        shader_collection_.scatter_shader.base_uniform_handles.color_map_selection.setInt(static_cast<int>(color_map_) +
                                                                                          1);
        shader_collection_.scatter_shader.base_uniform_handles.has_color_vec.setInt(0);

        shader_collection_.scatter_shader.base_uniform_handles.has_color_vec.setInt(0);
    }
    else
    {
        shader_collection_.scatter_shader.base_uniform_handles.has_color_vec.setInt(0);
    }
}

void Scatter3D::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Scatter3D::render()
{
    shader_collection_.scatter_shader.use();
    vertex_buffer_.render(num_added_elements_);
}

Scatter3D::~Scatter3D() {}

namespace
{
template <typename T>
std::unique_ptr<ConvertedData> convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    ConvertedData* converted_data = new ConvertedData;
    converted_data->points_ptr = new float[3 * input_params.num_elements];

    size_t idx = 0U;

    const T* const input_data_dt_x = reinterpret_cast<const T* const>(input_data);
    const T* const input_data_dt_y = input_data_dt_x + input_params.num_elements;
    const T* const input_data_dt_z = input_data_dt_y + input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        converted_data->points_ptr[idx] = input_data_dt_x[k];
        converted_data->points_ptr[idx + 1] = input_data_dt_y[k];
        converted_data->points_ptr[idx + 2] = input_data_dt_z[k];
        idx += 3;
    }

    if (input_params.has_color)
    {
        const RGB888* input_data_rgb;

        if (input_params.has_point_sizes)
        {
            input_data_rgb = reinterpret_cast<const RGB888* const>(input_data_dt_z + 2U * input_params.num_elements);
        }
        else
        {
            input_data_rgb = reinterpret_cast<const RGB888* const>(input_data_dt_z + input_params.num_elements);
        }

        converted_data->color_ptr = new float[3 * input_params.num_elements];

        idx = 0;

        for (size_t k = 0; k < input_params.num_elements; k++)
        {
            converted_data->color_ptr[idx] = static_cast<float>(input_data_rgb[k].red) / 255.0f;
            converted_data->color_ptr[idx + 1] = static_cast<float>(input_data_rgb[k].green) / 255.0f;
            converted_data->color_ptr[idx + 2] = static_cast<float>(input_data_rgb[k].blue) / 255.0f;

            idx += 3;
        }
    }

    if (input_params.has_point_sizes)
    {
        const T* input_data_point_sizes;

        input_data_point_sizes = reinterpret_cast<const T*>(input_data_dt_z + input_params.num_elements);
        converted_data->point_sizes_data = new float[input_params.num_elements];

        for (size_t k = 0; k < input_params.num_elements; k++)
        {
            converted_data->point_sizes_data[k] = input_data_point_sizes[k];
        }
    }

    return std::unique_ptr<ConvertedData>{converted_data};
}

}  // namespace
