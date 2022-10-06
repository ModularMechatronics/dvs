#include "main_application/plot_objects/plot3d/plot3d.h"

Plot3D::InputData convertData3DOuter(const uint8_t* const input_data,
                                     const DataType data_type,
                                     const size_t num_elements,
                                     const size_t num_bytes_per_element,
                                     const size_t num_bytes_for_one_vec);

Plot3D::Plot3D(std::unique_ptr<const ReceivedData> received_data,
               const CommunicationHeader& hdr,
               const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::PLOT3)
    {
        throw std::runtime_error("Invalid function type for Plot3D!");
    }

    input_data_ =
        convertData3DOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    const size_t num_segments = num_elements_ - 1;
    const size_t num_points = num_segments * 6;

    glGenBuffers(1, &p0_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p0_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 3, input_data_.p0, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Prev
    glGenBuffers(1, &p1_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 3, input_data_.p1, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Next
    glGenBuffers(1, &p2_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 3, input_data_.p2, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Idx
    glGenBuffers(1, &idx_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points, input_data_.idx_data_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);
}

void Plot3D::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Plot3D::render()
{
    const size_t num_segments = num_elements_ - 1;
    const size_t num_points = num_segments * 6;

    shader_collection_.plot_3d_shader.use();
    glUniform1f(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "line_width"),
                line_width_ / 1200.0f);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_points);
    glBindVertexArray(0);
    shader_collection_.basic_plot_shader.use();
}

Plot3D::~Plot3D()
{
    delete[] input_data_.p0;
    delete[] input_data_.p1;
    delete[] input_data_.p2;
    delete[] input_data_.idx_data_;
}

