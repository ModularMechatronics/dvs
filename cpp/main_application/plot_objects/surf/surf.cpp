#include "main_application/plot_objects/surf/surf.h"

inline float* convertMatrixDataOuter(uint8_t* input_data,
                                     const DataType data_type,
                                     const Dimension2D dims,
                                     const size_t num_bytes_for_one_vec);

Surf::Surf(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::SURF)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    dims_ = hdr.get(FunctionHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();

    points_ptr_ = convertMatrixDataOuter(data_ptr_, data_type_, dims_, num_bytes_for_one_vec_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * (dims_.rows - 1) * (dims_.cols - 1) * 2, points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

void Surf::findMinMax()
{
    std::tie<Vec3Dd, Vec3Dd>(min_vec, max_vec) =
        findMinMaxFromThreeMatrices(data_ptr_, dims_.rows, dims_.cols, num_bytes_for_one_vec_, data_type_);
}

void Surf::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, 4 * (dims_.rows - 1) * (dims_.cols - 1) * 2);
    glBindVertexArray(0);
}

Surf::~Surf()
{
    delete[] points_ptr_;
}

template <typename T>
float* convertMatrixData(uint8_t* input_data, const Dimension2D dims, const size_t num_bytes_for_one_vec)
{
    Matrix<T> x, y, z;
    x.setInternalData(reinterpret_cast<T*>(input_data), dims.rows, dims.cols);
    y.setInternalData(reinterpret_cast<T*>(&(input_data[num_bytes_for_one_vec])), dims.rows, dims.cols);
    z.setInternalData(reinterpret_cast<T*>(&(input_data[2 * num_bytes_for_one_vec])), dims.rows, dims.cols);

    const size_t new_data_size = (dims.rows - 1) * (dims.cols - 1) * 4 * 3 * 2;

    float* output_data = new float[new_data_size];
    size_t idx = 0;
    for (size_t r = 0; r < (dims.rows - 1); r++)
    {
        for (size_t c = 0; c < (dims.cols - 1); c++)
        {
            const size_t idx0_x = idx;
            const size_t idx0_y = idx + 1;
            const size_t idx0_z = idx + 2;

            const size_t idx1_x = idx + 3;
            const size_t idx1_y = idx + 4;
            const size_t idx1_z = idx + 5;

            const size_t idx2_x = idx + 6;
            const size_t idx2_y = idx + 7;
            const size_t idx2_z = idx + 8;

            idx = idx + 12;

            output_data[idx0_x] = x(r, c);
            output_data[idx1_x] = x(r + 1, c);
            output_data[idx2_x] = x(r, c + 1);

            output_data[idx0_y] = y(r, c);
            output_data[idx1_y] = y(r + 1, c);
            output_data[idx2_y] = y(r, c + 1);

            output_data[idx0_z] = z(r, c);
            output_data[idx1_z] = z(r + 1, c);
            output_data[idx2_z] = z(r, c + 1);

            const size_t idx0_x_2 = idx;
            const size_t idx0_y_2 = idx + 1;
            const size_t idx0_z_2 = idx + 2;

            const size_t idx1_x_2 = idx + 3;
            const size_t idx1_y_2 = idx + 4;
            const size_t idx1_z_2 = idx + 5;

            const size_t idx2_x_2 = idx + 6;
            const size_t idx2_y_2 = idx + 7;
            const size_t idx2_z_2 = idx + 8;

            idx = idx + 12;

            output_data[idx0_x_2] = x(r + 1, c);
            output_data[idx1_x_2] = x(r + 1, c + 1);
            output_data[idx2_x_2] = x(r, c + 1);

            output_data[idx0_y_2] = y(r + 1, c);
            output_data[idx1_y_2] = y(r + 1, c + 1);
            output_data[idx2_y_2] = y(r, c + 1);

            output_data[idx0_z_2] = z(r + 1, c);
            output_data[idx1_z_2] = z(r + 1, c + 1);
            output_data[idx2_z_2] = z(r, c + 1);
        }
    }

    x.setInternalData(nullptr, 0, 0);
    y.setInternalData(nullptr, 0, 0);
    z.setInternalData(nullptr, 0, 0);

    return output_data;
}

float* convertMatrixDataOuter(uint8_t* input_data,
                                     const DataType data_type,
                                     const Dimension2D dims,
                                     const size_t num_bytes_for_one_vec)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertMatrixData<float>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertMatrixData<double>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertMatrixData<int8_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertMatrixData<int16_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertMatrixData<int32_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertMatrixData<int64_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertMatrixData<uint8_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertMatrixData<uint16_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertMatrixData<uint32_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertMatrixData<uint64_t>(input_data, dims, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}

