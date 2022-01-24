#include "main_application/plot_objects/draw_tiles/draw_tiles.h"

inline float* convertSingleMatrixDataOuter(uint8_t* input_data,
                                           const DataType data_type,
                                           const Dimension2D dims,
                                           const Vec2D<double>& tile_size);

DrawTiles::DrawTiles(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr)
    : PlotObjectBase(std::move(received_data), hdr)
{
    if (type_ != Function::DRAW_TILES)
    {
        throw std::runtime_error("Invalid function type for DrawTiles!");
    }

    dims_ = hdr.get(FunctionHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();
    const size_t num_bytes_for_matrix = num_bytes_per_element_ * dims_.rows * dims_.cols;

    fillObjectsFromBuffer(&(data_ptr_[num_bytes_for_matrix]), tile_size_);

    points_ptr_ = convertSingleMatrixDataOuter(data_ptr_, data_type_, dims_, tile_size_);
}

void DrawTiles::findMinMax()
{
    min_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    max_vec = {points_ptr_[0], points_ptr_[1], points_ptr_[2]};
    // TODO: Broken? Also, find x-y-min/max from tile_size_ and dimensions, and iterate only over z
    size_t idx = 0;
    for (size_t k = 0; k < (dims_.rows * dims_.cols * 3); k++)
    {
        const Point3Dd current_point(points_ptr_[idx], points_ptr_[idx + 1], points_ptr_[idx + 2]);
        min_vec.x = std::min(current_point.x, min_vec.x);
        min_vec.y = std::min(current_point.y, min_vec.y);
        min_vec.z = std::min(current_point.z, min_vec.z);

        max_vec.x = std::max(current_point.x, max_vec.x);
        max_vec.y = std::max(current_point.y, max_vec.y);
        max_vec.z = std::max(current_point.z, max_vec.z);

        idx += 3;
    }
}

void DrawTiles::render()
{
    if (!visualize_has_run_)
    {
        visualize_has_run_ = true;
        glGenBuffers(1, &buffer_handle_);
        glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 4 * dims_.rows * dims_.cols, points_ptr_, GL_STATIC_DRAW);
    }

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer_handle_);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    setLinewidth(line_width_);

    glPolygonOffset(1, -1);
    glEnable(GL_POLYGON_OFFSET_FILL);
    setColor(face_color_);
    glDrawArrays(GL_QUADS, 0, 4 * dims_.rows * dims_.cols);
    setColor(edge_color_);

    glPolygonMode(GL_FRONT, GL_LINE);
    glPolygonMode(GL_BACK, GL_LINE);
    glDrawArrays(GL_QUADS, 0, 4 * dims_.rows * dims_.cols);
    glPolygonMode(GL_FRONT, GL_FILL);
    glPolygonMode(GL_BACK, GL_FILL);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glDisableVertexAttribArray(0);
}

DrawTiles::~DrawTiles()
{
    delete[] points_ptr_;
}

// TODO: Why is template parameter not used?
template <typename T>
inline float* convertSingleMatrixDataInner(uint8_t* const data_buffer,
                                           const size_t num_rows,
                                           const size_t num_cols,
                                           const Vec2D<double>& tile_size)
{
    float* output_data = new float[num_rows * num_cols * 4 * 3];
    Matrix<float> z;

    z.setInternalData(reinterpret_cast<float*>(data_buffer), num_rows, num_cols);

    size_t idx = 0;
    for (size_t r = 0; r < num_rows; r++)
    {
        for (size_t c = 0; c < num_cols; c++)
        {
            const float x = static_cast<float>(c) * tile_size.x;
            const float y = static_cast<float>(r) * tile_size.y;

            const size_t idx0_x = idx;
            const size_t idx0_y = idx + 1;
            const size_t idx0_z = idx + 2;

            const size_t idx1_x = idx + 3;
            const size_t idx1_y = idx + 4;
            const size_t idx1_z = idx + 5;

            const size_t idx2_x = idx + 6;
            const size_t idx2_y = idx + 7;
            const size_t idx2_z = idx + 8;

            const size_t idx3_x = idx + 9;
            const size_t idx3_y = idx + 10;
            const size_t idx3_z = idx + 11;
            idx = idx + 12;

            output_data[idx0_x] = x;
            output_data[idx1_x] = x;
            output_data[idx2_x] = x + tile_size.x;
            output_data[idx3_x] = x + tile_size.x;

            output_data[idx0_y] = y;
            output_data[idx1_y] = y + tile_size.y;
            output_data[idx2_y] = y + tile_size.y;
            output_data[idx3_y] = y;

            output_data[idx0_z] = z(r, c);
            output_data[idx1_z] = z(r, c);
            output_data[idx2_z] = z(r, c);
            output_data[idx3_z] = z(r, c);
        }
    }

    z.setInternalData(nullptr, 0, 0);

    return output_data;
}

inline float* convertSingleMatrixDataOuter(uint8_t* input_data,
                                           const DataType data_type,
                                           const Dimension2D dims,
                                           const Vec2D<double>& tile_size)
{
    float* output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertSingleMatrixDataInner<float>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertSingleMatrixDataInner<double>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertSingleMatrixDataInner<int8_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertSingleMatrixDataInner<int16_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertSingleMatrixDataInner<int32_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertSingleMatrixDataInner<int64_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertSingleMatrixDataInner<uint8_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertSingleMatrixDataInner<uint16_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertSingleMatrixDataInner<uint32_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertSingleMatrixDataInner<uint64_t>(input_data, dims.rows, dims.cols, tile_size);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