template <typename T>
Plot3D::InputData convertData3D(const uint8_t* const input_data,
                                const size_t num_elements,
                                const size_t num_bytes_per_element,
                                const size_t num_bytes_for_one_vec)
{
    const size_t num_segments = num_elements - 1;
    const size_t num_points = num_segments * 6;
    Plot3D::InputData output_data;
    output_data.p0 = new float[3 * num_points];
    output_data.p1 = new float[3 * num_points];
    output_data.p2 = new float[3 * num_points];
    output_data.idx_data_ = new float[num_points];

    size_t idx = 0;
    size_t idx_idx = 0;

    for (size_t k = 0; k < (num_elements - 2); k++)
    {
        const size_t idx_0x = k * num_bytes_per_element;
        const size_t idx_0y = num_bytes_for_one_vec + k * num_bytes_per_element;
        const size_t idx_0z = 2 * num_bytes_for_one_vec + k * num_bytes_per_element;

        const size_t idx_1x = (k + 1) * num_bytes_per_element;
        const size_t idx_1y = num_bytes_for_one_vec + (k + 1) * num_bytes_per_element;
        const size_t idx_1z = 2 * num_bytes_for_one_vec + (k + 1) * num_bytes_per_element;

        const size_t idx_2x = (k + 2) * num_bytes_per_element;
        const size_t idx_2y = num_bytes_for_one_vec + (k + 2) * num_bytes_per_element;
        const size_t idx_2z = 2 * num_bytes_for_one_vec + (k + 2) * num_bytes_per_element;

        const T p0x = *reinterpret_cast<const T* const>(&(input_data[idx_0x]));
        const T p0y = *reinterpret_cast<const T* const>(&(input_data[idx_0y]));
        const T p0z = *reinterpret_cast<const T* const>(&(input_data[idx_0z]));

        const T p1x = *reinterpret_cast<const T* const>(&(input_data[idx_1x]));
        const T p1y = *reinterpret_cast<const T* const>(&(input_data[idx_1y]));
        const T p1z = *reinterpret_cast<const T* const>(&(input_data[idx_1z]));

        const T p2x = *reinterpret_cast<const T* const>(&(input_data[idx_2x]));
        const T p2y = *reinterpret_cast<const T* const>(&(input_data[idx_2y]));
        const T p2z = *reinterpret_cast<const T* const>(&(input_data[idx_2z]));

        // p0
        // 1st triangle
        output_data.p0[idx + 0] = p0x;
        output_data.p0[idx + 1] = p0y;
        output_data.p0[idx + 2] = p0z;

        output_data.p0[idx + 3] = p0x;
        output_data.p0[idx + 4] = p0y;
        output_data.p0[idx + 5] = p0z;

        output_data.p0[idx + 6] = p0x;
        output_data.p0[idx + 7] = p0y;
        output_data.p0[idx + 8] = p0z;

        // 2nd triangle
        output_data.p0[idx + 9] = p0x;
        output_data.p0[idx + 10] = p0y;
        output_data.p0[idx + 11] = p0z;

        output_data.p0[idx + 12] = p0x;
        output_data.p0[idx + 13] = p0y;
        output_data.p0[idx + 14] = p0z;

        output_data.p0[idx + 15] = p0x;
        output_data.p0[idx + 16] = p0y;
        output_data.p0[idx + 17] = p0z;

        // p1
        // 1st triangle
        output_data.p1[idx] = p1x;
        output_data.p1[idx + 1] = p1y;
        output_data.p1[idx + 2] = p1z;

        output_data.p1[idx + 3] = p1x;
        output_data.p1[idx + 4] = p1y;
        output_data.p1[idx + 5] = p1z;

        output_data.p1[idx + 6] = p1x;
        output_data.p1[idx + 7] = p1y;
        output_data.p1[idx + 8] = p1z;

        // 2nd triangle
        output_data.p1[idx + 9] = p1x;
        output_data.p1[idx + 10] = p1y;
        output_data.p1[idx + 11] = p1z;

        output_data.p1[idx + 12] = p1x;
        output_data.p1[idx + 13] = p1y;
        output_data.p1[idx + 14] = p1z;

        output_data.p1[idx + 15] = p1x;
        output_data.p1[idx + 16] = p1y;
        output_data.p1[idx + 17] = p1z;

        // p2
        // 1st triangle
        output_data.p2[idx] = p2x;
        output_data.p2[idx + 1] = p2y;
        output_data.p2[idx + 2] = p2z;

        output_data.p2[idx + 3] = p2x;
        output_data.p2[idx + 4] = p2y;
        output_data.p2[idx + 5] = p2z;

        output_data.p2[idx + 6] = p2x;
        output_data.p2[idx + 7] = p2y;
        output_data.p2[idx + 8] = p2z;

        // 2nd triangle
        output_data.p2[idx + 9] = p2x;
        output_data.p2[idx + 10] = p2y;
        output_data.p2[idx + 11] = p2z;

        output_data.p2[idx + 12] = p2x;
        output_data.p2[idx + 13] = p2y;
        output_data.p2[idx + 14] = p2z;

        output_data.p2[idx + 15] = p2x;
        output_data.p2[idx + 16] = p2y;
        output_data.p2[idx + 17] = p2z;

        // Idx
        output_data.idx_data_[idx_idx] = 0;
        output_data.idx_data_[idx_idx + 1] = 1;
        output_data.idx_data_[idx_idx + 2] = 2;
        output_data.idx_data_[idx_idx + 3] = 3;
        output_data.idx_data_[idx_idx + 4] = 4;
        output_data.idx_data_[idx_idx + 5] = 5;

        idx_idx += 6;

        idx += 18;
    }

    // Last segment
    const size_t k = num_elements - 2;

    const size_t idx_0x = k * num_bytes_per_element;
    const size_t idx_0y = num_bytes_for_one_vec + k * num_bytes_per_element;
    const size_t idx_0z = 2 * num_bytes_for_one_vec + k * num_bytes_per_element;

    const size_t idx_1x = (k + 1) * num_bytes_per_element;
    const size_t idx_1y = num_bytes_for_one_vec + (k + 1) * num_bytes_per_element;
    const size_t idx_1z = 2 * num_bytes_for_one_vec + (k + 1) * num_bytes_per_element;

    const T p0x = *reinterpret_cast<const T* const>(&(input_data[idx_0x]));
    const T p0y = *reinterpret_cast<const T* const>(&(input_data[idx_0y]));
    const T p0z = *reinterpret_cast<const T* const>(&(input_data[idx_0z]));

    const T p1x = *reinterpret_cast<const T* const>(&(input_data[idx_1x]));
    const T p1y = *reinterpret_cast<const T* const>(&(input_data[idx_1y]));
    const T p1z = *reinterpret_cast<const T* const>(&(input_data[idx_1z]));

    const T v0x = p1x - p0x;
    const T v0y = p1y - p0y;
    const T v0z = p1z - p0z;

    const T p2x = p1x + v0x;
    const T p2y = p1y + v0y;
    const T p2z = p1z + v0z;

    // p0
    // 1st triangle
    output_data.p0[idx] = p0x;
    output_data.p0[idx + 1] = p0y;
    output_data.p0[idx + 2] = p0z;

    output_data.p0[idx + 3] = p0x;
    output_data.p0[idx + 4] = p0y;
    output_data.p0[idx + 5] = p0z;

    output_data.p0[idx + 6] = p0x;
    output_data.p0[idx + 7] = p0y;
    output_data.p0[idx + 8] = p0z;

    // 2nd triangle
    output_data.p0[idx + 9] = p0x;
    output_data.p0[idx + 10] = p0y;
    output_data.p0[idx + 11] = p0z;

    output_data.p0[idx + 12] = p0x;
    output_data.p0[idx + 13] = p0y;
    output_data.p0[idx + 14] = p0z;

    output_data.p0[idx + 15] = p0x;
    output_data.p0[idx + 16] = p0y;
    output_data.p0[idx + 17] = p0z;

    // p1
    // 1st triangle
    output_data.p1[idx] = p1x;
    output_data.p1[idx + 1] = p1y;
    output_data.p1[idx + 2] = p1z;

    output_data.p1[idx + 3] = p1x;
    output_data.p1[idx + 4] = p1y;
    output_data.p1[idx + 5] = p1z;

    output_data.p1[idx + 6] = p1x;
    output_data.p1[idx + 7] = p1y;
    output_data.p1[idx + 8] = p1z;

    // 2nd triangle
    output_data.p1[idx + 9] = p1x;
    output_data.p1[idx + 10] = p1y;
    output_data.p1[idx + 11] = p1z;

    output_data.p1[idx + 12] = p1x;
    output_data.p1[idx + 13] = p1y;
    output_data.p1[idx + 14] = p1z;

    output_data.p1[idx + 15] = p1x;
    output_data.p1[idx + 16] = p1y;
    output_data.p1[idx + 17] = p1z;

    // p2
    // 1st triangle
    output_data.p2[idx] = p2x;
    output_data.p2[idx + 1] = p2y;
    output_data.p2[idx + 2] = p2z;

    output_data.p2[idx + 3] = p2x;
    output_data.p2[idx + 4] = p2y;
    output_data.p2[idx + 5] = p2z;

    output_data.p2[idx + 6] = p2x;
    output_data.p2[idx + 7] = p2y;
    output_data.p2[idx + 8] = p2z;

    // 2nd triangle
    output_data.p2[idx + 9] = p2x;
    output_data.p2[idx + 10] = p2y;
    output_data.p2[idx + 11] = p2z;

    output_data.p2[idx + 12] = p2x;
    output_data.p2[idx + 13] = p2y;
    output_data.p2[idx + 14] = p2z;

    output_data.p2[idx + 15] = p2x;
    output_data.p2[idx + 16] = p2y;
    output_data.p2[idx + 17] = p2z;

    // Idx
    output_data.idx_data_[idx_idx] = 0;
    output_data.idx_data_[idx_idx + 1] = 1;
    output_data.idx_data_[idx_idx + 2] = 2;
    output_data.idx_data_[idx_idx + 3] = 3;
    output_data.idx_data_[idx_idx + 4] = 4;
    output_data.idx_data_[idx_idx + 5] = 5;

    return output_data;
}

Plot3D::InputData convertData3DOuter(const uint8_t* const input_data,
                                     const DataType data_type,
                                     const size_t num_elements,
                                     const size_t num_bytes_per_element,
                                     const size_t num_bytes_for_one_vec)
{
    Plot3D::InputData output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertData3D<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertData3D<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertData3D<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertData3D<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertData3D<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertData3D<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertData3D<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertData3D<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertData3D<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertData3D<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
