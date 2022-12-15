#include "main_application/plot_objects/plot_collection3/plot_collection3.h"

struct OutputData
{
    float* data_ptr;
    Vec3d min_vec;
    Vec3d max_vec;
};

template <typename T>
OutputData convertCollection3DData(uint8_t* input_data,
                                   const size_t num_objects,
                                   const size_t num_bytes_per_element,
                                   const size_t num_points,
                                   const Vector<uint16_t>& vector_lengths)
{
    const size_t total_num_bytes = num_points * 3 * num_bytes_per_element;
    const size_t num_bytes_per_collection = vector_lengths.sum() * num_bytes_per_element;

    const T* data_x = reinterpret_cast<T*>(input_data);
    const T* data_y = reinterpret_cast<T*>(input_data + num_bytes_per_collection);
    const T* data_z = reinterpret_cast<T*>(input_data + 2 * num_bytes_per_collection);

    OutputData output_data;
    output_data.data_ptr = new float[total_num_bytes];

    size_t idx_offset = 0;
    size_t idx = 0;

    Vec3d min_vec, max_vec;

    min_vec.x = data_x[0];
    min_vec.y = data_y[0];
    min_vec.z = data_z[0];

    max_vec.x = data_x[0];
    max_vec.y = data_y[0];
    max_vec.z = data_z[0];

    for (size_t i = 0; i < num_objects; i++)
    {
        for (size_t k = 0; k < vector_lengths(i) - 1; k++)
        {
            const T x_val = data_x[idx_offset + k];
            const T y_val = data_y[idx_offset + k];
            const T z_val = data_z[idx_offset + k];

            min_vec.x = x_val < min_vec.x ? x_val : min_vec.x;
            min_vec.y = y_val < min_vec.y ? y_val : min_vec.y;
            min_vec.z = z_val < min_vec.z ? z_val : min_vec.z;

            max_vec.x = x_val > max_vec.x ? x_val : max_vec.x;
            max_vec.y = y_val > max_vec.y ? y_val : max_vec.y;
            max_vec.z = z_val > max_vec.z ? z_val : max_vec.z;

            output_data.data_ptr[idx] = x_val;
            output_data.data_ptr[idx + 1] = y_val;
            output_data.data_ptr[idx + 2] = z_val;

            output_data.data_ptr[idx + 3] = data_x[idx_offset + k + 1];
            output_data.data_ptr[idx + 4] = data_y[idx_offset + k + 1];
            output_data.data_ptr[idx + 5] = data_z[idx_offset + k + 1];

            idx += 6;
        }
        idx_offset += vector_lengths(i);
    }

    output_data.min_vec = min_vec;
    output_data.max_vec = max_vec;

    return output_data;
}

inline OutputData convertCollection3DDataOuter(uint8_t* input_data,
                                               const DataType data_type,
                                               const size_t num_objects,
                                               const size_t num_bytes_per_element,
                                               const size_t num_points,
                                               const Vector<uint16_t>& vector_lengths)
{
    OutputData output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data =
            convertCollection3DData<float>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data =
            convertCollection3DData<double>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT8)
    {
        output_data =
            convertCollection3DData<int8_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertCollection3DData<int16_t>(
            input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertCollection3DData<int32_t>(
            input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertCollection3DData<int64_t>(
            input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertCollection3DData<uint8_t>(
            input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertCollection3DData<uint16_t>(
            input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertCollection3DData<uint32_t>(
            input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertCollection3DData<uint64_t>(
            input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

PlotCollection3D::PlotCollection3D(std::unique_ptr<const ReceivedData> received_data,
                                   const CommunicationHeader& hdr,
                                   const Properties& props,
                                   const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection)
{
    if (type_ != Function::PLOT_COLLECTION3)
    {
        throw std::runtime_error("Invalid function type for PlotCollection3D!");
    }

    num_points_ = 0;
    num_objects_ = hdr.get(CommunicationHeaderObjectType::NUM_OBJECTS).as<uint32_t>();

    Vector<uint16_t> vector_lengths(num_objects_);

    std::memcpy(vector_lengths.data(), data_ptr_, num_objects_ * sizeof(uint16_t));

    for (size_t k = 0; k < num_objects_; k++)
    {
        num_points_ += (vector_lengths(k) - 1) * 2;
    }

    // Advance pointer to account for first bytes where 'vector_lengths' are stored
    data_ptr_ += num_objects_ * sizeof(uint16_t);

    OutputData output_data = convertCollection3DDataOuter(
        data_ptr_, data_type_, num_objects_, num_bytes_per_element_, num_points_, vector_lengths);

    points_ptr_ = output_data.data_ptr;
    min_vec = output_data.min_vec;
    max_vec = output_data.max_vec;

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points_ * 3, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void PlotCollection3D::findMinMax()
{
    // Already calculated in constructor phase
}

void PlotCollection3D::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, num_points_);
    glBindVertexArray(0);
}

PlotCollection3D::~PlotCollection3D() {}
