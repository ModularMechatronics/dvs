#include "main_application/plot_objects/scatter3/scatter3.h"

uint8_t* convertScatterData3DOuter(const uint8_t* const input_data,
                            const DataType data_type,
                            const size_t num_elements,
                            const size_t num_bytes_per_element,
                            const size_t num_bytes_for_one_vec);

Scatter3D::Scatter3D(std::unique_ptr<const ReceivedData> received_data, const CommunicationHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::SCATTER3)
    {
        throw std::runtime_error("Invalid function type for Scatter3D!");
    }

    points_ptr_ =
        convertScatterData3DOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    /*float* color_data = new float[num_elements_ * 3];

    for(size_t k = 0; k < (num_elements_ * 3); k += 3)
    {
        color_data[k] = color_.red;
        color_data[k + 1] = color_.green;
        color_data[k + 2] = color_.blue;
    }

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3, color_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );*/

}

void Scatter3D::modifyShader()
{
    PlotObjectBase::modifyShader();
    glUseProgram(shader_collection_.scatter_shader.programId());
    glUniform1f(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "point_size"), point_size_);
    glUniform1i(glGetUniformLocation(shader_collection_.scatter_shader.programId(), "scatter_mode"), static_cast<int>(scatter_style_type_));
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
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_POINTS, 0, num_elements_);
    glBindVertexArray(0);
    glUseProgram(shader_collection_.basic_plot_shader.programId());
}

Scatter3D::~Scatter3D()
{
    delete[] points_ptr_;
}


template <typename T>
uint8_t* convertScatterData3D(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data = new uint8_t[sizeof(float) * 3 * num_elements];

    float f0, f1, f2;
    uint8_t* f0_data = reinterpret_cast<uint8_t*>(&f0);
    uint8_t* f1_data = reinterpret_cast<uint8_t*>(&f1);
    uint8_t* f2_data = reinterpret_cast<uint8_t*>(&f2);
    T t0, t1, t2;
    uint8_t* t0_data = reinterpret_cast<uint8_t*>(&t0);
    uint8_t* t1_data = reinterpret_cast<uint8_t*>(&t1);
    uint8_t* t2_data = reinterpret_cast<uint8_t*>(&t2);

    size_t idx_x = 0;
    size_t idx_y = sizeof(float);
    size_t idx_z = 2 * sizeof(float);

    for (size_t k = 0; k < num_elements; k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + k * num_bytes_per_element;
        const size_t idx_2 = num_bytes_for_one_vec * 2 + k * num_bytes_per_element;
        const uint8_t* const tmp_ptr_0 = &(input_data[idx_0]);
        const uint8_t* const tmp_ptr_1 = &(input_data[idx_1]);
        const uint8_t* const tmp_ptr_2 = &(input_data[idx_2]);

        for (size_t i = 0; i < num_bytes_per_element; i++)
        {
            t0_data[i] = tmp_ptr_0[i];
            t1_data[i] = tmp_ptr_1[i];
            t2_data[i] = tmp_ptr_2[i];
        }

        f0 = t0;
        f1 = t1;
        f2 = t2;

        for (size_t i = 0; i < sizeof(float); i++)
        {
            output_data[idx_x + i] = f0_data[i];
            output_data[idx_y + i] = f1_data[i];
            output_data[idx_z + i] = f2_data[i];
        }
        idx_x += sizeof(float) * 3;
        idx_y += sizeof(float) * 3;
        idx_z += sizeof(float) * 3;
    }

    return output_data;
}

uint8_t* convertScatterData3DOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertScatterData3D<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertScatterData3D<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertScatterData3D<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertScatterData3D<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertScatterData3D<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertScatterData3D<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertScatterData3D<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertScatterData3D<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertScatterData3D<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertScatterData3D<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

