#include "main_application/plot_objects/scatter3/scatter3.h"

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

OutputData convertScatterData3DOuter(const uint8_t* const input_data,
                                     const DataType data_type,
                                     const InputParams input_params);

Scatter3D::Scatter3D(std::unique_ptr<const ReceivedData> received_data,
                     const CommunicationHeader& hdr,
                     const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection), vertex_buffer2_{OGLPrimitiveType::POINTS}
{
    if (type_ != Function::SCATTER3)
    {
        throw std::runtime_error("Invalid function type for Scatter3D!");
    }

    const InputParams input_params{num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_, has_color_};
    OutputData output_data = convertScatterData3DOuter(data_ptr_, data_type_, input_params);

    vertex_buffer2_.addBuffer(output_data.points_ptr, num_elements_, 3);

    if (has_color_)
    {
        vertex_buffer2_.addBuffer(output_data.color_ptr, num_elements_, 3);

        delete[] output_data.color_ptr;
    }

    delete[] output_data.points_ptr;
}

void Scatter3D::modifyShader()
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

void Scatter3D::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Scatter3D::render()
{
    glUseProgram(shader_collection_.scatter_shader.programId());
    vertex_buffer2_.render(num_elements_);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

Scatter3D::~Scatter3D() {}

template <typename T> OutputData convertScatterData3D(const uint8_t* const input_data, const InputParams input_params)
{
    OutputData output_data;
    output_data.points_ptr = new float[3 * input_params.num_elements];

    size_t idx = 0U;

    const T* const input_data_dt_x = reinterpret_cast<const T* const>(input_data);
    const T* const input_data_dt_y = input_data_dt_x + input_params.num_elements;
    const T* const input_data_dt_z = input_data_dt_y + input_params.num_elements;

    for (size_t k = 0; k < input_params.num_elements; k++)
    {
        output_data.points_ptr[idx] = input_data_dt_x[k];
        output_data.points_ptr[idx + 1] = input_data_dt_y[k];
        output_data.points_ptr[idx + 2] = input_data_dt_z[k];
        idx += 3;
    }

    if (input_params.has_color)
    {
        const RGB888* const input_data_rgb =
            reinterpret_cast<const RGB888* const>(input_data_dt_z + input_params.num_elements);
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

OutputData convertScatterData3DOuter(const uint8_t* const input_data,
                                     const DataType data_type,
                                     const InputParams input_params)
{
    OutputData output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertScatterData3D<float>(input_data, input_params);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertScatterData3D<double>(input_data, input_params);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertScatterData3D<int8_t>(input_data, input_params);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertScatterData3D<int16_t>(input_data, input_params);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertScatterData3D<int32_t>(input_data, input_params);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertScatterData3D<int64_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertScatterData3D<uint8_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertScatterData3D<uint16_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertScatterData3D<uint32_t>(input_data, input_params);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertScatterData3D<uint64_t>(input_data, input_params);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
