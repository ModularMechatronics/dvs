#include "main_application/plot_objects/plot2d/plot2d.h"

#include <functional>

#include "outer_converter.h"

namespace
{

struct OutputData
{
    float* p0;
    float* p1;
    float* p2;
    float* length_along;
    int32_t* idx_data;
    float* color_data;
    size_t num_points;
};

struct InputParams
{
    size_t num_elements;
    size_t num_bytes_per_element;
    size_t num_bytes_for_one_vec;
    bool has_color;

    InputParams() = default;
    InputParams(const size_t num_elements_,
                const size_t num_bytes_per_element_,
                const size_t num_bytes_for_one_vec_,
                const bool has_color_)
        : num_elements{num_elements_},
          num_bytes_per_element{num_bytes_per_element_},
          num_bytes_for_one_vec{num_bytes_for_one_vec_},
          has_color{has_color_}
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

Plot2D::Plot2D(std::unique_ptr<const ReceivedData> received_data,
               const CommunicationHeader& hdr,
               const Properties& props,
               const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, props, shader_collection),
      vertex_buffer2_{OGLPrimitiveType::TRIANGLES}
{
    if (type_ != Function::PLOT2)
    {
        throw std::runtime_error("Invalid function type for Plot2D!");
    }
    std::cout << "Creating new plot2" << std::endl;
    const InputParams input_params{num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_, has_color_};

    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    num_points_ = output_data.num_points;

    if (is_dashed_)
    {
        if (line_style_.data == internal::LineStyleType::DASHED)
        {
            gap_size_ = 3.0f;
            dash_size_ = 3.0f;
        }
        else if (line_style_.data == internal::LineStyleType::DOTTED)
        {
            gap_size_ = 6.0f;
            dash_size_ = 3.0f;
        }
        else if (line_style_.data == internal::LineStyleType::LONG_DASHED)
        {
            gap_size_ = 2.0f;
            dash_size_ = 6.0f;
        }
    }

    vertex_buffer2_.addBuffer(output_data.p0, num_points_, 2, usage_);
    vertex_buffer2_.addBuffer(output_data.p1, num_points_, 2, usage_);
    vertex_buffer2_.addBuffer(output_data.p2, num_points_, 2, usage_);

    vertex_buffer2_.addBuffer(output_data.idx_data, num_points_, 1, usage_);
    vertex_buffer2_.addBuffer(output_data.length_along, num_points_, 1, usage_);

    if (has_color_)
    {
        vertex_buffer2_.addBuffer(output_data.color_data, num_points_, 3);

        delete[] output_data.color_data;
    }

    delete[] output_data.p0;
    delete[] output_data.p1;
    delete[] output_data.p2;
    delete[] output_data.length_along;
    delete[] output_data.idx_data;
}

void Plot2D::findMinMax()
{
    Vec2d min_vec_2d, max_vec_2d;
    std::tie<Vec2d, Vec2d>(min_vec_2d, max_vec_2d) =
        findMinMaxFromTwoVectors(data_ptr_, num_elements_, num_bytes_for_one_vec_, data_type_);

    min_vec.x = min_vec_2d.x;
    min_vec.y = min_vec_2d.y;
    min_vec.z = -1.0;

    max_vec.x = max_vec_2d.x;
    max_vec.y = max_vec_2d.y;
    max_vec.z = 1.0;
}

void Plot2D::render()
{
    glEnable(GL_BLEND);
    shader_collection_.plot_2d_shader.use();
    preRender(shader_collection_.plot_2d_shader);

    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "half_line_width"),
                line_width_ / 600.0f);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "z_offset"), z_offset_);
    glUniform1i(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "use_dash"), 0);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "alpha"), alpha_);

    if (has_color_)
    {
        glUniform1i(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "has_color_vec"),
                    static_cast<int>(1));
    }
    else
    {
        glUniform1i(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "has_color_vec"),
                    static_cast<int>(0));
    }

    vertex_buffer2_.render(num_points_);

    shader_collection_.basic_plot_shader.use();
    glDisable(GL_BLEND);
}

