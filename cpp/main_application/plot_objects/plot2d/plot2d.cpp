#include "main_application/plot_objects/plot2d/plot2d.h"

Plot2D::InputData convertData2DOuter(const uint8_t* const input_data,
                          const DataType data_type,
                          const float line_width,
                          const size_t num_elements,
                          const size_t num_bytes_per_element,
                          const size_t num_bytes_for_one_vec);

Plot2D::Plot2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::PLOT2)
    {
        throw std::runtime_error("Invalid function type for Plot2D!");
    }

    input_data_ =
        convertData2DOuter(data_ptr_, data_type_, line_width_ / 3.0f, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    const size_t num_segments = num_elements_ - 1;
    const size_t num_points = num_segments * 6;

    glGenBuffers(1, &p0_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p0_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, input_data_.p0, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Prev
    glGenBuffers(1, &p1_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, input_data_.p1, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Next
    glGenBuffers(1, &p2_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, input_data_.p2, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Idx
    glGenBuffers(1, &idx_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, input_data_.idx_data_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

}

void Plot2D::findMinMax()
{
    Vec2Dd min_vec_2d, max_vec_2d;
    std::tie<Vec2Dd, Vec2Dd>(min_vec_2d, max_vec_2d) =
        findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec.x = min_vec_2d.x;
    min_vec.y = min_vec_2d.y;
    min_vec.z = -1.0;

    max_vec.x = max_vec_2d.x;
    max_vec.y = max_vec_2d.y;
    max_vec.z = 1.0;
}

void Plot2D::render()
{
    const size_t num_segments = num_elements_ - 1;
    const size_t num_points = num_segments * 6;

    shader_collection_.plot_shader.use();
    glUniform1f(glGetUniformLocation(shader_collection_.plot_shader.programId(), "line_width"), line_width_ / 1500.0f);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_points);
    glBindVertexArray(0);
    shader_collection_.basic_plot_shader.use();
}

Plot2D::~Plot2D()
{
    delete[] input_data_.p0;
    delete[] input_data_.p1;
    delete[] input_data_.p2;
    delete[] input_data_.idx_data_;
}

LegendProperties Plot2D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

template <typename T>
Plot2D::InputData convertData2D(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec,
                       const float line_width)
{
    const size_t num_segments = num_elements - 1;
    const size_t num_points = num_segments * 6;
    Plot2D::InputData output_data;
    output_data.p0 = new float[2 * num_points];
    output_data.p1 = new float[2 * num_points];
    output_data.p2 = new float[2 * num_points];
    output_data.idx_data_ = new float[2 * num_points];

    size_t idx = 0;
    size_t idx_idx = 0;

    for (size_t k = 0; k < (num_elements - 2); k++)
    {
        const size_t idx_00 = k * num_bytes_per_element;
        const size_t idx_01 = num_bytes_for_one_vec + k * num_bytes_per_element;

        const size_t idx_10 = (k + 1) * num_bytes_per_element;
        const size_t idx_11 = num_bytes_for_one_vec + (k + 1) * num_bytes_per_element;

        const size_t idx_20 = (k + 2) * num_bytes_per_element;
        const size_t idx_21 = num_bytes_for_one_vec + (k + 2) * num_bytes_per_element;

        const T p0x = *reinterpret_cast<const T* const>(&(input_data[idx_00]));
        const T p0y = *reinterpret_cast<const T* const>(&(input_data[idx_01]));

        const T p1x = *reinterpret_cast<const T* const>(&(input_data[idx_10]));
        const T p1y = *reinterpret_cast<const T* const>(&(input_data[idx_11]));

        const T p2x = *reinterpret_cast<const T* const>(&(input_data[idx_20]));
        const T p2y = *reinterpret_cast<const T* const>(&(input_data[idx_21]));

#if 1
        // 1st triangle
        output_data.p0[idx] = p0x;
        output_data.p0[idx + 1] = p0y;

        output_data.p0[idx + 2] = p0x;
        output_data.p0[idx + 3] = p0y;

        output_data.p0[idx + 4] = p0x;
        output_data.p0[idx + 5] = p0y;

        // 2nd triangle
        output_data.p0[idx + 6] = p0x;
        output_data.p0[idx + 7] = p0y;

        output_data.p0[idx + 8] = p0x;
        output_data.p0[idx + 9] = p0y;

        output_data.p0[idx + 10] = p0x;
        output_data.p0[idx + 11] = p0y;
#else
        const float v0x = static_cast<float>(p1x) - static_cast<float>(p0x);
        const float v0y = static_cast<float>(p1y) - static_cast<float>(p0y);

        const float v1x = static_cast<float>(p2x) - static_cast<float>(p1x);
        const float v1y = static_cast<float>(p2y) - static_cast<float>(p1y);

        float v0px = -v0y;
        float v0py = v0x;
        const float v0_len = std::sqrt(v0px * v0px + v0py * v0py);
        v0px *= line_width / v0_len;
        v0py *= line_width / v0_len;

        float v1px = -v1y;
        float v1py = v1x;
        const float v1_len = std::sqrt(v1px * v1px + v1py * v1py);

        v1px *= line_width / v1_len;
        v1py *= line_width / v1_len;

        // 1st triangle
        output_data.p0[idx] = static_cast<float>(p0x) + v0px;
        output_data.p0[idx + 1] = static_cast<float>(p0y) + v0py;

        output_data.p0[idx + 2] = static_cast<float>(p0x) - v0px;
        output_data.p0[idx + 3] = static_cast<float>(p0y) - v0py;

        output_data.p0[idx + 4] = static_cast<float>(p1x) + v1px;
        output_data.p0[idx + 5] = static_cast<float>(p1y) + v1py;

        // 2nd triangle
        output_data.p0[idx + 6] = static_cast<float>(p0x) - v0px;
        output_data.p0[idx + 7] = static_cast<float>(p0y) - v0py;

        output_data.p0[idx + 8] = static_cast<float>(p1x) + v1px;
        output_data.p0[idx + 9] = static_cast<float>(p1y) + v1py;

        output_data.p0[idx + 10] = static_cast<float>(p1x) - v1px;
        output_data.p0[idx + 11] = static_cast<float>(p1y) - v1py;
#endif

        // 1st triangle
        output_data.p1[idx] = p1x;
        output_data.p1[idx + 1] = p1y;

        output_data.p1[idx + 2] = p1x;
        output_data.p1[idx + 3] = p1y;

        output_data.p1[idx + 4] = p1x;
        output_data.p1[idx + 5] = p1y;

        // 2nd triangle
        output_data.p1[idx + 6] = p1x;
        output_data.p1[idx + 7] = p1y;

        output_data.p1[idx + 8] = p1x;
        output_data.p1[idx + 9] = p1y;

        output_data.p1[idx + 10] = p1x;
        output_data.p1[idx + 11] = p1y;

        // 1st triangle
        output_data.p2[idx] = p2x;
        output_data.p2[idx + 1] = p2y;

        output_data.p2[idx + 2] = p2x;
        output_data.p2[idx + 3] = p2y;

        output_data.p2[idx + 4] = p2x;
        output_data.p2[idx + 5] = p2y;

        // 2nd triangle
        output_data.p2[idx + 6] = p2x;
        output_data.p2[idx + 7] = p2y;

        output_data.p2[idx + 8] = p2x;
        output_data.p2[idx + 9] = p2y;

        output_data.p2[idx + 10] = p2x;
        output_data.p2[idx + 11] = p2y;

        // Idx
        output_data.idx_data_[idx_idx] = 0.0f;
        output_data.idx_data_[idx_idx + 1] = 0.0f;
        output_data.idx_data_[idx_idx + 2] = 1.0f;
        output_data.idx_data_[idx_idx + 3] = 1.0f;
        output_data.idx_data_[idx_idx + 4] = 2.0f;
        output_data.idx_data_[idx_idx + 5] = 2.0f;
        output_data.idx_data_[idx_idx + 6] = 3.0f;
        output_data.idx_data_[idx_idx + 7] = 3.0f;
        output_data.idx_data_[idx_idx + 8] = 4.0f;
        output_data.idx_data_[idx_idx + 9] = 4.0f;
        output_data.idx_data_[idx_idx + 10] = 5.0f;
        output_data.idx_data_[idx_idx + 11] = 5.0f;

        idx_idx += 12;
        // idx += 12;

        /*float* ptr = output_data.p0;

        const float ax0 = (ptr[idx + 2] + ptr[idx + 0] + ptr[idx + 4])/3.0f;
        const float ay0 = (ptr[idx + 1] + ptr[idx + 3] + ptr[idx + 5])/3.0f;

        // 2nd triangle
        const float ax1 = (ptr[idx + 6] + ptr[idx + 8] + ptr[idx + 10])/3.0f;
        const float ay1 = (ptr[idx + 7] + ptr[idx + 9] + ptr[idx + 11])/3.0f;

        ptr[idx + 0] = (ptr[idx + 0] - ax0) * 0.98 + ax0;
        ptr[idx + 1] = (ptr[idx + 1] - ay0) * 0.98 + ay0;

        ptr[idx + 2] = (ptr[idx + 2] - ax0) * 0.98 + ax0;
        ptr[idx + 3] = (ptr[idx + 3] - ay0) * 0.98 + ay0;

        ptr[idx + 4] = (ptr[idx + 4] - ax0) * 0.98 + ax0;
        ptr[idx + 5] = (ptr[idx + 5] - ay0) * 0.98 + ay0;

        // 2nd triangle

        ptr[idx + 6] = (ptr[idx + 6] - ax1) * 0.98 + ax1;
        ptr[idx + 7] = (ptr[idx + 7] - ay1) * 0.98 + ay1;

        ptr[idx + 8] = (ptr[idx + 8] - ax1) * 0.98 + ax1;
        ptr[idx + 9] = (ptr[idx + 9] - ay1) * 0.98 + ay1;

        ptr[idx + 10] = (ptr[idx + 10] - ax1) * 0.98 + ax1;
        ptr[idx + 11] = (ptr[idx + 11] - ay1) * 0.98 + ay1;*/

        /////

        idx += 12;
    }

    return output_data;
}

Plot2D::InputData convertData2DOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const float line_width,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    Plot2D::InputData output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertData2D<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertData2D<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertData2D<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertData2D<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertData2D<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertData2D<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertData2D<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertData2D<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertData2D<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertData2D<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec, line_width);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
