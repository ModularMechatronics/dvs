#include "main_application/plot_objects/surf/surf.h"

#include "outer_converter.h"

namespace
{
struct OutputData
{
    float* points_ptr;
    float* lines_ptr;
    float* normals_ptr;
    float* mean_height_ptr;
    float* color_data;
};

struct InputParams
{
    Dimension2D dims;
    size_t num_bytes_for_one_vec;
    bool has_color;

    InputParams() = default;
    InputParams(const Dimension2D dims_, const size_t num_bytes_for_one_vec_, const bool has_color_)
        : dims{dims_}, num_bytes_for_one_vec{num_bytes_for_one_vec_}, has_color{has_color_}
    {
    }
};

template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params);

struct Converter
{
    template <class T> OutputData convert(const uint8_t* const input_data, const InputParams& input_params) const
    {
        return convertData<T>(input_data, input_params);
    }
};

}  // namespace

Surf::Surf(std::unique_ptr<const ReceivedData> received_data,
           const CommunicationHeader& hdr,
           const Properties& props,
           const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection),
      vertex_buffer_{OGLPrimitiveType::TRIANGLES},
      vertex_buffer_lines_{OGLPrimitiveType::LINES}
{
    if (type_ != Function::SURF)
    {
        throw std::runtime_error("Invalid function type for Surf!");
    }

    dims_ = hdr.get(CommunicationHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();

    const InputParams input_params{dims_, num_bytes_for_one_vec_, has_color_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    num_elements_to_render_ = (dims_.rows - 1U) * (dims_.cols - 1U) * 6U;

    const size_t num_lines =
        (input_params.dims.rows - 1U) * input_params.dims.cols + input_params.dims.rows * (input_params.dims.cols - 1U);
    num_lines_to_render_ = num_lines * 2U;

    vertex_buffer_lines_.addBuffer(output_data.lines_ptr, num_lines_to_render_, 3);

    vertex_buffer_.addBuffer(output_data.points_ptr, num_elements_to_render_, 3);
    vertex_buffer_.addBuffer(output_data.normals_ptr, num_elements_to_render_, 3);
    vertex_buffer_.addBuffer(output_data.mean_height_ptr, num_elements_to_render_, 1);

    if (has_color_)
    {
        vertex_buffer_.addBuffer(output_data.color_data, num_elements_to_render_, 3);

        delete[] output_data.color_data;
    }

    findMinMax();

    delete[] output_data.lines_ptr;
    delete[] output_data.points_ptr;
    delete[] output_data.normals_ptr;
    delete[] output_data.mean_height_ptr;
}

void Surf::findMinMax()
{
    if (!min_max_calculated_)
    {
        std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
            findMinMaxFromThreeMatrices(data_ptr_, dims_.rows, dims_.cols, num_bytes_for_one_vec_, data_type_);
    }
}

void Surf::updateWithNewData(std::unique_ptr<const ReceivedData> received_data,
                             const CommunicationHeader& hdr,
                             const Properties& props)
{
    throwIfNotUpdateable();

    initialize(std::move(received_data), hdr, props);

    dims_ = hdr.get(CommunicationHeaderObjectType::DIMENSION_2D).as<internal::Dimension2D>();

    const InputParams input_params{dims_, num_bytes_for_one_vec_, has_color_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    num_elements_to_render_ = (dims_.rows - 1U) * (dims_.cols - 1U) * 6U;

    const size_t num_lines =
        (input_params.dims.rows - 1U) * input_params.dims.cols + input_params.dims.rows * (input_params.dims.cols - 1U);
    num_lines_to_render_ = num_lines * 2U;

    vertex_buffer_lines_.addBuffer(output_data.lines_ptr, num_lines_to_render_, 3);

    vertex_buffer_.updateBufferData(0, output_data.points_ptr, num_elements_to_render_, 3);
    vertex_buffer_.updateBufferData(1, output_data.normals_ptr, num_elements_to_render_, 3);
    vertex_buffer_.updateBufferData(2, output_data.mean_height_ptr, num_elements_to_render_, 1);

    if (has_color_)
    {
        vertex_buffer_.updateBufferData(3, output_data.color_data, num_elements_to_render_, 3);

        delete[] output_data.color_data;
    }

    findMinMax();

    delete[] output_data.lines_ptr;
    delete[] output_data.points_ptr;
    delete[] output_data.normals_ptr;
    delete[] output_data.mean_height_ptr;
}

bool Surf::affectsColormapMinMax() const
{
    return color_map_set_;
}

void Surf::render()
{
    glEnable(GL_BLEND);

    shader_collection_.draw_mesh_shader.use();
    glUniform3f(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "edge_color"),
                edge_color_.red,
                edge_color_.green,
                edge_color_.blue);
    glUniform3f(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "face_color"),
                face_color_.red,
                face_color_.green,
                face_color_.blue);
    glUniform1f(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "min_z"), min_vec.z);
    glUniform1f(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "max_z"), max_vec.z);
    glUniform1f(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "alpha"), alpha_);

    if (has_color_)
    {
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "has_color_vec"),
                    static_cast<int>(1));
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "color_map_selection"), 0);
    }
    else
    {
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "has_color_vec"),
                    static_cast<int>(0));
        if (color_map_set_)
        {
            glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "color_map_selection"),
                        static_cast<int>(color_map_) + 1);
            glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "interpolate_colormap"),
                        static_cast<int>(interpolate_colormap_));
        }
        else
        {
            glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "color_map_selection"),
                        0);
        }
    }

    if (has_edge_color_)
    {
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "is_edge"), 1);
        vertex_buffer_lines_.render(num_lines_to_render_);
    }

    if (has_face_color_)
    {
        glUniform1i(glGetUniformLocation(shader_collection_.draw_mesh_shader.programId(), "is_edge"), 0);
        vertex_buffer_.render(num_elements_to_render_);
    }

    glDisable(GL_BLEND);

    shader_collection_.basic_plot_shader.use();
}

