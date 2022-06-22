#include "main_application/plot_objects/plot2d/plot2d.h"

float* convertData2DOuter(const uint8_t* const input_data,
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

    points_ptr_ =
        convertData2DOuter(data_ptr_, data_type_, line_width_ / 3.0f, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    const size_t num_segments = num_elements_ - 1;
    const size_t num_points = num_segments * 6;

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
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
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_points);
    glBindVertexArray(0);
    shader_collection_.basic_plot_shader.use();
}

Plot2D::~Plot2D()
{
    delete[] points_ptr_;
}

LegendProperties Plot2D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

template <typename T>
float* convertData2D(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec,
                       const float line_width)
{
    const size_t num_segments = num_elements - 1;
    const size_t num_points = num_segments * 6;
    float* output_data = new float[2 * num_points];

    size_t idx = 0;

    for (size_t k = 0; k < (num_elements - 2); k++)
    {
        const size_t idx_00 = k * num_bytes_per_element;
        const size_t idx_01 = num_bytes_for_one_vec + k * num_bytes_per_element;

        const size_t idx_10 = (k + 1) * num_bytes_per_element;
        const size_t idx_11 = num_bytes_for_one_vec + (k + 1) * num_bytes_per_element;

        const size_t idx_20 = (k + 2) * num_bytes_per_element;
        const size_t idx_21 = num_bytes_for_one_vec + (k + 2) * num_bytes_per_element;

        const T* const p0x = reinterpret_cast<const T* const>(&(input_data[idx_00]));
        const T* const p0y = reinterpret_cast<const T* const>(&(input_data[idx_01]));

        const T* const p1x = reinterpret_cast<const T* const>(&(input_data[idx_10]));
        const T* const p1y = reinterpret_cast<const T* const>(&(input_data[idx_11]));

        const T* const p2x = reinterpret_cast<const T* const>(&(input_data[idx_20]));
        const T* const p2y = reinterpret_cast<const T* const>(&(input_data[idx_21]));

        const float v0x = static_cast<float>(*p1x) - static_cast<float>(*p0x);
        const float v0y = static_cast<float>(*p1y) - static_cast<float>(*p0y);

        const float v1x = static_cast<float>(*p2x) - static_cast<float>(*p1x);
        const float v1y = static_cast<float>(*p2y) - static_cast<float>(*p1y);

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
        output_data[idx] = static_cast<float>(*p0x) + v0px;
        output_data[idx + 1] = static_cast<float>(*p0y) + v0py;

        output_data[idx + 2] = static_cast<float>(*p0x) - v0px;
        output_data[idx + 3] = static_cast<float>(*p0y) - v0py;

        output_data[idx + 4] = static_cast<float>(*p1x) + v1px;
        output_data[idx + 5] = static_cast<float>(*p1y) + v1py;

        // 2nd triangle
        output_data[idx + 6] = static_cast<float>(*p0x) - v0px;
        output_data[idx + 7] = static_cast<float>(*p0y) - v0py;

        output_data[idx + 8] = static_cast<float>(*p1x) + v1px;
        output_data[idx + 9] = static_cast<float>(*p1y) + v1py;

        output_data[idx + 10] = static_cast<float>(*p1x) - v1px;
        output_data[idx + 11] = static_cast<float>(*p1y) - v1py;

        idx += 12;
    }

    return output_data;
}

float* convertData2DOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const float line_width,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    float* output_data;
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
