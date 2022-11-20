#include "main_application/plot_objects/plot2d/plot2d.h"

#include <functional>

#include "outer_converter.h"

namespace
{

template <typename T> struct HomogeneousLine2D
{
    T a;
    T b;
    T c;
    HomogeneousLine2D(const T a_, const T b_, const T c_) : a{a_}, b{b_}, c{c_} {}
    HomogeneousLine2D() = default;
};

template <typename T> HomogeneousLine2D<T> lineFromTwoPoints(const Vec2<T>& p0, const Vec2<T>& p1)
{
    HomogeneousLine2D<T> line;

    Vec2<T> v = p1 - p0;

    line.a = -v.y;
    line.b = v.x;
    line.c = -(line.a * p0.x + line.b * p0.y);

    return line;
}

struct OutputData
{
    float* pts;
    float* p1;
    int32_t* idx_data;
};

struct InputParams
{
    size_t num_elements;
    size_t num_bytes_per_element;
    size_t num_bytes_for_one_vec;
    float line_width;

    InputParams() = default;
    InputParams(const size_t num_elements_,
                const size_t num_bytes_per_element_,
                const size_t num_bytes_for_one_vec_,
                const float line_width_)
        : num_elements{num_elements_},
          num_bytes_per_element{num_bytes_per_element_},
          num_bytes_for_one_vec{num_bytes_for_one_vec_},
          line_width{line_width_}
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

template <typename T>
Vec2<T> intersectionOfTwoLines(const HomogeneousLine2D<T>& line0, const HomogeneousLine2D<T>& line1)
{
    Vec2<T> p;

    p.x = (line0.b * line1.c - line1.b * line0.c) / (line0.a * line1.b - line1.a * line0.b);
    p.y = -(line0.a * line1.c - line1.a * line0.c) / (line0.a * line1.b - line1.a * line0.b);

    return p;
}

}  // namespace

Plot2D::Plot2D(std::unique_ptr<const ReceivedData> received_data,
               const CommunicationHeader& hdr,
               const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::PLOT2)
    {
        throw std::runtime_error("Invalid function type for Plot2D!");
    }

    const InputParams input_params{num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_, line_width_ / 100.0f};

    const OutputData output_data = applyConverter<OutputData>(data_ptr_, data_type_, Converter{}, input_params);

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

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    const size_t num_segments = num_elements_ - 1U;
    const size_t num_triangles = num_segments * 2U + (num_segments - 1U) * 2U;
    const size_t num_points = num_triangles * 3U;

    // pts
    glGenBuffers(1, &pts_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, pts_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, output_data.pts, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // p1
    glGenBuffers(1, &p1_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, output_data.p1, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Idx
    glGenBuffers(1, &idx_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(int32_t) * num_points, output_data.idx_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glVertexAttribIPointer(2, 1, GL_INT, 0, 0);

    delete[] output_data.pts;
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
    const size_t num_segments = num_elements_ - 1U;
    const size_t num_triangles = num_segments * 2U + (num_segments - 1U) * 2U;
    const size_t num_points = num_triangles * 3U;

    shader_collection_.plot_2d_shader.use();
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "half_line_width"),
                line_width_ / 1200.0f);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_points);
    glBindVertexArray(0);
    shader_collection_.basic_plot_shader.use();
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
    output_data.pts = new float[2 * num_points];
    output_data.p1 = new float[2 * num_points];
    output_data.idx_data = new int32_t[num_points];

    const float half_line_width = input_params.line_width / 2.0f;
    std::memset(output_data.pts, 0, 2 * num_points * sizeof(float));

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

