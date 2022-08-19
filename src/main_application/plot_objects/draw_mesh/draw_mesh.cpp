#include "main_application/plot_objects/draw_mesh/draw_mesh.h"

float* convertVerticesDataOuter(uint8_t* input_data,
                                const DataType data_type,
                                const uint32_t num_vertices,
                                const uint32_t num_indices,
                                const uint32_t num_bytes_per_element);

DrawMesh::DrawMesh(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::DRAW_MESH)
    {
        throw std::runtime_error("Invalid function type for DrawMesh!");
    }

    num_vertices_ = hdr.get(FunctionHeaderObjectType::NUM_VERTICES).as<uint32_t>();
    num_indices_ = hdr.get(FunctionHeaderObjectType::NUM_INDICES).as<uint32_t>();

    points_ptr_ = convertVerticesDataOuter(data_ptr_, data_type_, num_vertices_, num_indices_, num_bytes_per_element_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_elements_ * 3 * 3, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void DrawMesh::findMinMax()
{
    min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
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
    }
}

void DrawMesh::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_elements_ * 3);
    glBindVertexArray(0);
}

DrawMesh::~DrawMesh() {}

template <typename T>
inline float* convertVerticesData(uint8_t* input_data,
                                  const uint32_t num_vertices,
                                  const uint32_t num_indices,
                                  const uint32_t num_bytes_per_element)
{
    float* output_data = new float[num_indices * 3 * 3];
    Vector<Point3D<T>> vertices;
    Vector<IndexTriplet> indices;

    vertices.setInternalData(reinterpret_cast<Point3D<T>*>(input_data), num_vertices);
    indices.setInternalData(reinterpret_cast<IndexTriplet*>(&(input_data[num_vertices * num_bytes_per_element * 3])),
                            num_indices);

    size_t idx = 0;

    for (size_t k = 0; k < num_indices; k++)
    {
        const Point3D<T> p0 = vertices(indices(k).i0);
        const Point3D<T> p1 = vertices(indices(k).i1);
        const Point3D<T> p2 = vertices(indices(k).i2);

        output_data[idx] = p0.x;
        output_data[idx + 1] = p0.y;
        output_data[idx + 2] = p0.z;

        output_data[idx + 3] = p1.x;
        output_data[idx + 4] = p1.y;
        output_data[idx + 5] = p1.z;

        output_data[idx + 6] = p2.x;
        output_data[idx + 7] = p2.y;
        output_data[idx + 8] = p2.z;
        idx += 9;
    }

    vertices.setInternalData(nullptr, 0);
    indices.setInternalData(nullptr, 0);

    return output_data;
}

float* convertVerticesDataOuter(uint8_t* input_data,
                                       const DataType data_type,
                                       const uint32_t num_vertices,
                                       const uint32_t num_indices,
                                       const uint32_t num_bytes_per_element)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertVerticesData<float>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertVerticesData<double>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertVerticesData<int8_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertVerticesData<int16_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertVerticesData<int32_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertVerticesData<int64_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertVerticesData<uint8_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertVerticesData<uint16_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertVerticesData<uint32_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertVerticesData<uint64_t>(input_data, num_vertices, num_indices, num_bytes_per_element);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
