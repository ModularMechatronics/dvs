#include "main_application/plot_objects/stem/stem.h"

#include <utility>

std::pair<float*, float*> convertStemDataOuter(const uint8_t* const input_data,
                                               const DataType data_type,
                                               const size_t num_elements,
                                               const size_t num_bytes_per_element,
                                               const size_t num_bytes_for_one_vec);

Stem::Stem(std::unique_ptr<const ReceivedData> received_data,
           const CommunicationHeader& hdr,
           const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::STEM)
    {
        throw std::runtime_error("Invalid function type for Stem!");
    }

    std::tie<float*, float*>(lines_ptr_, points_ptr_) =
        convertStemDataOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &lines_vertex_buffer_array_);
    glBindVertexArray(lines_vertex_buffer_array_);

    glGenBuffers(1, &lines_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, lines_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 4, lines_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Points

    glGenVertexArrays(1, &points_vertex_buffer_array_);
    glBindVertexArray(points_vertex_buffer_array_);

    glGenBuffers(1, &points_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, points_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
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
    glBindVertexArray(lines_vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, num_elements_ * 2);
    glBindVertexArray(0);

    glUseProgram(shader_collection_.scatter_shader.programId());
    glBindVertexArray(points_vertex_buffer_array_);
    glDrawArrays(GL_POINTS, 0, num_elements_);
    glBindVertexArray(0);
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

Stem::~Stem()
{
    delete[] points_ptr_;
    delete[] lines_ptr_;
}

LegendProperties Stem::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

template <typename T>
std::pair<float*, float*> convertStemData(const uint8_t* const input_data,
                                          const size_t num_elements,
                                          const size_t num_bytes_per_element,
                                          const size_t num_bytes_for_one_vec)
{
    float* lines_data = new float[4 * num_elements];
    float* points_data = new float[2 * num_elements];
    size_t lines_idx = 0, points_idx = 0;

    for (size_t k = 0; k < num_elements; k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + k * num_bytes_per_element;

        const T data_x = *reinterpret_cast<const T* const>(&(input_data[idx_0]));
        const T data_y = *reinterpret_cast<const T* const>(&(input_data[idx_1]));

        lines_data[lines_idx] = data_x;
        lines_data[lines_idx + 1] = 0.0f;
        lines_data[lines_idx + 2] = data_x;
        lines_data[lines_idx + 3] = data_y;

        points_data[points_idx] = data_x;
        points_data[points_idx + 1] = data_y;

        lines_idx += 4;
        points_idx += 2;
    }

    std::pair<float*, float*> output_data;
    output_data.first = lines_data;
    output_data.second = points_data;

    return output_data;
}

std::pair<float*, float*> convertStemDataOuter(const uint8_t* const input_data,
                                               const DataType data_type,
                                               const size_t num_elements,
                                               const size_t num_bytes_per_element,
                                               const size_t num_bytes_for_one_vec)
{
    std::pair<float*, float*> output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertStemData<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertStemData<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertStemData<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertStemData<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertStemData<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertStemData<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertStemData<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertStemData<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertStemData<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertStemData<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