Surf::~Surf() {}

LegendProperties Surf::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::POLYGON;
    lp.edge_color = edge_color_;

    if (color_map_set_)
    {
        lp.color_map_set = true;
        lp.color_map = color_map_;
    }
    else
    {
        lp.color_map_set = false;
        lp.face_color = face_color_;
    }

    return lp;
}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    const MatrixConstView<T> x{reinterpret_cast<const T*>(input_data), input_params.dims.rows, input_params.dims.cols},
        y{reinterpret_cast<const T*>(&(input_data[input_params.num_bytes_for_one_vec])),
          input_params.dims.rows,
          input_params.dims.cols},
        z{reinterpret_cast<const T*>(&(input_data[2 * input_params.num_bytes_for_one_vec])),
          input_params.dims.rows,
          input_params.dims.cols};

    const size_t new_data_size = (input_params.dims.rows - 1) * (input_params.dims.cols - 1) * 6 * 3;

    OutputData output_data;
    output_data.points_ptr = new float[new_data_size];
    output_data.normals_ptr = new float[new_data_size];
    output_data.mean_height_ptr = new float[new_data_size / 3];

    const size_t num_lines =
        (input_params.dims.rows - 1U) * input_params.dims.cols + input_params.dims.rows * (input_params.dims.cols - 1U);
    const size_t lines_data_size = num_lines * 2U * 3U;
    output_data.lines_ptr = new float[lines_data_size];

    size_t idx = 0U, mean_height_idx = 0U, idx_line = 0U;

    for (size_t r = 0U; r < (input_params.dims.rows - 1U); r++)
    {
        for (size_t c = 0U; c < (input_params.dims.cols - 1U); c++)
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

            const Point3<T> p0{x(r, c), y(r, c), z(r, c)};
            const Point3<T> p1{x(r + 1, c), y(r + 1, c), z(r + 1, c)};
            const Point3<T> p2{x(r + 1, c + 1), y(r + 1, c + 1), z(r + 1, c + 1)};

            const Plane<T> plane{planeFromThreePoints<T>(p0, p1, p2)};
            const Vec3<T> normal_vec{plane.a, plane.b, plane.c};
            const Vec3<T> normalized_normal_vec{normal_vec.normalized()};

            // Vertices
            output_data.points_ptr[idx0_x] = x(r, c);
            output_data.points_ptr[idx1_x] = x(r + 1, c);
            output_data.points_ptr[idx2_x] = x(r + 1, c + 1);

            output_data.points_ptr[idx3_x] = x(r, c);
            output_data.points_ptr[idx4_x] = x(r, c + 1);
            output_data.points_ptr[idx5_x] = x(r + 1, c + 1);

            output_data.points_ptr[idx0_y] = y(r, c);
            output_data.points_ptr[idx1_y] = y(r + 1, c);
            output_data.points_ptr[idx2_y] = y(r + 1, c + 1);

            output_data.points_ptr[idx3_y] = y(r, c);
            output_data.points_ptr[idx4_y] = y(r, c + 1);
            output_data.points_ptr[idx5_y] = y(r + 1, c + 1);

            output_data.points_ptr[idx0_z] = z(r, c);
            output_data.points_ptr[idx1_z] = z(r + 1, c);
            output_data.points_ptr[idx2_z] = z(r + 1, c + 1);

            output_data.points_ptr[idx3_z] = z(r, c);
            output_data.points_ptr[idx4_z] = z(r, c + 1);
            output_data.points_ptr[idx5_z] = z(r + 1, c + 1);

            // Normals
            output_data.normals_ptr[idx0_x] = normalized_normal_vec.x;
            output_data.normals_ptr[idx1_y] = normalized_normal_vec.y;
            output_data.normals_ptr[idx2_z] = normalized_normal_vec.z;

            output_data.normals_ptr[idx3_x] = normalized_normal_vec.x;
            output_data.normals_ptr[idx4_y] = normalized_normal_vec.y;
            output_data.normals_ptr[idx5_z] = normalized_normal_vec.z;

            output_data.normals_ptr[idx0_y] = normalized_normal_vec.x;
            output_data.normals_ptr[idx1_y] = normalized_normal_vec.y;
            output_data.normals_ptr[idx2_z] = normalized_normal_vec.z;

            output_data.normals_ptr[idx3_y] = normalized_normal_vec.x;
            output_data.normals_ptr[idx4_y] = normalized_normal_vec.y;
            output_data.normals_ptr[idx5_z] = normalized_normal_vec.z;

            output_data.normals_ptr[idx0_z] = normalized_normal_vec.x;
            output_data.normals_ptr[idx1_y] = normalized_normal_vec.y;
            output_data.normals_ptr[idx2_z] = normalized_normal_vec.z;

            output_data.normals_ptr[idx3_z] = normalized_normal_vec.x;
            output_data.normals_ptr[idx4_y] = normalized_normal_vec.y;
            output_data.normals_ptr[idx5_z] = normalized_normal_vec.z;

            const float z_m = (z(r, c) + z(r + 1, c) + z(r, c + 1) + z(r + 1, c + 1)) * 0.25f;

            // Mean height
            output_data.mean_height_ptr[mean_height_idx] = z_m;
            output_data.mean_height_ptr[mean_height_idx + 1] = z_m;
            output_data.mean_height_ptr[mean_height_idx + 2] = z_m;
            output_data.mean_height_ptr[mean_height_idx + 3] = z_m;
            output_data.mean_height_ptr[mean_height_idx + 4] = z_m;
            output_data.mean_height_ptr[mean_height_idx + 5] = z_m;

            mean_height_idx += 6;

            output_data.lines_ptr[idx_line] = x(r, c);
            output_data.lines_ptr[idx_line + 1] = y(r, c);
            output_data.lines_ptr[idx_line + 2] = z(r, c);

            output_data.lines_ptr[idx_line + 3] = x(r, c + 1);
            output_data.lines_ptr[idx_line + 4] = y(r, c + 1);
            output_data.lines_ptr[idx_line + 5] = z(r, c + 1);

            output_data.lines_ptr[idx_line + 6] = x(r, c);
            output_data.lines_ptr[idx_line + 7] = y(r, c);
            output_data.lines_ptr[idx_line + 8] = z(r, c);

            output_data.lines_ptr[idx_line + 9] = x(r + 1, c);
            output_data.lines_ptr[idx_line + 10] = y(r + 1, c);
            output_data.lines_ptr[idx_line + 11] = z(r + 1, c);

            idx_line += 12;
        }
    }

    const size_t final_row_idx = input_params.dims.rows - 1U;

    for (size_t c = 0U; c < (input_params.dims.cols - 1U); c++)
    {
        output_data.lines_ptr[idx_line] = x(final_row_idx, c);
        output_data.lines_ptr[idx_line + 1] = y(final_row_idx, c);
        output_data.lines_ptr[idx_line + 2] = z(final_row_idx, c);

        output_data.lines_ptr[idx_line + 3] = x(final_row_idx, c + 1);
        output_data.lines_ptr[idx_line + 4] = y(final_row_idx, c + 1);
        output_data.lines_ptr[idx_line + 5] = z(final_row_idx, c + 1);

        idx_line += 6;
    }

    const size_t final_col_idx = input_params.dims.cols - 1U;

    for (size_t r = 0U; r < (input_params.dims.rows - 1U); r++)
    {
        output_data.lines_ptr[idx_line] = x(r, final_col_idx);
        output_data.lines_ptr[idx_line + 1] = y(r, final_col_idx);
        output_data.lines_ptr[idx_line + 2] = z(r, final_col_idx);

        output_data.lines_ptr[idx_line + 3] = x(r + 1, final_col_idx);
        output_data.lines_ptr[idx_line + 4] = y(r + 1, final_col_idx);
        output_data.lines_ptr[idx_line + 5] = z(r + 1, final_col_idx);

        idx_line += 6;
    }

    if (input_params.has_color)
    {
        const MatrixConstView<RGB888> colors{
            reinterpret_cast<const RGB888*>(&(input_data[3 * input_params.num_bytes_for_one_vec])),
            input_params.dims.rows,
            input_params.dims.cols};

        output_data.color_data = new float[new_data_size];

        size_t idx = 0;
        for (size_t r = 0; r < (input_params.dims.rows - 1); r++)
        {
            for (size_t c = 0; c < (input_params.dims.cols - 1); c++)
            {
                output_data.color_data[idx] = static_cast<float>(colors(r, c).red) / 255.0f;
                output_data.color_data[idx + 1] = static_cast<float>(colors(r, c).green) / 255.0f;
                output_data.color_data[idx + 2] = static_cast<float>(colors(r, c).blue) / 255.0f;

                output_data.color_data[idx + 3] = static_cast<float>(colors(r + 1, c).red) / 255.0f;
                output_data.color_data[idx + 4] = static_cast<float>(colors(r + 1, c).green) / 255.0f;
                output_data.color_data[idx + 5] = static_cast<float>(colors(r + 1, c).blue) / 255.0f;

                output_data.color_data[idx + 6] = static_cast<float>(colors(r + 1, c + 1).red) / 255.0f;
                output_data.color_data[idx + 7] = static_cast<float>(colors(r + 1, c + 1).green) / 255.0f;
                output_data.color_data[idx + 8] = static_cast<float>(colors(r + 1, c + 1).blue) / 255.0f;

                output_data.color_data[idx + 9] = static_cast<float>(colors(r, c).red) / 255.0f;
                output_data.color_data[idx + 10] = static_cast<float>(colors(r, c).green) / 255.0f;
                output_data.color_data[idx + 11] = static_cast<float>(colors(r, c).blue) / 255.0f;

                output_data.color_data[idx + 12] = static_cast<float>(colors(r, c + 1).red) / 255.0f;
                output_data.color_data[idx + 13] = static_cast<float>(colors(r, c + 1).green) / 255.0f;
                output_data.color_data[idx + 14] = static_cast<float>(colors(r, c + 1).blue) / 255.0f;

                output_data.color_data[idx + 15] = static_cast<float>(colors(r + 1, c + 1).red) / 255.0f;
                output_data.color_data[idx + 16] = static_cast<float>(colors(r + 1, c + 1).green) / 255.0f;
                output_data.color_data[idx + 17] = static_cast<float>(colors(r + 1, c + 1).blue) / 255.0f;

                idx = idx + 18;
            }
        }
    }

    return output_data;
}
}  // namespace