    for (size_t k = 1; k < (input_params.num_elements - 1U); k++)
    {
        const Points& pt_1 = pts[k - 1];
        const Points& pt = pts[k];

        const Vec2f vec_along01 = (pt.p1 - pt.p0).normalized();
        const Vec2f vec_along12 = (pt.p2 - pt.p1).normalized();

        const Vec2f vec_on_line_edge01(-vec_along01.y * half_line_width, vec_along01.x * half_line_width);
        const Vec2f vec_on_line_edge12(-vec_along12.y * half_line_width, vec_along12.x * half_line_width);

        const Vec2f vec_along01_1 = (pt_1.p1 - pt_1.p0).normalized();
        const Vec2f vec_along12_1 = (pt_1.p2 - pt_1.p1).normalized();

        const Vec2f vec_on_line_edge01_1(-vec_along01_1.y * half_line_width, vec_along01_1.x * half_line_width);
        const Vec2f vec_on_line_edge12_1(-vec_along12_1.y * half_line_width, vec_along12_1.x * half_line_width);

        const float which_side_vec_z = vec_along01.x * vec_along12.y - vec_along01.y * vec_along12.x;

        bool should_flip;
        if (which_side_vec_z > 0.0)
        {
            should_flip = true;
        }
        else
        {
            should_flip = false;
        }

        const Vec2 point0_on_edge01 = vec_on_line_edge01;
        const Vec2 point1_on_edge01 = vec_on_line_edge01 - vec_along01;

        const Vec2 point0_on_edge12 = vec_on_line_edge12;
        const Vec2 point1_on_edge12 = vec_on_line_edge12 + vec_along12;

        const HomogeneousLine2D<float> line0 = lineFromTwoPoints(point0_on_edge01, point1_on_edge01);
        const HomogeneousLine2D<float> line1 = lineFromTwoPoints(point0_on_edge12, point1_on_edge12);

        Vec2 intersection_point = intersectionOfTwoLines(line0, line1);

        const Vec2f t00 = pt.p1 + vec_on_line_edge01;
        const Vec2f t01 = pt_1.p1 - vec_on_line_edge12_1;
        const Vec2f t02 = pt_1.p1 + vec_on_line_edge12_1;

        output_data.pts[idx] = t00.x;
        output_data.pts[idx + 1] = t00.y;

        output_data.pts[idx + 2] = t01.x;
        output_data.pts[idx + 3] = t01.y;

        output_data.pts[idx + 4] = t02.x;
        output_data.pts[idx + 5] = t02.y;

        // Second triangle
        const Vec2f t10 = pt.p1 + vec_on_line_edge01;
        const Vec2f t11 = pt.p1 - vec_on_line_edge01;
        const Vec2f t12 = pt_1.p1 - vec_on_line_edge12_1;

        output_data.pts[idx + 6] = t10.x;
        output_data.pts[idx + 7] = t10.y;

        output_data.pts[idx + 8] = t11.x;
        output_data.pts[idx + 9] = t11.y;

        output_data.pts[idx + 10] = t12.x;
        output_data.pts[idx + 11] = t12.y;

        // Third triangle
        const Vec2f t20 = pt.p1;

        Vec2f t21;
        if (should_flip)
        {
            t21 = pt.p1 - vec_on_line_edge01;
        }
        else
        {
            t21 = pt.p1 + vec_on_line_edge01;
        }

        Vec2f t22;
        if (should_flip)
        {
            t22 = pt.p1 - vec_on_line_edge12;
        }
        else
        {
            t22 = pt.p1 + vec_on_line_edge12;
        }

        output_data.pts[idx + 12] = t20.x;
        output_data.pts[idx + 13] = t20.y;

        output_data.pts[idx + 14] = t21.x;
        output_data.pts[idx + 15] = t21.y;

        output_data.pts[idx + 16] = t22.x;
        output_data.pts[idx + 17] = t22.y;

        // Fourth triangle
        Vec2f t30, t31, t32;
        if (should_flip)
        {
            t30 = pt.p1 - vec_on_line_edge01;
        }
        else
        {
            t30 = pt.p1 + vec_on_line_edge01;
        }

        if (should_flip)
        {
            t31 = pt.p1 - vec_on_line_edge12;
        }
        else
        {
            t31 = pt.p1 + vec_on_line_edge12;
        }

        if (should_flip)
        {
            intersection_point = -intersection_point;
        }
        t32 = pt.p1 + intersection_point;

        output_data.pts[idx + 18] = t30.x;
        output_data.pts[idx + 19] = t30.y;

        output_data.pts[idx + 20] = t31.x;
        output_data.pts[idx + 21] = t31.y;

        output_data.pts[idx + 22] = t32.x;
        output_data.pts[idx + 23] = t32.y;

        output_data.p1[idx + 18] = pt.p1.x;
        output_data.p1[idx + 19] = pt.p1.y;

        output_data.p1[idx + 20] = pt.p1.x;
        output_data.p1[idx + 21] = pt.p1.y;

        output_data.p1[idx + 22] = pt.p1.x;
        output_data.p1[idx + 23] = pt.p1.y;

        idx += 24;

        output_data.idx_data[idx_idx] = 0;
        output_data.idx_data[idx_idx + 1] = 0;
        output_data.idx_data[idx_idx + 2] = 0;
        output_data.idx_data[idx_idx + 3] = 0;
        output_data.idx_data[idx_idx + 4] = 0;
        output_data.idx_data[idx_idx + 5] = 0;
        output_data.idx_data[idx_idx + 6] = 0;
        output_data.idx_data[idx_idx + 7] = 0;
        output_data.idx_data[idx_idx + 8] = 0;
        output_data.idx_data[idx_idx + 9] = 1;
        output_data.idx_data[idx_idx + 10] = 1;
        output_data.idx_data[idx_idx + 11] = 1;

        idx_idx += 12;
    }

