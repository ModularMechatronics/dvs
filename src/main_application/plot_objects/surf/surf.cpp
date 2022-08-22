#include "main_application/plot_objects/surf/surf.h"

inline float* convertMatrixDataOuter(uint8_t* input_data,
                                     const DataType data_type,
                                     const Dimension2D dims,
                                     const size_t num_bytes_for_one_vec);
float* convertMatrixColorDataOuter(uint8_t* input_data,
                                     const DataType data_type,
                                     const Dimension2D dims,
                                     const size_t num_bytes_for_one_vec,
                                     const Vec3d min_vec,
                                     const Vec3d max_vec,
                                     const RGBColorMap<float>* const color_map_function);

Surf::Surf(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6 * (dims_.rows - 1) * (dims_.cols - 1), points_ptr_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    if(color_map_set_)
    {
        findMinMax();
        const RGBColorMap<float>* const color_map_function = getColorMapFromColorMapType(color_map_);
        colors_ptr_ = convertMatrixColorDataOuter(data_ptr_, data_type_, dims_, num_bytes_for_one_vec_, min_vec, max_vec, color_map_function);

        glGenBuffers(1, &color_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 6 * (dims_.rows - 1) * (dims_.cols - 1), colors_ptr_, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void*)0 
        );
    }
}

void Surf::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeMatrices(data_ptr_, dims_.rows, dims_.cols, num_bytes_for_one_vec_, data_type_);
}

void Surf::render()
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);

    glUniform3f(glGetUniformLocation(shader_collection_.basic_plot_shader.programId(), "vertex_color"), edge_color_.red, edge_color_.green, edge_color_.blue);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, (dims_.rows - 1) * (dims_.cols - 1) * 6);

    if(color_map_set_)
    {
        glUseProgram(shader_collection_.surf_shader.programId());
    }
    else
    {
        glUniform3f(glGetUniformLocation(shader_collection_.basic_plot_shader.programId(), "vertex_color"), face_color_.red, face_color_.green, face_color_.blue);
    }

    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays(GL_TRIANGLES, 0, (dims_.rows - 1) * (dims_.cols - 1) * 6);

    glBindVertexArray(0);

    if(color_map_set_)
    {
        glUseProgram(shader_collection_.basic_plot_shader.programId());
    }
}

Surf::~Surf()
{
    delete[] points_ptr_;
}

LegendProperties Surf::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::POLYGON;
    lp.edge_color = edge_color_;

    if(color_map_set_)
    {
        lp.color_map_set = true;
        lp.color_map = getColorMapFromColorMapType(color_map_);
    }
    else
    {
        lp.color_map_set = false;
        lp.face_color = face_color_;
    }

    return lp;
}

template <typename T>
float* convertMatrixData(uint8_t* input_data, const Dimension2D dims, const size_t num_bytes_for_one_vec)
{
    Matrix<T> x, y, z;
    x.setInternalData(reinterpret_cast<T*>(input_data), dims.rows, dims.cols);
    y.setInternalData(reinterpret_cast<T*>(&(input_data[num_bytes_for_one_vec])), dims.rows, dims.cols);
    z.setInternalData(reinterpret_cast<T*>(&(input_data[2 * num_bytes_for_one_vec])), dims.rows, dims.cols);

    const size_t new_data_size = (dims.rows - 1) * (dims.cols - 1) * 6 * 3;

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

            const size_t idx3_x = idx + 9;
            const size_t idx3_y = idx + 10;
            const size_t idx3_z = idx + 11;

            const size_t idx4_x = idx + 12;
            const size_t idx4_y = idx + 13;
            const size_t idx4_z = idx + 14;

            const size_t idx5_x = idx + 15;
            const size_t idx5_y = idx + 16;
            const size_t idx5_z = idx + 17;
            idx = idx + 18;

            output_data[idx0_x] = x(r, c);
            output_data[idx1_x] = x(r + 1, c);
            output_data[idx2_x] = x(r + 1, c + 1);
        
            output_data[idx3_x] = x(r, c);
            output_data[idx4_x] = x(r, c + 1);
            output_data[idx5_x] = x(r + 1, c + 1);

            output_data[idx0_y] = y(r, c);
            output_data[idx1_y] = y(r + 1, c);
            output_data[idx2_y] = y(r + 1, c + 1);

            output_data[idx3_y] = y(r, c);
            output_data[idx4_y] = y(r, c + 1);
            output_data[idx5_y] = y(r + 1, c + 1);

            output_data[idx0_z] = z(r, c);
            output_data[idx1_z] = z(r + 1, c);
            output_data[idx2_z] = z(r + 1, c + 1);

            output_data[idx3_z] = z(r, c);
            output_data[idx4_z] = z(r, c + 1);
            output_data[idx5_z] = z(r + 1, c + 1);
        }
    }

    x.setInternalData(nullptr, 0, 0);
    y.setInternalData(nullptr, 0, 0);
    z.setInternalData(nullptr, 0, 0);

    return output_data;
}

