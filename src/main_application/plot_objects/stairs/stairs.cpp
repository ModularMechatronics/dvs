#include "main_application/plot_objects/stairs/stairs.h"

inline uint8_t* convertStairsOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec);

Stairs::Stairs(std::unique_ptr<const ReceivedData> received_data,
               const CommunicationHeader& hdr,
               const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::STAIRS)
    {
        throw std::runtime_error("Invalid function type for Stairs!");
    }

    points_ptr_ =
        convertStairsOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (num_elements_ * 2 - 1) * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Stairs::findMinMax()
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

void Stairs::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINE_STRIP, 0, num_elements_ * 2 - 1);
    glBindVertexArray(0);
}

Stairs::~Stairs()
{
    delete[] points_ptr_;
}

template <typename T>
uint8_t* convertStairs(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec)
{
    const size_t num_support_points = num_elements - 1;

    uint8_t* output_data = new uint8_t[sizeof(float) * 2 * (num_elements + num_support_points)];

    float f0, f0_1, f1;
    uint8_t* const f0_data = reinterpret_cast<uint8_t*>(&f0);
    uint8_t* const f0_1_data = reinterpret_cast<uint8_t*>(&f0_1);
    uint8_t* const f1_data = reinterpret_cast<uint8_t*>(&f1);
    T t0, t0_1, t1;
    uint8_t* const t0_data = reinterpret_cast<uint8_t*>(&t0);
    uint8_t* const t0_1_data = reinterpret_cast<uint8_t*>(&t0_1);
    uint8_t* const t1_data = reinterpret_cast<uint8_t*>(&t1);

    size_t idx_x = 0;
    size_t idx_y = sizeof(float);

    for (size_t k = 0; k < (num_elements - 1); k++)
    {
        const size_t idx_0 = k * num_bytes_per_element;
        const size_t idx_0_1 = (k + 1) * num_bytes_per_element;
        const size_t idx_1 = num_bytes_for_one_vec + k * num_bytes_per_element;
        const uint8_t* const tmp_ptr_0 = &(input_data[idx_0]);
        const uint8_t* const tmp_ptr_0_1 = &(input_data[idx_0_1]);
        const uint8_t* const tmp_ptr_1 = &(input_data[idx_1]);

        for (size_t i = 0; i < num_bytes_per_element; i++)
        {
            t0_data[i] = tmp_ptr_0[i];
            t0_1_data[i] = tmp_ptr_0_1[i];
            t1_data[i] = tmp_ptr_1[i];
        }

        f0 = t0;
        f0_1 = t0_1;
        f1 = t1;

        for (size_t i = 0; i < sizeof(float); i++)
        {
            output_data[idx_x + i] = f0_data[i];
            output_data[idx_y + i] = f1_data[i];
        }
        idx_x += sizeof(float) * 2;
        idx_y += sizeof(float) * 2;

        for (size_t i = 0; i < sizeof(float); i++)
        {
            output_data[idx_x + i] = f0_1_data[i];
            output_data[idx_y + i] = f1_data[i];
        }
        idx_x += sizeof(float) * 2;
        idx_y += sizeof(float) * 2;
    }

    const size_t last_idx = num_elements - 1;

    const size_t idx_0 = last_idx * num_bytes_per_element;
    const size_t idx_1 = num_bytes_for_one_vec + last_idx * num_bytes_per_element;
    const uint8_t* const tmp_ptr_0 = &(input_data[idx_0]);
    const uint8_t* const tmp_ptr_1 = &(input_data[idx_1]);

    for (size_t i = 0; i < num_bytes_per_element; i++)
    {
        t0_data[i] = tmp_ptr_0[i];
        t1_data[i] = tmp_ptr_1[i];
    }

    f0 = t0;
    f1 = t1;

    for (size_t i = 0; i < sizeof(float); i++)
    {
        output_data[idx_x + i] = f0_data[i];
        output_data[idx_y + i] = f1_data[i];
    }
    idx_x += sizeof(float) * 2;
    idx_y += sizeof(float) * 2;

    return output_data;
}

inline uint8_t* convertStairsOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertStairs<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertStairs<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertStairs<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertStairs<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertStairs<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertStairs<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertStairs<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertStairs<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertStairs<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertStairs<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
