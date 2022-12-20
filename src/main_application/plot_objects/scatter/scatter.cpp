#include "main_application/plot_objects/scatter/scatter.h"

#include "outer_converter.h"

namespace
{
struct OutputData
{
    float* points_ptr;
    float* color_data;
};

struct InputParams
{
    size_t num_elements;
    size_t num_bytes_per_element;
    size_t num_bytes_for_one_vec;
    bool has_color;
    float z_offset;

    InputParams() = default;
    InputParams(const size_t num_elements_,
                const size_t num_bytes_per_element_,
                const size_t num_bytes_for_one_vec_,
                const bool has_color_,
                float z_offset_)
        : num_elements{num_elements_},
          num_bytes_per_element{num_bytes_per_element_},
          num_bytes_for_one_vec{num_bytes_for_one_vec_},
          has_color{has_color_},
          z_offset{z_offset_}
    {
    }
};

template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params);

struct Converter
{
    template <class T> OutputData convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};

}  // namespace

Scatter2D::Scatter2D(std::unique_ptr<const ReceivedData> received_data,
                     const CommunicationHeader& hdr,
                     const Properties& props,
                     const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection), vertex_buffer_{OGLPrimitiveType::POINTS}
{
    if (type_ != Function::SCATTER2)
    {
        throw std::runtime_error("Invalid function type for Scatter2D!");
    }

    const InputParams input_params{
        num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_, has_color_, z_offset_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    vertex_buffer_.addBuffer(output_data.points_ptr, num_elements_, 3);

    if (has_color_)
    {
        vertex_buffer_.addBuffer(output_data.color_data, num_elements_, 3);

        delete[] output_data.color_data;
    }

    delete[] output_data.points_ptr;
}

LegendProperties Scatter2D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::DOT;
    lp.color = color_;
    lp.scatter_style_type = scatter_style_type_;
    lp.point_size = point_size_;

    return lp;
}

void Scatter2D::modifyShader()
{
    PlotObjectBase::modifyShader();
    glUseProgram(shader_collection_.scatter_shader.programId());
    glUniform1f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "point_size"), point_size_);
    glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "scatter_mode"),
                static_cast<int>(scatter_style_type_));
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

void Scatter2D::findMinMax()
{
    Vec2d min_vec_2d, max_vec_2d;
    std::tie<Vec2d, Vec2d>(min_vec_2d, max_vec_2d) =
        findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec.x = min_vec_2d.x;
    min_vec.y = min_vec_2d.y;
    min_vec.z = -1.0;

    max_vec.x = max_vec_2d.x;
    max_vec.y = max_vec_2d.y;
    max_vec.z = 1.0;
}

void Scatter2D::render()
{
    glUseProgram(shader_collection_.scatter_shader.programId());
    vertex_buffer_.render(num_elements_);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

Scatter2D::~Scatter2D() {}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    OutputData output_data;
    output_data.points_ptr = new float[3 * input_params.num_elements];

    size_t idx = 0U;

    const T* const input_data_dt_x = reinterpret_cast<const T* const>(input_data);
    const T* const input_data_dt_y = input_data_dt_x + input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        output_data.points_ptr[idx] = input_data_dt_x[k];
        output_data.points_ptr[idx + 1] = input_data_dt_y[k];
        output_data.points_ptr[idx + 2] = input_params.z_offset;
        idx += 3;
    }

    if (input_params.has_color)
    {
        const RGB888* const input_data_rgb =
            reinterpret_cast<const RGB888* const>(input_data_dt_x + 2U * input_params.num_elements);
        output_data.color_data = new float[3 * input_params.num_elements];

        idx = 0;

        for (size_t k = 0; k < input_params.num_elements; k++)
        {
            output_data.color_data[idx] = static_cast<float>(input_data_rgb[k].red) / 255.0f;
            output_data.color_data[idx + 1] = static_cast<float>(input_data_rgb[k].green) / 255.0f;
            output_data.color_data[idx + 2] = static_cast<float>(input_data_rgb[k].blue) / 255.0f;

            idx += 3;
        }
    }

    return output_data;
}

}  // namespace
