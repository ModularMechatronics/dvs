#include "main_application/plot_objects/line_collection3/line_collection3.h"

uint8_t* convertData3DLineCollection3Outer(const uint8_t* const input_data,
                            const DataType data_type,
                            const size_t num_elements,
                            const size_t num_bytes_per_element,
                            const size_t num_bytes_for_one_vec);

LineCollection3D::LineCollection3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::LINE_COLLECTION3)
    {
        throw std::runtime_error("Invalid function type for LineCollection3D!");
    }

    points_ptr_ =
        convertData3DLineCollection3Outer(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void LineCollection3D::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void LineCollection3D::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, num_elements_);
    glBindVertexArray(0);
}

LineCollection3D::~LineCollection3D()
{
    delete[] points_ptr_;
}

template <typename T>
uint8_t* convertData3DLineCollection(const uint8_t* const input_data,
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

        // TODO: Redo this as in fastPlot2/lineCollection2
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

uint8_t* convertData3DLineCollection3Outer(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    uint8_t* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertData3DLineCollection<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertData3DLineCollection<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertData3DLineCollection<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertData3DLineCollection<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertData3DLineCollection<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertData3DLineCollection<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertData3DLineCollection<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertData3DLineCollection<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertData3DLineCollection<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertData3DLineCollection<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

