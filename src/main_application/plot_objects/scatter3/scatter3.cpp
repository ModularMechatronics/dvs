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

    InputParams() = default;
    InputParams(const size_t num_elements_,
                const size_t num_bytes_per_element_,
                const size_t num_bytes_for_one_vec_,
                const bool has_color_)
        : num_elements{num_elements_},
          num_bytes_per_element{num_bytes_per_element_},
          num_bytes_for_one_vec{num_bytes_for_one_vec_},
          has_color{has_color_}
    {
    }
};

struct ConvertedData : ConvertedDataBase
{
    float* points_ptr;
    float* color_ptr;

    ConvertedData() : points_ptr{nullptr}, color_ptr{nullptr} {}

    ConvertedData(const ConvertedData& other) = delete;
    ConvertedData& operator=(const ConvertedData& other) = delete;
    ConvertedData(ConvertedData&& other) = delete;
    ConvertedData& operator=(ConvertedData&& other) = delete;

    ~ConvertedData() override
    {
        delete[] points_ptr;
        delete[] color_ptr;
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
                     const Properties& props,
                     const ShaderCollection shader_collection,
                     ColorPicker& color_picker)
    : PlotObjectBase(received_data, hdr, props, shader_collection, color_picker),
      vertex_buffer_{OGLPrimitiveType::POINTS}
{
    if (function_ != Function::SCATTER3)
    {
        throw std::runtime_error("Invalid function type for Scatter3D!");
    }
    const ConvertedData* const converted_data_local = static_cast<const ConvertedData* const>(converted_data.get());

    vertex_buffer_.addBuffer(converted_data_local->points_ptr, num_elements_, 3);

    if (has_color_)
    {
        vertex_buffer_.addBuffer(converted_data_local->color_ptr, num_elements_, 3);
    }
}

std::unique_ptr<const ConvertedDataBase> Scatter3D::convertRawData(const PlotObjectAttributes& attributes,
                                                                   const uint8_t* const data_ptr)
{
    const InputParams input_params{attributes.num_elements,
                                   attributes.num_bytes_per_element,
                                   attributes.num_bytes_for_one_vec,
                                   attributes.has_color};

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
    glUseProgram(shader_collection_.scatter_shader.programId());
    glUniform1f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "point_size"), point_size_);
    glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "scatter_mode"),
                static_cast<int>(scatter_style_));

    if (has_color_)
    {
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "has_color_vec"),
                    static_cast<int>(1));
    }
    else if (has_distance_from_)
    {
        glUniform3f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "distance_from_point"),
                    distance_from_.getPoint().x,
                    distance_from_.getPoint().y,
                    distance_from_.getPoint().z);
        glUniform1f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "min_dist"),
                    distance_from_.getMinDist());
        glUniform1f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "max_dist"),
                    distance_from_.getMaxDist());
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "has_distance_from"),
                    static_cast<int>(1));
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "distance_from_type"),
                    static_cast<int>(distance_from_.getDistFromType()));
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "has_color_vec"),
                    static_cast<int>(0));
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "color_map_selection"),
                    static_cast<int>(color_map_) + 1);
    }
    else
    {
        glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "has_color_vec"),
                    static_cast<int>(0));
    }
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

void Scatter3D::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Scatter3D::render()
{
    glUseProgram(shader_collection_.scatter_shader.programId());
    vertex_buffer_.render(num_elements_);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
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
        const RGB888* const input_data_rgb =
            reinterpret_cast<const RGB888* const>(input_data_dt_z + input_params.num_elements);
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

    return std::unique_ptr<ConvertedData>{converted_data};
}

}  // namespace