void Plot2D::updateWithNewData(std::unique_ptr<const ReceivedData> received_data,
                               const CommunicationHeader& hdr,
                               const Properties& props)
{
    throwIfNotUpdateable();

    initialize(std::move(received_data), hdr, props);

    const InputParams input_params{num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_, has_color_};

    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

    num_points_ = output_data.num_points;

    vertex_buffer2_.updateBufferData(0, output_data.p0, num_points_, 2);
    vertex_buffer2_.updateBufferData(1, output_data.p1, num_points_, 2);
    vertex_buffer2_.updateBufferData(2, output_data.p2, num_points_, 2);
    vertex_buffer2_.updateBufferData(3, output_data.idx_data, num_points_, 1);
    vertex_buffer2_.updateBufferData(4, output_data.length_along, num_points_, 1);

    delete[] output_data.p0;
    delete[] output_data.p1;
    delete[] output_data.p2;
    delete[] output_data.length_along;
    delete[] output_data.idx_data;
}

Plot2D::~Plot2D() {}

LegendProperties Plot2D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

namespace
{
template <typename T> OutputData convertData(const uint8_t* const input_data, const InputParams& input_params)
{
    const size_t num_segments = input_params.num_elements - 1U;
    const size_t num_triangles = num_segments * 2U + (num_segments - 1U) * 2U;
    const size_t num_points = num_triangles * 3U;

    OutputData output_data;
    output_data.num_points = num_points;
    output_data.p0 = new float[2 * num_points];
    output_data.p1 = new float[2 * num_points];
    output_data.p2 = new float[2 * num_points];
    output_data.length_along = new float[num_points];
    output_data.idx_data = new int32_t[num_points];

    float* length_along_tmp = new float[input_params.num_elements];

    std::memset(output_data.p0, 0, 2 * num_points * sizeof(float));
    std::memset(output_data.p1, 0, 2 * num_points * sizeof(float));
    std::memset(output_data.p2, 0, 2 * num_points * sizeof(float));

    std::memset(output_data.idx_data, 0, num_points * sizeof(int32_t));

    struct Points
    {
        Vec2<float> p0;
        Vec2<float> p1;
        Vec2<float> p2;
    };
    std::vector<Points> pts;
    pts.resize(input_params.num_elements);

    const T* const input_data_dt = reinterpret_cast<const T* const>(input_data);

    length_along_tmp[0] = 0.0f;

    for (size_t k = 1; k < input_params.num_elements; k++)
    {
        // First segment
        const T p0x = input_data_dt[k - 1];
        const T p0y = input_data_dt[input_params.num_elements + k - 1];

        const T p1x = input_data_dt[k];
        const T p1y = input_data_dt[input_params.num_elements + k];

        const float x = static_cast<float>(p0x - p1x);
        const float y = static_cast<float>(p0y - p1y);

        const float d = std::sqrt(x * x + y * y);

        length_along_tmp[k] = length_along_tmp[k - 1U] + d;
    }

    {
        // First segment
        const T p1x = input_data_dt[0];
        const T p1y = input_data_dt[input_params.num_elements];

        const T p2x = input_data_dt[1];
        const T p2y = input_data_dt[input_params.num_elements + 1];

        const T vx = p2x - p1x;
        const T vy = p2y - p1y;

        pts[0].p0.x = p1x - vx;
        pts[0].p0.y = p1y - vy;

        pts[0].p1.x = p1x;
        pts[0].p1.y = p1y;

        pts[0].p2.x = p2x;
        pts[0].p2.y = p2y;
    }

    {
        // Last segment
        const T p0x = input_data_dt[input_params.num_elements - 2];
        const T p0y = input_data_dt[input_params.num_elements + input_params.num_elements - 2];

        const T p1x = input_data_dt[input_params.num_elements - 1];
        const T p1y = input_data_dt[input_params.num_elements + input_params.num_elements - 1];

        const T vx = p1x - p0x;
        const T vy = p1y - p0y;

        pts[input_params.num_elements - 1].p0.x = p0x;
        pts[input_params.num_elements - 1].p0.y = p0y;

        pts[input_params.num_elements - 1].p1.x = p1x;
        pts[input_params.num_elements - 1].p1.y = p1y;

        pts[input_params.num_elements - 1].p2.x = p1x + vx;
        pts[input_params.num_elements - 1].p2.y = p1y + vy;
    }

    // Segments inbetween
    for (size_t k = 1; k < (input_params.num_elements - 1); k++)
    {
        const T p0x = input_data_dt[k - 1];
        const T p0y = input_data_dt[input_params.num_elements + k - 1];

        const T p1x = input_data_dt[k];
        const T p1y = input_data_dt[input_params.num_elements + k];

        const T p2x = input_data_dt[k + 1];
        const T p2y = input_data_dt[input_params.num_elements + k + 1];

        pts[k].p0.x = p0x;
        pts[k].p0.y = p0y;

        pts[k].p1.x = p1x;
        pts[k].p1.y = p1y;

        pts[k].p2.x = p2x;
        pts[k].p2.y = p2y;
    }

    size_t idx = 0;
    size_t idx_idx = 0;
    size_t length_along_idx = 1;

    for (size_t k = 1; k < (input_params.num_elements - 1U); k++)
    {
        const Points& pt_1 = pts[k - 1];
        const Points& pt = pts[k];

        // 1st triangle
        output_data.p0[idx] = pt_1.p0.x;
        output_data.p0[idx + 1] = pt_1.p0.y;

        output_data.p0[idx + 2] = pt.p0.x;
        output_data.p0[idx + 3] = pt.p0.y;

        output_data.p0[idx + 4] = pt.p0.x;
        output_data.p0[idx + 5] = pt.p0.y;

        // 2nd triangle
        output_data.p0[idx + 6] = pt_1.p0.x;
        output_data.p0[idx + 7] = pt_1.p0.y;

        output_data.p0[idx + 8] = pt.p0.x;
        output_data.p0[idx + 9] = pt.p0.y;

        output_data.p0[idx + 10] = pt_1.p0.x;
        output_data.p0[idx + 11] = pt_1.p0.y;

        // 3nd triangle
        output_data.p0[idx + 12] = pt.p0.x;
        output_data.p0[idx + 13] = pt.p0.y;

        output_data.p0[idx + 14] = pt.p0.x;
        output_data.p0[idx + 15] = pt.p0.y;

        output_data.p0[idx + 16] = pt.p0.x;
        output_data.p0[idx + 17] = pt.p0.y;

        // 4th triangle
        output_data.p0[idx + 18] = pt.p0.x;
        output_data.p0[idx + 19] = pt.p0.y;

        output_data.p0[idx + 20] = pt.p0.x;
        output_data.p0[idx + 21] = pt.p0.y;

        output_data.p0[idx + 22] = pt.p0.x;
        output_data.p0[idx + 23] = pt.p0.y;

        // 1st triangle
        output_data.p1[idx] = pt_1.p1.x;
        output_data.p1[idx + 1] = pt_1.p1.y;

        output_data.p1[idx + 2] = pt.p1.x;
        output_data.p1[idx + 3] = pt.p1.y;

        output_data.p1[idx + 4] = pt.p1.x;
        output_data.p1[idx + 5] = pt.p1.y;

        // 2nd triangle
        output_data.p1[idx + 6] = pt_1.p1.x;
        output_data.p1[idx + 7] = pt_1.p1.y;

        output_data.p1[idx + 8] = pt.p1.x;
        output_data.p1[idx + 9] = pt.p1.y;

        output_data.p1[idx + 10] = pt_1.p1.x;
        output_data.p1[idx + 11] = pt_1.p1.y;

        // 3nd triangle
        output_data.p1[idx + 12] = pt.p1.x;
        output_data.p1[idx + 13] = pt.p1.y;

        output_data.p1[idx + 14] = pt.p1.x;
        output_data.p1[idx + 15] = pt.p1.y;

        output_data.p1[idx + 16] = pt.p1.x;
        output_data.p1[idx + 17] = pt.p1.y;

        // 4th triangle
        output_data.p1[idx + 18] = pt.p1.x;
        output_data.p1[idx + 19] = pt.p1.y;

        output_data.p1[idx + 20] = pt.p1.x;
        output_data.p1[idx + 21] = pt.p1.y;

        output_data.p1[idx + 22] = pt.p1.x;
        output_data.p1[idx + 23] = pt.p1.y;

        // 1st triangle
        output_data.p2[idx] = pt_1.p2.x;
        output_data.p2[idx + 1] = pt_1.p2.y;

        output_data.p2[idx + 2] = pt.p2.x;
        output_data.p2[idx + 3] = pt.p2.y;

        output_data.p2[idx + 4] = pt.p2.x;
        output_data.p2[idx + 5] = pt.p2.y;

        // 2nd triangle
        output_data.p2[idx + 6] = pt_1.p2.x;
        output_data.p2[idx + 7] = pt_1.p2.y;

        output_data.p2[idx + 8] = pt.p2.x;
        output_data.p2[idx + 9] = pt.p2.y;

        output_data.p2[idx + 10] = pt_1.p2.x;
        output_data.p2[idx + 11] = pt_1.p2.y;

        // 3nd triangle
        output_data.p2[idx + 12] = pt.p2.x;
        output_data.p2[idx + 13] = pt.p2.y;

        output_data.p2[idx + 14] = pt.p2.x;
        output_data.p2[idx + 15] = pt.p2.y;

        output_data.p2[idx + 16] = pt.p2.x;
        output_data.p2[idx + 17] = pt.p2.y;

        // 4th triangle
        output_data.p2[idx + 18] = pt.p2.x;
        output_data.p2[idx + 19] = pt.p2.y;

        output_data.p2[idx + 20] = pt.p2.x;
        output_data.p2[idx + 21] = pt.p2.y;

        output_data.p2[idx + 22] = pt.p2.x;
        output_data.p2[idx + 23] = pt.p2.y;

        const float la_1 = length_along_tmp[k - 1];
        const float la = length_along_tmp[k];

        output_data.length_along[length_along_idx] = la_1;
        output_data.length_along[length_along_idx + 1] = la;
        output_data.length_along[length_along_idx + 2] = la;

        output_data.length_along[length_along_idx + 3] = la_1;
        output_data.length_along[length_along_idx + 4] = la_1;
        output_data.length_along[length_along_idx + 5] = la;

        output_data.length_along[length_along_idx + 6] = la;
        output_data.length_along[length_along_idx + 7] = la;
        output_data.length_along[length_along_idx + 8] = la;

        output_data.length_along[length_along_idx + 9] = la;
        output_data.length_along[length_along_idx + 10] = la;
        output_data.length_along[length_along_idx + 11] = la;

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

        idx += 24;
        idx_idx += 12;
        length_along_idx += 12;
    }

    // Last segment will not have the two "Inbetween triangles"
    const Points& pt_1 = pts[input_params.num_elements - 2];
    const Points& pt = pts[input_params.num_elements - 1];

    // 1st triangle
    output_data.p0[idx] = pt_1.p0.x;
    output_data.p0[idx + 1] = pt_1.p0.y;

    output_data.p0[idx + 2] = pt.p0.x;
    output_data.p0[idx + 3] = pt.p0.y;

    output_data.p0[idx + 4] = pt.p0.x;
    output_data.p0[idx + 5] = pt.p0.y;

    // 2nd triangle
    output_data.p0[idx + 6] = pt_1.p0.x;
    output_data.p0[idx + 7] = pt_1.p0.y;

    output_data.p0[idx + 8] = pt.p0.x;
    output_data.p0[idx + 9] = pt.p0.y;

    output_data.p0[idx + 10] = pt_1.p0.x;
    output_data.p0[idx + 11] = pt_1.p0.y;

    // 1st triangle
    output_data.p1[idx] = pt_1.p1.x;
    output_data.p1[idx + 1] = pt_1.p1.y;

    output_data.p1[idx + 2] = pt.p1.x;
    output_data.p1[idx + 3] = pt.p1.y;

    output_data.p1[idx + 4] = pt.p1.x;
    output_data.p1[idx + 5] = pt.p1.y;

    // 2nd triangle
    output_data.p1[idx + 6] = pt_1.p1.x;
    output_data.p1[idx + 7] = pt_1.p1.y;

    output_data.p1[idx + 8] = pt.p1.x;
    output_data.p1[idx + 9] = pt.p1.y;

    output_data.p1[idx + 10] = pt_1.p1.x;
    output_data.p1[idx + 11] = pt_1.p1.y;

    // 1st triangle
    output_data.p2[idx] = pt_1.p2.x;
    output_data.p2[idx + 1] = pt_1.p2.y;

    output_data.p2[idx + 2] = pt.p2.x;
    output_data.p2[idx + 3] = pt.p2.y;

    output_data.p2[idx + 4] = pt.p2.x;
    output_data.p2[idx + 5] = pt.p2.y;

    // 2nd triangle
    output_data.p2[idx + 6] = pt_1.p2.x;
    output_data.p2[idx + 7] = pt_1.p2.y;

    output_data.p2[idx + 8] = pt.p2.x;
    output_data.p2[idx + 9] = pt.p2.y;

    output_data.p2[idx + 10] = pt_1.p2.x;
    output_data.p2[idx + 11] = pt_1.p2.y;

    output_data.idx_data[idx_idx] = 0;
    output_data.idx_data[idx_idx + 1] = 1;
    output_data.idx_data[idx_idx + 2] = 2;
    output_data.idx_data[idx_idx + 3] = 3;
    output_data.idx_data[idx_idx + 4] = 4;
    output_data.idx_data[idx_idx + 5] = 5;

    const float la_1 = length_along_tmp[input_params.num_elements - 2];
    const float la = length_along_tmp[input_params.num_elements - 1];

    output_data.length_along[length_along_idx] = la_1;
    output_data.length_along[length_along_idx + 1] = la;
    output_data.length_along[length_along_idx + 2] = la;

    output_data.length_along[length_along_idx + 3] = la_1;
    output_data.length_along[length_along_idx + 4] = la_1;
    output_data.length_along[length_along_idx + 5] = la;

    delete[] length_along_tmp;

    if (input_params.has_color)
    {
        const RGB888* const input_data_rgb =
            reinterpret_cast<const RGB888* const>(input_data_dt + 2U * input_params.num_elements);
        output_data.color_data = new float[3 * num_points];

        idx = 0;

        for (size_t k = 1; k < input_params.num_elements - 1; k++)
        {
            const RGBTripletf color_k{static_cast<float>(input_data_rgb[k].red) / 255.0f,
                                      static_cast<float>(input_data_rgb[k].green) / 255.0f,
                                      static_cast<float>(input_data_rgb[k].blue) / 255.0f};

            const RGBTripletf color_k_1{static_cast<float>(input_data_rgb[k - 1].red) / 255.0f,
                                        static_cast<float>(input_data_rgb[k - 1].green) / 255.0f,
                                        static_cast<float>(input_data_rgb[k - 1].blue) / 255.0f};
            // v0
            output_data.color_data[idx] = color_k_1.red;
            output_data.color_data[idx + 1] = color_k_1.green;
            output_data.color_data[idx + 2] = color_k_1.blue;

            // v1
            output_data.color_data[idx + 3] = color_k.red;
            output_data.color_data[idx + 4] = color_k.green;
            output_data.color_data[idx + 5] = color_k.blue;

            // v2
            output_data.color_data[idx + 6] = color_k.red;
            output_data.color_data[idx + 7] = color_k.green;
            output_data.color_data[idx + 8] = color_k.blue;

            // v3
            output_data.color_data[idx + 9] = color_k_1.red;
            output_data.color_data[idx + 10] = color_k_1.green;
            output_data.color_data[idx + 11] = color_k_1.blue;

            // v4
            output_data.color_data[idx + 12] = color_k.red;
            output_data.color_data[idx + 13] = color_k.green;
            output_data.color_data[idx + 14] = color_k.blue;

            // v5
            output_data.color_data[idx + 15] = color_k_1.red;
            output_data.color_data[idx + 16] = color_k_1.green;
            output_data.color_data[idx + 17] = color_k_1.blue;

            // v6
            output_data.color_data[idx + 18] = color_k.red;
            output_data.color_data[idx + 19] = color_k.green;
            output_data.color_data[idx + 20] = color_k.blue;

            // v7
            output_data.color_data[idx + 21] = color_k.red;
            output_data.color_data[idx + 22] = color_k.green;
            output_data.color_data[idx + 23] = color_k.blue;

            // v8
            output_data.color_data[idx + 24] = color_k.red;
            output_data.color_data[idx + 25] = color_k.green;
            output_data.color_data[idx + 26] = color_k.blue;

            // v9
            output_data.color_data[idx + 27] = color_k.red;
            output_data.color_data[idx + 28] = color_k.green;
            output_data.color_data[idx + 29] = color_k.blue;

            // v10
            output_data.color_data[idx + 30] = color_k.red;
            output_data.color_data[idx + 31] = color_k.green;
            output_data.color_data[idx + 32] = color_k.blue;

            // v11
            output_data.color_data[idx + 33] = color_k.red;
            output_data.color_data[idx + 34] = color_k.green;
            output_data.color_data[idx + 35] = color_k.blue;

            idx += 36;
        }

        const RGBTripletf color_k{static_cast<float>(input_data_rgb[input_params.num_elements - 1].red) / 255.0f,
                                  static_cast<float>(input_data_rgb[input_params.num_elements - 1].green) / 255.0f,
                                  static_cast<float>(input_data_rgb[input_params.num_elements - 1].blue) / 255.0f};

        const RGBTripletf color_k_1{static_cast<float>(input_data_rgb[input_params.num_elements - 2].red) / 255.0f,
                                    static_cast<float>(input_data_rgb[input_params.num_elements - 2].green) / 255.0f,
                                    static_cast<float>(input_data_rgb[input_params.num_elements - 2].blue) / 255.0f};
        // v0
        output_data.color_data[idx] = color_k_1.red;
        output_data.color_data[idx + 1] = color_k_1.green;
        output_data.color_data[idx + 2] = color_k_1.blue;

        // v1
        output_data.color_data[idx + 3] = color_k.red;
        output_data.color_data[idx + 4] = color_k.green;
        output_data.color_data[idx + 5] = color_k.blue;

        // v2
        output_data.color_data[idx + 6] = color_k.red;
        output_data.color_data[idx + 7] = color_k.green;
        output_data.color_data[idx + 8] = color_k.blue;

        // v3
        output_data.color_data[idx + 9] = color_k_1.red;
        output_data.color_data[idx + 10] = color_k_1.green;
        output_data.color_data[idx + 11] = color_k_1.blue;

        // v4
        output_data.color_data[idx + 12] = color_k.red;
        output_data.color_data[idx + 13] = color_k.green;
        output_data.color_data[idx + 14] = color_k.blue;

        // v5
        output_data.color_data[idx + 15] = color_k_1.red;
        output_data.color_data[idx + 16] = color_k_1.green;
        output_data.color_data[idx + 17] = color_k_1.blue;
    }

    return output_data;
}

}  // namespace