    // Last segment will not have the two "Inbetween triangles"
    const Points& pt_1 = pts[input_params.num_elements - 2];
    const Points& pt = pts[input_params.num_elements - 1];

    const Vec2f vec_along01 = (pt.p1 - pt.p0).normalized();
    const Vec2f vec_along12 = (pt.p2 - pt.p1).normalized();

    const Vec2f vec_on_line_edge01(-vec_along01.y * half_line_width, vec_along01.x * half_line_width);
    const Vec2f vec_on_line_edge12(-vec_along12.y * half_line_width, vec_along12.x * half_line_width);

    const Vec2f vec_along01_1 = (pt_1.p1 - pt_1.p0).normalized();
    const Vec2f vec_along12_1 = (pt_1.p2 - pt_1.p1).normalized();

    const Vec2f vec_on_line_edge01_1(-vec_along01_1.y * half_line_width, vec_along01_1.x * half_line_width);
    const Vec2f vec_on_line_edge12_1(-vec_along12_1.y * half_line_width, vec_along12_1.x * half_line_width);

    const Vec2f t00 = pt.p1 + vec_on_line_edge01;
    const Vec2f t01 = pt_1.p1 - vec_on_line_edge12_1;
    const Vec2f t02 = pt_1.p1 + vec_on_line_edge12_1;

    output_data.pts[idx] = t00.x;
    output_data.pts[idx + 1] = t00.y;

    output_data.pts[idx + 2] = t01.x;
    output_data.pts[idx + 3] = t01.y;

    output_data.pts[idx + 4] = t02.x;
    output_data.pts[idx + 5] = t02.y;

    // Second triangle
    const Vec2f t10 = pt.p1 + vec_on_line_edge01;
    const Vec2f t11 = pt.p1 - vec_on_line_edge01;
    const Vec2f t12 = pt_1.p1 - vec_on_line_edge12_1;

    output_data.pts[idx + 6] = t10.x;
    output_data.pts[idx + 7] = t10.y;

    output_data.pts[idx + 8] = t11.x;
    output_data.pts[idx + 9] = t11.y;

    output_data.pts[idx + 10] = t12.x;
    output_data.pts[idx + 11] = t12.y;

    return output_data;
}

}  // namespace