template <typename T>
float* convertMatrixColorData(uint8_t* input_data, const Dimension2D dims, const size_t num_bytes_for_one_vec, const Vec3d min_vec, const Vec3d max_vec, const RGBColorMap<float>* const color_map_function)
{
    Matrix<T> x, y, z;
    x.setInternalData(reinterpret_cast<T*>(input_data), dims.rows, dims.cols);
    y.setInternalData(reinterpret_cast<T*>(&(input_data[num_bytes_for_one_vec])), dims.rows, dims.cols);
    z.setInternalData(reinterpret_cast<T*>(&(input_data[2 * num_bytes_for_one_vec])), dims.rows, dims.cols);

    const size_t new_data_size = (dims.rows - 1) * (dims.cols - 1) * 6 * 3;
    const float delta = max_vec.z - min_vec.z;

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

            const size_t idx3_x = idx + 9;
            const size_t idx3_y = idx + 10;
            const size_t idx3_z = idx + 11;

            const size_t idx4_x = idx + 12;
            const size_t idx4_y = idx + 13;
            const size_t idx4_z = idx + 14;

            const size_t idx5_x = idx + 15;
            const size_t idx5_y = idx + 16;
            const size_t idx5_z = idx + 17;
            idx = idx + 18;

            const float mean_z_val = static_cast<float>(z(r, c) + z(r + 1, c) + z(r, c + 1) + z(r + 1, c + 1)) / 4.0f;
            const float height_val = (mean_z_val - min_vec.z) / delta;

            const RGBTripletf col = color_map_function->getColor(height_val);

            output_data[idx0_x] = col.red;
            output_data[idx1_x] = col.red;
            output_data[idx2_x] = col.red;
        
            output_data[idx3_x] = col.red;
            output_data[idx4_x] = col.red;
            output_data[idx5_x] = col.red;

            output_data[idx0_y] = col.green;
            output_data[idx1_y] = col.green;
            output_data[idx2_y] = col.green;

            output_data[idx3_y] = col.green;
            output_data[idx4_y] = col.green;
            output_data[idx5_y] = col.green;

            output_data[idx0_z] = col.blue;
            output_data[idx1_z] = col.blue;
            output_data[idx2_z] = col.blue;

            output_data[idx3_z] = col.blue;
            output_data[idx4_z] = col.blue;
            output_data[idx5_z] = col.blue;

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

float* convertMatrixColorDataOuter(uint8_t* input_data,
                                     const DataType data_type,
                                     const Dimension2D dims,
                                     const size_t num_bytes_for_one_vec,
                                     const Vec3d min_vec,
                                     const Vec3d max_vec,
                                     const RGBColorMap<float>* const color_map_function)
{
    float* output_data;

    if (data_type == DataType::FLOAT)
    {
        output_data = convertMatrixColorData<float>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertMatrixColorData<double>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertMatrixColorData<int8_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertMatrixColorData<int16_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertMatrixColorData<int32_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertMatrixColorData<int64_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertMatrixColorData<uint8_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertMatrixColorData<uint16_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertMatrixColorData<uint32_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertMatrixColorData<uint64_t>(input_data, dims, num_bytes_for_one_vec, min_vec, max_vec, color_map_function);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
