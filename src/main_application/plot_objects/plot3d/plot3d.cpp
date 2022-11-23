#include "main_application/plot_objects/plot3d/plot3d.h"

#include "outer_converter.h"

namespace
{
struct OutputData
{
    float* p0;
    float* p1;
    float* p2;
    int32_t* idx_data;
    size_t num_points;
};

struct InputParams
{
    size_t num_elements;
    size_t num_bytes_per_element;
    size_t num_bytes_for_one_vec;

    InputParams() = default;
    InputParams(const size_t num_elements_, const size_t num_bytes_per_element_, const size_t num_bytes_for_one_vec_)
        : num_elements{num_elements_},
          num_bytes_per_element{num_bytes_per_element_},
          num_bytes_for_one_vec{num_bytes_for_one_vec_}
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

Plot3D::Plot3D(std::unique_ptr<const ReceivedData> received_data,
               const CommunicationHeader& hdr,
               const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::PLOT3)
    {
        throw std::runtime_error("Invalid function type for Plot3D!");
    }

    const InputParams input_params{num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_};
    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    num_points_ = output_data.num_points;

    // p0
    glGenBuffers(1, &p0_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p0_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points_ * 3, output_data.p0, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // p1
    glGenBuffers(1, &p1_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points_ * 3, output_data.p1, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // p2
    glGenBuffers(1, &p2_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points_ * 3, output_data.p2, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    // Idx
    glGenBuffers(1, &idx_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * num_points_, output_data.idx_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glVertexAttribIPointer(3, 1, GL_INT, 0, 0);

    delete[] output_data.p0;
    delete[] output_data.p1;
    delete[] output_data.p2;
    delete[] output_data.idx_data;
}

void Plot3D::findMinMax()
{
    std::tie<Vec3d, Vec3d>(min_vec, max_vec) =
        findMinMaxFromThreeVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);
}

void Plot3D::render()
{
    shader_collection_.plot_3d_shader.use();
    glUniform1f(glGetUniformLocation(shader_collection_.plot_3d_shader.programId(), "half_line_width"),
                line_width_ / 1200.0f);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_points_);
    glBindVertexArray(0);
    shader_collection_.basic_plot_shader.use();
}

Plot3D::~Plot3D() {}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    const size_t num_segments = input_params.num_elements - 1U;
    const size_t num_triangles = num_segments * 2U + (num_segments - 1U) * 2U;
    const size_t num_points = num_triangles * 3U;

    OutputData output_data;
    output_data.num_points = num_points;
    output_data.p0 = new float[3 * num_points];
    output_data.p1 = new float[3 * num_points];
    output_data.p2 = new float[3 * num_points];
    output_data.idx_data = new int32_t[num_points];

    std::memset(output_data.p0, 0, 3 * num_points * sizeof(float));
    std::memset(output_data.p1, 0, 3 * num_points * sizeof(float));
    std::memset(output_data.p2, 0, 3 * num_points * sizeof(float));

    std::memset(output_data.idx_data, 0, num_points * sizeof(int32_t));

    struct Points
    {
        Vec3<float> p0;
        Vec3<float> p1;
        Vec3<float> p2;
    };
    std::vector<Points> pts;
    pts.resize(input_params.num_elements);

    const T* const input_data_dt = reinterpret_cast<const T* const>(input_data);

    {
        // First segment
        const T p1x = input_data_dt[0];
        const T p1y = input_data_dt[input_params.num_elements];
        const T p1z = input_data_dt[input_params.num_elements * 2];

        const T p2x = input_data_dt[1];
        const T p2y = input_data_dt[input_params.num_elements + 1];
        const T p2z = input_data_dt[input_params.num_elements * 2 + 1];

        const T vx = p2x - p1x;
        const T vy = p2y - p1y;
        const T vz = p2z - p1z;

        pts[0].p0.x = p1x - vx;
        pts[0].p0.y = p1y - vy;
        pts[0].p0.y = p1z - vz;

        pts[0].p1.x = p1x;
        pts[0].p1.y = p1y;
        pts[0].p1.z = p1z;

        pts[0].p2.x = p2x;
        pts[0].p2.y = p2y;
        pts[0].p2.z = p2z;
    }

    {
        // Last segment
        const T p0x = input_data_dt[input_params.num_elements - 2];
        const T p0y = input_data_dt[2 * input_params.num_elements - 2];
        const T p0z = input_data_dt[3 * input_params.num_elements - 2];

        const T p1x = input_data_dt[input_params.num_elements - 1];
        const T p1y = input_data_dt[2 * input_params.num_elements - 1];
        const T p1z = input_data_dt[3 * input_params.num_elements - 1];

        const T vx = p1x - p0x;
        const T vy = p1y - p0y;
        const T vz = p1z - p0z;

        pts[input_params.num_elements - 1].p0.x = p0x;
        pts[input_params.num_elements - 1].p0.y = p0y;
        pts[input_params.num_elements - 1].p0.z = p0z;

        pts[input_params.num_elements - 1].p1.x = p1x;
        pts[input_params.num_elements - 1].p1.y = p1y;
        pts[input_params.num_elements - 1].p1.z = p1z;

        pts[input_params.num_elements - 1].p2.x = p1x + vx;
        pts[input_params.num_elements - 1].p2.y = p1y + vy;
        pts[input_params.num_elements - 1].p2.z = p1z + vz;
    }

    // Segments inbetween
    for (size_t k = 1; k < (input_params.num_elements - 1); k++)
    {
        const T p0x = input_data_dt[k - 1];
        const T p0y = input_data_dt[input_params.num_elements + k - 1];
        const T p0z = input_data_dt[2 * input_params.num_elements + k - 1];

        const T p1x = input_data_dt[k];
        const T p1y = input_data_dt[input_params.num_elements + k];
        const T p1z = input_data_dt[2 * input_params.num_elements + k];

        const T p2x = input_data_dt[k + 1];
        const T p2y = input_data_dt[input_params.num_elements + k + 1];
        const T p2z = input_data_dt[2 * input_params.num_elements + k + 1];

        pts[k].p0.x = p0x;
        pts[k].p0.y = p0y;
        pts[k].p0.z = p0z;

        pts[k].p1.x = p1x;
        pts[k].p1.y = p1y;
        pts[k].p1.z = p1z;

        pts[k].p2.x = p2x;
        pts[k].p2.y = p2y;
        pts[k].p2.z = p2z;
    }

    size_t idx = 0;
    size_t idx_idx = 0;

    for (size_t k = 1; k < (input_params.num_elements - 1U); k++)
    {
        const Points& pt_1 = pts[k - 1];
        const Points& pt = pts[k];

        // 1st triangle
        output_data.p0[idx] = pt_1.p0.x;
        output_data.p0[idx + 1] = pt_1.p0.y;
        output_data.p0[idx + 2] = pt_1.p0.z;

        output_data.p0[idx + 3] = pt.p0.x;
        output_data.p0[idx + 4] = pt.p0.y;
        output_data.p0[idx + 5] = pt.p0.z;

        output_data.p0[idx + 6] = pt.p0.x;
        output_data.p0[idx + 7] = pt.p0.y;
        output_data.p0[idx + 8] = pt.p0.z;

        // 2nd triangle
        output_data.p0[idx + 9] = pt_1.p0.x;
        output_data.p0[idx + 10] = pt_1.p0.y;
        output_data.p0[idx + 11] = pt_1.p0.z;

        output_data.p0[idx + 12] = pt.p0.x;
        output_data.p0[idx + 13] = pt.p0.y;
        output_data.p0[idx + 14] = pt.p0.z;

        output_data.p0[idx + 15] = pt_1.p0.x;
        output_data.p0[idx + 16] = pt_1.p0.y;
        output_data.p0[idx + 17] = pt_1.p0.z;

        // 3nd triangle
        output_data.p0[idx + 18] = pt.p0.x;
        output_data.p0[idx + 19] = pt.p0.y;
        output_data.p0[idx + 20] = pt.p0.z;

        output_data.p0[idx + 21] = pt.p0.x;
        output_data.p0[idx + 22] = pt.p0.y;
        output_data.p0[idx + 23] = pt.p0.z;

        output_data.p0[idx + 24] = pt.p0.x;
        output_data.p0[idx + 25] = pt.p0.y;
        output_data.p0[idx + 26] = pt.p0.z;

        // 4th triangle
        output_data.p0[idx + 27] = pt.p0.x;
        output_data.p0[idx + 28] = pt.p0.y;
        output_data.p0[idx + 29] = pt.p0.z;

        output_data.p0[idx + 30] = pt.p0.x;
        output_data.p0[idx + 31] = pt.p0.y;
        output_data.p0[idx + 32] = pt.p0.z;

        output_data.p0[idx + 33] = pt.p0.x;
        output_data.p0[idx + 34] = pt.p0.y;
        output_data.p0[idx + 35] = pt.p0.z;

        // 1st triangle
        output_data.p1[idx] = pt_1.p1.x;
        output_data.p1[idx + 1] = pt_1.p1.y;
        output_data.p1[idx + 2] = pt_1.p1.z;

        output_data.p1[idx + 3] = pt.p1.x;
        output_data.p1[idx + 4] = pt.p1.y;
        output_data.p1[idx + 5] = pt.p1.z;

        output_data.p1[idx + 6] = pt.p1.x;
        output_data.p1[idx + 7] = pt.p1.y;
        output_data.p1[idx + 8] = pt.p1.z;

        // 2nd triangle
        output_data.p1[idx + 9] = pt_1.p1.x;
        output_data.p1[idx + 10] = pt_1.p1.y;
        output_data.p1[idx + 11] = pt_1.p1.z;

        output_data.p1[idx + 12] = pt.p1.x;
        output_data.p1[idx + 13] = pt.p1.y;
        output_data.p1[idx + 14] = pt.p1.z;

        output_data.p1[idx + 15] = pt_1.p1.x;
        output_data.p1[idx + 16] = pt_1.p1.y;
        output_data.p1[idx + 17] = pt_1.p1.z;

        // 3nd triangle
        output_data.p1[idx + 18] = pt.p1.x;
        output_data.p1[idx + 19] = pt.p1.y;
        output_data.p1[idx + 20] = pt.p1.z;

        output_data.p1[idx + 21] = pt.p1.x;
        output_data.p1[idx + 22] = pt.p1.y;
        output_data.p1[idx + 23] = pt.p1.z;

        output_data.p1[idx + 24] = pt.p1.x;
        output_data.p1[idx + 25] = pt.p1.y;
        output_data.p1[idx + 26] = pt.p1.z;

        // 4th triangle
        output_data.p1[idx + 27] = pt.p1.x;
        output_data.p1[idx + 28] = pt.p1.y;
        output_data.p1[idx + 29] = pt.p1.z;

        output_data.p1[idx + 30] = pt.p1.x;
        output_data.p1[idx + 31] = pt.p1.y;
        output_data.p1[idx + 32] = pt.p1.z;

        output_data.p1[idx + 33] = pt.p1.x;
        output_data.p1[idx + 34] = pt.p1.y;
        output_data.p1[idx + 35] = pt.p1.z;

        // 1st triangle
        output_data.p2[idx] = pt_1.p2.x;
        output_data.p2[idx + 1] = pt_1.p2.y;
        output_data.p2[idx + 2] = pt_1.p2.z;

        output_data.p2[idx + 3] = pt.p2.x;
        output_data.p2[idx + 4] = pt.p2.y;
        output_data.p2[idx + 5] = pt.p2.z;

        output_data.p2[idx + 6] = pt.p2.x;
        output_data.p2[idx + 7] = pt.p2.y;
        output_data.p2[idx + 8] = pt.p2.z;

        // 2nd triangle
        output_data.p2[idx + 9] = pt_1.p2.x;
        output_data.p2[idx + 10] = pt_1.p2.y;
        output_data.p2[idx + 11] = pt_1.p2.z;

        output_data.p2[idx + 12] = pt.p2.x;
        output_data.p2[idx + 13] = pt.p2.y;
        output_data.p2[idx + 14] = pt.p2.z;

        output_data.p2[idx + 15] = pt_1.p2.x;
        output_data.p2[idx + 16] = pt_1.p2.y;
        output_data.p2[idx + 17] = pt_1.p2.z;

        // 3nd triangle
        output_data.p2[idx + 18] = pt.p2.x;
        output_data.p2[idx + 19] = pt.p2.y;
        output_data.p2[idx + 20] = pt.p2.z;

        output_data.p2[idx + 21] = pt.p2.x;
        output_data.p2[idx + 22] = pt.p2.y;
        output_data.p2[idx + 23] = pt.p2.z;

        output_data.p2[idx + 24] = pt.p2.x;
        output_data.p2[idx + 25] = pt.p2.y;
        output_data.p2[idx + 26] = pt.p2.z;

        // 4th triangle
        output_data.p2[idx + 27] = pt.p2.x;
        output_data.p2[idx + 28] = pt.p2.y;
        output_data.p2[idx + 29] = pt.p2.z;

        output_data.p2[idx + 30] = pt.p2.x;
        output_data.p2[idx + 31] = pt.p2.y;
        output_data.p2[idx + 32] = pt.p2.z;

        output_data.p2[idx + 33] = pt.p2.x;
        output_data.p2[idx + 34] = pt.p2.y;
        output_data.p2[idx + 35] = pt.p2.z;

        output_data.idx_data[idx_idx] = 0;
        output_data.idx_data[idx_idx + 1] = 1;
        output_data.idx_data[idx_idx + 2] = 2;
        output_data.idx_data[idx_idx + 3] = 3;
        output_data.idx_data[idx_idx + 4] = 4;
        output_data.idx_data[idx_idx + 5] = 5;
        output_data.idx_data[idx_idx + 6] = 6;
        output_data.idx_data[idx_idx + 7] = 7;
        output_data.idx_data[idx_idx + 8] = 8;
        output_data.idx_data[idx_idx + 9] = 9;
        output_data.idx_data[idx_idx + 10] = 10;
        output_data.idx_data[idx_idx + 11] = 11;

        idx += 36;
        idx_idx += 12;
    }

    // Last segment will not have the two "Inbetween triangles"
    const Points& pt_1 = pts[input_params.num_elements - 2];
    const Points& pt = pts[input_params.num_elements - 1];

    output_data.p0[idx] = pt_1.p0.x;
    output_data.p0[idx + 1] = pt_1.p0.y;
    output_data.p0[idx + 2] = pt_1.p0.z;

    output_data.p0[idx + 3] = pt.p0.x;
    output_data.p0[idx + 4] = pt.p0.y;
    output_data.p0[idx + 5] = pt.p0.z;

    output_data.p0[idx + 6] = pt.p0.x;
    output_data.p0[idx + 7] = pt.p0.y;
    output_data.p0[idx + 8] = pt.p0.z;

    output_data.p0[idx + 9] = pt_1.p0.x;
    output_data.p0[idx + 10] = pt_1.p0.y;
    output_data.p0[idx + 11] = pt_1.p0.z;

    output_data.p0[idx + 12] = pt.p0.x;
    output_data.p0[idx + 13] = pt.p0.y;
    output_data.p0[idx + 14] = pt.p0.z;

    output_data.p0[idx + 15] = pt_1.p0.x;
    output_data.p0[idx + 16] = pt_1.p0.y;
    output_data.p0[idx + 17] = pt_1.p0.z;

    output_data.p1[idx] = pt_1.p1.x;
    output_data.p1[idx + 1] = pt_1.p1.y;
    output_data.p1[idx + 2] = pt_1.p1.z;

    output_data.p1[idx + 3] = pt.p1.x;
    output_data.p1[idx + 4] = pt.p1.y;
    output_data.p1[idx + 5] = pt.p1.z;

    output_data.p1[idx + 6] = pt.p1.x;
    output_data.p1[idx + 7] = pt.p1.y;
    output_data.p1[idx + 8] = pt.p1.z;

    output_data.p1[idx + 9] = pt_1.p1.x;
    output_data.p1[idx + 10] = pt_1.p1.y;
    output_data.p1[idx + 11] = pt_1.p1.z;

    output_data.p1[idx + 12] = pt.p1.x;
    output_data.p1[idx + 13] = pt.p1.y;
    output_data.p1[idx + 14] = pt.p1.z;

    output_data.p1[idx + 15] = pt_1.p1.x;
    output_data.p1[idx + 16] = pt_1.p1.y;
    output_data.p1[idx + 17] = pt_1.p1.z;

    output_data.p2[idx] = pt_1.p2.x;
    output_data.p2[idx + 1] = pt_1.p2.y;
    output_data.p2[idx + 2] = pt_1.p2.z;

    output_data.p2[idx + 3] = pt.p2.x;
    output_data.p2[idx + 4] = pt.p2.y;
    output_data.p2[idx + 5] = pt.p2.z;

    output_data.p2[idx + 6] = pt.p2.x;
    output_data.p2[idx + 7] = pt.p2.y;
    output_data.p2[idx + 8] = pt.p2.z;

    output_data.p2[idx + 9] = pt_1.p2.x;
    output_data.p2[idx + 10] = pt_1.p2.y;
    output_data.p2[idx + 11] = pt_1.p2.z;

    output_data.p2[idx + 12] = pt.p2.x;
    output_data.p2[idx + 13] = pt.p2.y;
    output_data.p2[idx + 14] = pt.p2.z;

    output_data.p2[idx + 15] = pt_1.p2.x;
    output_data.p2[idx + 16] = pt_1.p2.y;
    output_data.p2[idx + 17] = pt_1.p2.z;

    output_data.idx_data[idx_idx] = 0;
    output_data.idx_data[idx_idx + 1] = 1;
    output_data.idx_data[idx_idx + 2] = 2;
    output_data.idx_data[idx_idx + 3] = 3;
    output_data.idx_data[idx_idx + 4] = 4;
    output_data.idx_data[idx_idx + 5] = 5;

    return output_data;
}
}  // namespace
