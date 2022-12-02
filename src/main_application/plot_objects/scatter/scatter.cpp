#include "main_application/plot_objects/scatter/scatter.h"

struct OutputData
{
    float* points_ptr;
    float* color_ptr;
};

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

OutputData convertDataScatter2DOuter(const uint8_t* const input_data,
                                     const DataType data_type,
                                     const InputParams input_params);

Scatter2D::Scatter2D(std::unique_ptr<const ReceivedData> received_data,
                     const CommunicationHeader& hdr,
                     const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::SCATTER2)
    {
        throw std::runtime_error("Invalid function type for Scatter2D!");
    }

    const InputParams input_params{num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_, has_color_};
    OutputData output_data = convertDataScatter2DOuter(data_ptr_, data_type_, input_params);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 2, output_data.points_ptr, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    if (has_color_)
    {
        glGenBuffers(1, &color_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3, output_data.color_ptr, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

        delete[] output_data.color_ptr;
    }

    delete[] output_data.points_ptr;
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
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_POINTS, 0, num_elements_);
    glBindVertexArray(0);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

Scatter2D::~Scatter2D() {}

template <typename T> OutputData convertDataScatter2D(const uint8_t* const input_data, const InputParams input_params)
{
    OutputData output_data;
    output_data.points_ptr = new float[2 * input_params.num_elements];

    size_t idx = 0U;

    const T* const input_data_dt_x = reinterpret_cast<const T* const>(input_data);
    const T* const input_data_dt_y = input_data_dt_x + input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        output_data.points_ptr[idx] = input_data_dt_x[k];
        output_data.points_ptr[idx + 1] = input_data_dt_y[k];
        idx += 2;
    }

    if (input_params.has_color)
    {
        const RGB888* const input_data_rgb =
            reinterpret_cast<const RGB888* const>(input_data_dt_x + 2U * input_params.num_elements);
        output_data.color_ptr = new float[3 * input_params.num_elements];

        idx = 0;

        for (size_t k = 0; k < input_params.num_elements; k++)
        {
            output_data.color_ptr[idx] = static_cast<float>(input_data_rgb[k].red) / 255.0f;
            output_data.color_ptr[idx + 1] = static_cast<float>(input_data_rgb[k].green) / 255.0f;
            output_data.color_ptr[idx + 2] = static_cast<float>(input_data_rgb[k].blue) / 255.0f;

            idx += 3;
        }
    }

    return output_data;
}

OutputData convertDataScatter2DOuter(const uint8_t* const input_data,
                                     const DataType data_type,
                                     const InputParams input_params)
{
    OutputData output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertDataScatter2D<float>(input_data, input_params);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertDataScatter2D<double>(input_data, input_params);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertDataScatter2D<int8_t>(input_data, input_params);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertDataScatter2D<int16_t>(input_data, input_params);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertDataScatter2D<int32_t>(input_data, input_params);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertDataScatter2D<int64_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertDataScatter2D<uint8_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertDataScatter2D<uint16_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertDataScatter2D<uint32_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertDataScatter2D<uint64_t>(input_data, input_params);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
