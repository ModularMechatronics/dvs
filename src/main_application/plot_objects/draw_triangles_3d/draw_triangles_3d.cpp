#include "main_application/plot_objects/draw_triangles_3d/draw_triangles_3d.h"

float* convertTrianglesData(uint8_t* input_data, const DataType data_type, const uint32_t num_elements);

DrawTriangles3D::DrawTriangles3D(std::unique_ptr<const ReceivedData> received_data, const CommunicationHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::DRAW_TRIANGLES_3D)
    {
        throw std::runtime_error("Invalid function type for DrawTriangles3D!");
    }

    points_ptr_ = convertTrianglesData(data_ptr_, data_type_, num_elements_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3 * 3, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void DrawTriangles3D::findMinMax()
{
    min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    max_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    size_t idx = 0;
    for (size_t k = 0; k < (num_elements_ * 3); k++)
    {
        const Point3d current_point(points_ptr_[idx], points_ptr_[idx + 1], points_ptr_[idx + 2]);
        min_vec.x = std::min(current_point.x, min_vec.x);
        min_vec.y = std::min(current_point.y, min_vec.y);
        min_vec.z = std::min(current_point.z, min_vec.z);

        max_vec.x = std::max(current_point.x, max_vec.x);
        max_vec.y = std::max(current_point.y, max_vec.y);
        max_vec.z = std::max(current_point.z, max_vec.z);

        idx += 3;
    }
}

void DrawTriangles3D::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_elements_ * 3);
    glBindVertexArray(0);
}

DrawTriangles3D::~DrawTriangles3D()
{
    delete[] points_ptr_;
}

template <typename T> float* convertTrianglesDataInner(uint8_t* input_data, const uint32_t num_elements)
{
    float* output_data = new float[num_elements * 3 * 3];
    VectorView<Triangle3D<T>> vertices{reinterpret_cast<Triangle3D<T>*>(input_data), num_elements};

    size_t idx = 0;

    for (size_t k = 0; k < num_elements; k++)
    {
        const Triangle3D<T> tri = vertices(k);

        output_data[idx] = tri.p0.x;
        output_data[idx + 1] = tri.p0.y;
        output_data[idx + 2] = tri.p0.z;

        output_data[idx + 3] = tri.p1.x;
        output_data[idx + 4] = tri.p1.y;
        output_data[idx + 5] = tri.p1.z;

        output_data[idx + 6] = tri.p2.x;
        output_data[idx + 7] = tri.p2.y;
        output_data[idx + 8] = tri.p2.z;
        idx += 9;
    }

    return output_data;
}

float* convertTrianglesData(uint8_t* input_data, const DataType data_type, const uint32_t num_elements)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertTrianglesDataInner<float>(input_data, num_elements);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertTrianglesDataInner<double>(input_data, num_elements);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertTrianglesDataInner<int8_t>(input_data, num_elements);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertTrianglesDataInner<int16_t>(input_data, num_elements);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertTrianglesDataInner<int32_t>(input_data, num_elements);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertTrianglesDataInner<int64_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertTrianglesDataInner<uint8_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertTrianglesDataInner<uint16_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertTrianglesDataInner<uint32_t>(input_data, num_elements);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertTrianglesDataInner<uint64_t>(input_data, num_elements);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
