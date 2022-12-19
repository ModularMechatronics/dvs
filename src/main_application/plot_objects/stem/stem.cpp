#include "main_application/plot_objects/stem/stem.h"

#include "outer_converter.h"

namespace
{
struct OutputData
{
    float* lines_data;
    float* points_data;
};

struct InputParams
{
    size_t num_elements;

    InputParams() = default;
    InputParams(const size_t num_elements_) : num_elements{num_elements_} {}
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

Stem::Stem(std::unique_ptr<const ReceivedData> received_data,
           const CommunicationHeader& hdr,
           const Properties& props,
           const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection),
      vertex_buffer_lines_{OGLPrimitiveType::LINES},
      vertex_buffer_points_{OGLPrimitiveType::POINTS}
{
    if (type_ != Function::STEM)
    {
        throw std::runtime_error("Invalid function type for Stem!");
    }

    const InputParams input_params{num_elements_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    vertex_buffer_lines_.addBuffer(output_data.lines_data, num_elements_ * 2, 2);
    vertex_buffer_points_.addBuffer(output_data.points_data, num_elements_, 2);

    delete[] output_data.points_data;
    delete[] output_data.lines_data;
}

void Stem::findMinMax()
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

void Stem::render()
{
    vertex_buffer_lines_.render(num_elements_ * 2);

    glUseProgram(shader_collection_.scatter_shader.programId());
    vertex_buffer_points_.render(num_elements_);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

void Stem::modifyShader()
{
    PlotObjectBase::modifyShader();
    glUseProgram(shader_collection_.scatter_shader.programId());
    glUniform1f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "point_size"), 10);
    glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "scatter_mode"),
                static_cast<int>(2));
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

Stem::~Stem() {}

LegendProperties Stem::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    OutputData output_data;

    output_data.lines_data = new float[4 * input_params.num_elements];
    output_data.points_data = new float[2 * input_params.num_elements];

    size_t lines_idx = 0, points_idx = 0;

    const T* const input_data_dt_x = reinterpret_cast<const T* const>(input_data);
    const T* const input_data_dt_y = input_data_dt_x + input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        const T data_x = input_data_dt_x[k];
        const T data_y = input_data_dt_y[k];

        output_data.lines_data[lines_idx] = data_x;
        output_data.lines_data[lines_idx + 1] = 0.0f;
        output_data.lines_data[lines_idx + 2] = data_x;
        output_data.lines_data[lines_idx + 3] = data_y;

        output_data.points_data[points_idx] = data_x;
        output_data.points_data[points_idx + 1] = data_y;

        lines_idx += 4;
        points_idx += 2;
    }

    return output_data;
}

}  // namespace