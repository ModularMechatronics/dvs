#include "main_application/plot_objects/plot2_collection/plot2_collection.h"

template <typename T>
inline float* convertCollectionData(uint8_t* input_data,
                                    const size_t num_objects,
                                    const size_t num_bytes_per_element,
                                    const size_t num_points,
                                    const Vector<uint16_t>& vector_lengths)
{
    const size_t total_num_bytes = num_points * 2 * num_bytes_per_element;
    const size_t num_bytes_per_collection = vector_lengths.sum() * num_bytes_per_element;

    T* data_x = reinterpret_cast<T*>(input_data);
    T* data_y = reinterpret_cast<T*>(input_data + num_bytes_per_collection);

    float* output_data = new float[total_num_bytes];

    size_t idx_offset = 0;
    size_t idx = 0;

    for (size_t i = 0; i < num_objects; i++)
    {
        for (size_t k = 0; k < vector_lengths(i) - 1; k++)
        {
            output_data[idx] = data_x[idx_offset + k];
            output_data[idx + 1] = data_y[idx_offset + k];

            output_data[idx + 2] = data_x[idx_offset + k + 1];
            output_data[idx + 3] = data_y[idx_offset + k + 1];

            idx += 4;
        }
        idx_offset += vector_lengths(i);
    }

    return output_data;
}

inline float* convertCollectionDataOuter(uint8_t* input_data,
                                         const DataType data_type,
                                         const size_t num_objects,
                                         const size_t num_bytes_per_element,
                                         const size_t num_points,
                                         const Vector<uint16_t>& vector_lengths)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data =
            convertCollectionData<float>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data =
            convertCollectionData<double>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT8)
    {
        output_data =
            convertCollectionData<int8_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT16)
    {
        output_data =
            convertCollectionData<int16_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT32)
    {
        output_data =
            convertCollectionData<int32_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::INT64)
    {
        output_data =
            convertCollectionData<int64_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data =
            convertCollectionData<uint8_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data =
            convertCollectionData<uint16_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data =
            convertCollectionData<uint32_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data =
            convertCollectionData<uint64_t>(input_data, num_objects, num_bytes_per_element, num_points, vector_lengths);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

Plot2Collection::Plot2Collection(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::PLOT2_COLLECTION)
    {
        throw std::runtime_error("Invalid function type for Plot2Collection!");
    }

    num_points_ = 0;
    num_objects_ = hdr.get(FunctionHeaderObjectType::NUM_OBJECTS).as<uint32_t>();

    Vector<uint16_t> vector_lengths(num_objects_);

    for (size_t k = 0; k < num_objects_; k++)
    {
        vector_lengths(k) = data_ptr_[k];
    }

    for (size_t k = 0; k < num_objects_; k++)
    {
        num_points_ += (vector_lengths(k) - 1) * 2;
    }

    // Advance pointer to account for first bytes where 'vector_lengths' are stored
    data_ptr_ += num_objects_ * sizeof(uint8_t);

    points_ptr_ = convertCollectionDataOuter(
        data_ptr_, data_type_, num_objects_, num_bytes_per_element_, num_points_, vector_lengths);
    
    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points_ * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Plot2Collection::findMinMax()
{
    // TODO
    min_vec = {-1.0, -1.0, -1.0};
    max_vec = {1.0, 1.0, 1.0};
    /*min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    max_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    size_t idx = 0;
    for (size_t k = 0; k < (num_indices_ * 3 * 3); k += 3)
    {
        const Point3Dd current_point(points_ptr_[k], points_ptr_[k + 1], points_ptr_[k + 2]);
        min_vec.x = std::min(current_point.x, min_vec.x);
        min_vec.y = std::min(current_point.y, min_vec.y);
        min_vec.z = std::min(current_point.z, min_vec.z);

        max_vec.x = std::max(current_point.x, max_vec.x);
        max_vec.y = std::max(current_point.y, max_vec.y);
        max_vec.z = std::max(current_point.z, max_vec.z);

        idx += 3;
    }*/
}

void Plot2Collection::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, num_points_);
    glBindVertexArray(0);
}

Plot2Collection::~Plot2Collection() {}
