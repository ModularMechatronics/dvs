#include "main_application/plot_objects/quiver/quiver.h"


template <typename T>
float* convertQuiverData(uint8_t* input_data, const Dimension2D& dims, const size_t num_bytes_per_element)
{
    Matrix<T> x, y, u, v;

    const size_t num_bytes_per_matrix = (dims.rows * dims.cols) * num_bytes_per_element;

    x.setInternalData(reinterpret_cast<T*>(input_data), dims.rows, dims.cols);
    y.setInternalData(reinterpret_cast<T*>(input_data + num_bytes_per_matrix), dims.rows, dims.cols);
    u.setInternalData(reinterpret_cast<T*>(input_data + 2 * num_bytes_per_matrix), dims.rows, dims.cols);
    v.setInternalData(reinterpret_cast<T*>(input_data + 3 * num_bytes_per_matrix), dims.rows, dims.cols);

    const float ang = 10.0 * M_PI / 180.0;
    const float arrow_edge_length_ratio = 0.9;

    const float ca = std::cos(ang), sa = std::sin(ang);
    const float nca = std::cos(-ang), nsa = std::sin(-ang);

    const Matrix<float> r_0 = {{ca, -sa}, {sa, ca}};
    const Matrix<float> r_1 = {{nca, -nsa}, {nsa, nca}};

    float* points_ptr = new float[dims.rows * dims.cols * 6 * 2];

    size_t idx = 0;

    for (size_t r = 0; r < dims.rows; r++)
    {
        for (size_t c = 0; c < dims.cols; c++)
        {
            const Point2D<float> p0(x(r, c), y(r, c));
            const Point2D<float> p1(x(r, c) + u(r, c), y(r, c) + v(r, c));
            const Vec2D<float> v = p1 - p0;

            const Vec2D<float> vec_short = v * arrow_edge_length_ratio;
            const Vec2D<float> vec_rotated0 = r_0 * vec_short;
            const Vec2D<float> vec_rotated1 = r_1 * vec_short;

            const Point2D<float> p1_left = p0 + vec_rotated0;
            const Point2D<float> p1_right = p0 + vec_rotated1;

            points_ptr[idx + 0] = p0.x;
            points_ptr[idx + 1] = p0.y;

            points_ptr[idx + 2] = p1.x;
            points_ptr[idx + 3] = p1.y;

            points_ptr[idx + 4] = p1.x;
            points_ptr[idx + 5] = p1.y;

            points_ptr[idx + 6] = p1_left.x;
            points_ptr[idx + 7] = p1_left.y;

            points_ptr[idx + 8] = p1.x;
            points_ptr[idx + 9] = p1.y;

            points_ptr[idx + 10] = p1_right.x;
            points_ptr[idx + 11] = p1_right.y;

            idx += 12;
        }
    }

    x.setInternalData(nullptr, 0, 0);
    y.setInternalData(nullptr, 0, 0);
    u.setInternalData(nullptr, 0, 0);
    v.setInternalData(nullptr, 0, 0);

    return points_ptr;
}

inline float* convertQuiverDataOuter(uint8_t* input_data,
                                     const DataType data_type,
                                     const Dimension2D& dims,
                                     const size_t num_bytes_per_element)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertQuiverData<float>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertQuiverData<double>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertQuiverData<int8_t>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertQuiverData<int16_t>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertQuiverData<int32_t>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertQuiverData<int64_t>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertQuiverData<uint8_t>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertQuiverData<uint16_t>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertQuiverData<uint32_t>(input_data, dims, num_bytes_per_element);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertQuiverData<uint64_t>(input_data, dims, num_bytes_per_element);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

Quiver::Quiver(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::QUIVER)
    {
        throw std::runtime_error("Invalid function type for Quiver!");
    }

    dims_ = hdr.get(FunctionHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();
    points_ptr_ = convertQuiverDataOuter(data_ptr_, data_type_, dims_, num_bytes_per_element_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 6 * dims_.rows * dims_.cols, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void Quiver::findMinMax()
{
    min_vec.x = -1.0;
    min_vec.y = -1.0;
    min_vec.z = -1.0;

    max_vec.x = 1.0;
    max_vec.y = 1.0;
    max_vec.z = 1.0;
}

void Quiver::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINE_STRIP, 0, 2 * 6 * dims_.rows * dims_.cols);
    glBindVertexArray(0);
}
