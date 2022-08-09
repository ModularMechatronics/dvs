#include "main_application/plot_objects/plot2d/plot2d.h"

Plot2D::InputData convertData2DOuter(const uint8_t* const input_data,
                          const DataType data_type,
                          const size_t num_elements,
                          const size_t num_bytes_per_element,
                          const size_t num_bytes_for_one_vec);

Plot2D::Plot2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::PLOT2)
    {
        throw std::runtime_error("Invalid function type for Plot2D!");
    }

    input_data_ =
        convertData2DOuter(data_ptr_, data_type_, num_elements_, num_bytes_per_element_, num_bytes_for_one_vec_);

    if(is_dashed_)
    {
        if(line_style_.data == internal::LineStyleType::DASHED)
        {
            gap_size_ = 3.0f;
            dash_size_ = 3.0f;
        }
        else if(line_style_.data == internal::LineStyleType::DOTTED)
        {
            gap_size_ = 6.0f;
            dash_size_ = 3.0f;
        }
        else if(line_style_.data == internal::LineStyleType::LONG_DASHED)
        {
            gap_size_ = 2.0f;
            dash_size_ = 6.0f;
        }
    }

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    const size_t num_segments = num_elements_ - 1;
    const size_t num_points = num_segments * 12U - 6U;

    glGenBuffers(1, &p0_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p0_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, input_data_.p0, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Prev
    glGenBuffers(1, &p1_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, input_data_.p1, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, p1_vertex_buffer_);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Next
    glGenBuffers(1, &p2_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points * 2, input_data_.p2, GL_STATIC_DRAW);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, p2_vertex_buffer_);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

    // Idx
    glGenBuffers(1, &idx_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points, input_data_.idx_data_, GL_STATIC_DRAW);

    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, idx_buffer_);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, 0);

    // Length along
    glGenBuffers(1, &lenth_along_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, lenth_along_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_points, input_data_.length_along, GL_STATIC_DRAW);

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, lenth_along_vertex_buffer_);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, 0, 0);
}

void Plot2D::findMinMax()
{
    Vec2Dd min_vec_2d, max_vec_2d;
    std::tie<Vec2Dd, Vec2Dd>(min_vec_2d, max_vec_2d) =
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
    const size_t num_segments = num_elements_ - 1;
    const size_t num_points = num_segments * 12U - 6U;

    shader_collection_.plot_2d_shader.use();
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "line_width"), line_width_ / 1200.0f);
    glUniform1i(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "use_dash"), is_dashed_);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "dash_size"), dash_size_);
    glUniform1f(glGetUniformLocation(shader_collection_.plot_2d_shader.programId(), "gap_size"), gap_size_);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_points);
    glBindVertexArray(0);
    shader_collection_.basic_plot_shader.use();
}

Plot2D::~Plot2D()
{
    delete[] input_data_.p0;
    delete[] input_data_.p1;
    delete[] input_data_.p2;
    delete[] input_data_.idx_data_;
    delete[] input_data_.length_along;
}

LegendProperties Plot2D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

void Plot2D::debugFunction(glm::mat4 mvp_mat)
{
    /* // In constructor
    points.resize(num_elements_);

    for (int k = 0; k < static_cast<int>(num_elements_); k++)
    {
        const int idx_00 = (k - 1) * static_cast<int>(num_bytes_per_element_);
        const int idx_01 = static_cast<int>(num_bytes_for_one_vec_) + (k - 1) * static_cast<int>(num_bytes_per_element_);

        const int idx_10 = k * static_cast<int>(num_bytes_per_element_);
        const int idx_11 = static_cast<int>(num_bytes_for_one_vec_) + k * static_cast<int>(num_bytes_per_element_);

        const int idx_20 = (k + 1) * static_cast<int>(num_bytes_per_element_);
        const int idx_21 = static_cast<int>(num_bytes_for_one_vec_) + (k + 1) * static_cast<int>(num_bytes_per_element_);

        if(k == 0)
        {
            const float p1x = *reinterpret_cast<const float* const>(&(data_ptr_[idx_10]));
            const float p1y = *reinterpret_cast<const float* const>(&(data_ptr_[idx_11]));

            const float p2x = *reinterpret_cast<const float* const>(&(data_ptr_[idx_20]));
            const float p2y = *reinterpret_cast<const float* const>(&(data_ptr_[idx_21]));

            const Point2Df p1(p1x, p1y);
            const Point2Df p2(p2x, p2y);

            const Vec2Df vq = p2 - p1;
            points[k].p0 = p1 - vq;
            points[k].p1 = p1;
            points[k].p2 = p2;
        }
        else if(k == (static_cast<int>(num_elements_) - 1))
        {
            const float p0x = *reinterpret_cast<const float* const>(&(data_ptr_[idx_00]));
            const float p0y = *reinterpret_cast<const float* const>(&(data_ptr_[idx_01]));

            const float p1x = *reinterpret_cast<const float* const>(&(data_ptr_[idx_10]));
            const float p1y = *reinterpret_cast<const float* const>(&(data_ptr_[idx_11]));

            const Point2Df p0(p0x, p0y);
            const Point2Df p1(p1x, p1y);
            
            const Vec2Df vq = p1 - p0;
            points[k].p0 = p0;
            points[k].p1 = p1;
            points[k].p2 = p1 + vq;
        }
        else
        {
            const float p0x = *reinterpret_cast<const float* const>(&(data_ptr_[idx_00]));
            const float p0y = *reinterpret_cast<const float* const>(&(data_ptr_[idx_01]));

            const float p1x = *reinterpret_cast<const float* const>(&(data_ptr_[idx_10]));
            const float p1y = *reinterpret_cast<const float* const>(&(data_ptr_[idx_11]));

            const float p2x = *reinterpret_cast<const float* const>(&(data_ptr_[idx_20]));
            const float p2y = *reinterpret_cast<const float* const>(&(data_ptr_[idx_21]));

            points[k].p0.x = p0x;
            points[k].p0.y = p0y;

            points[k].p1.x = p1x;
            points[k].p1.y = p1y;

            points[k].p2.x = p2x;
            points[k].p2.y = p2y;
        }

        points[k].v0 = points[k].p1 - points[k].p0;
        points[k].v1 = points[k].p2 - points[k].p1;
    }*/

    /*const Matrixf r_mat = {{0.0f, -1.0f}, {1.0f, 0.0f}};

    const float lw = line_width_ / 1200.0f;

    for(size_t k = 0; k < points.size(); k++)
    {
        const Point2Df p0p = points[k].p0;
        const Point2Df p1p = points[k].p1;
        const Point2Df p2p = points[k].p2;

        glm::vec4 p0_4(p0p.x, p0p.y, 0.0f, 1.0f);
        glm::vec4 p1_4(p1p.x, p1p.y, 0.0f, 1.0f);
        glm::vec4 p2_4(p2p.x, p2p.y, 0.0f, 1.0f);

        glm::vec4 p0_4p = mvp_mat * p0_4;
        glm::vec4 p1_4p = mvp_mat * p1_4;
        glm::vec4 p2_4p = mvp_mat * p2_4;

        const Point2Df p0(p0_4p.x, p0_4p.y);
        const Point2Df p1(p1_4p.x, p1_4p.y);
        const Point2Df p2(p2_4p.x, p2_4p.y);

        const Vec2Df v0 = p1 - p0;
        const Vec2Df v1 = p2 - p1;

        // Line intersection method
        const Vec2Df q0 = r_mat * (v0.normalized()) * lw;
        const Vec2Df q1 = r_mat * (v1.normalized()) * lw;

        const Point2Df p0_0 = -q0;
        const Point2Df p0_1 = -v0.normalized() - q0;

        const Point2Df p1_0 = -q1;
        const Point2Df p1_1 = v1.normalized() - q1;

        const float dp = ((p0_1 - p0_0)).normalized() * ((p1_1 - p1_0)).normalized() + 1.0f;

        Vec2Df res_vec;
        if(std::fabs(dp) < 0.000001f)
        {
            res_vec = -q0.normalized() * lw;
        }
        else
        {
            const HomogeneousLine2D<float> line0 = homogeneousLineFromPoints(p0_0, p0_1);
            const HomogeneousLine2D<float> line1 = homogeneousLineFromPoints(p1_0, p1_1);

            // std::cout << "v0: " << v0 << ", v1: " << v1  << ", q0: " << q0 << ", q1: " << q1 << 
            // "[ " << line0.a << ", " << line0.b << ", " << line0.c << " ]" << std::endl;

            const Point2Df intersection_points = line0.lineIntersection(line1);
            res_vec = intersection_points;
        }
        // std::cout << res_vec << std::endl;
    }

    std::cout << std::endl;*/
}

template <typename T>
Plot2D::InputData convertData2D(const uint8_t* const input_data,
                       const size_t num_elements,
                       const size_t num_bytes_per_element,
                       const size_t num_bytes_for_one_vec)
{
    const size_t num_segments = num_elements - 1;
    const size_t num_points = num_segments * 12U;
    Plot2D::InputData output_data;
    output_data.p0 = new float[2 * num_points];
    output_data.p1 = new float[2 * num_points];
    output_data.p2 = new float[2 * num_points];
    output_data.idx_data_ = new float[num_points];
    output_data.length_along = new float[num_points];

    for(size_t k = 0; k < (2 * num_points); k++)
    {
        output_data.p0[k] = 0;
        output_data.p1[k] = 0;
        output_data.p2[k] = 0;
    }
    for(size_t k = 0; k < num_points; k++)
    {
        output_data.idx_data_[k] = 0;
    }

    /*output_data.length_along[0] = 0.0f;

    for (size_t k = 1; k < num_elements; k++)
    {
        const size_t idx_00 = (k - 1) * num_bytes_per_element;
        const size_t idx_01 = num_bytes_for_one_vec + (k - 1) * num_bytes_per_element;

        const size_t idx_10 = k * num_bytes_per_element;
        const size_t idx_11 = num_bytes_for_one_vec + k * num_bytes_per_element;

        const T p0x = *reinterpret_cast<const T* const>(&(input_data[idx_00]));
        const T p0y = *reinterpret_cast<const T* const>(&(input_data[idx_01]));

        const T p1x = *reinterpret_cast<const T* const>(&(input_data[idx_10]));
        const T p1y = *reinterpret_cast<const T* const>(&(input_data[idx_11]));

        const float vx = p1x - p0x;
        const float vy = p1y - p0y;

        const float d = std::sqrt(vx * vx + vy * vy);
        output_data.length_along[k] = output_data.length_along[k - 1] + d;
    }*/

    struct Points
    {
        Vec2D<float> p0;
        Vec2D<float> p1;
        Vec2D<float> p2;
    };
    std::vector<Points> pts;
    pts.resize(num_elements);

    {
        // First segment
        const size_t idx_10 = 0;
        const size_t idx_11 = num_bytes_for_one_vec;

        const size_t idx_20 = num_bytes_per_element;
        const size_t idx_21 = num_bytes_for_one_vec + num_bytes_per_element;

        const T p1x = *reinterpret_cast<const T* const>(&(input_data[idx_10]));
        const T p1y = *reinterpret_cast<const T* const>(&(input_data[idx_11]));

        const T p2x = *reinterpret_cast<const T* const>(&(input_data[idx_20]));
        const T p2y = *reinterpret_cast<const T* const>(&(input_data[idx_21]));

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
        const size_t idx_00 = (num_elements - 2) * num_bytes_per_element;
        const size_t idx_01 = num_bytes_for_one_vec + (num_elements - 2) * num_bytes_per_element;

        const size_t idx_10 = (num_elements - 1) * num_bytes_per_element;
        const size_t idx_11 = num_bytes_for_one_vec + (num_elements - 1) * num_bytes_per_element;

        const T p0x = *reinterpret_cast<const T* const>(&(input_data[idx_00]));
        const T p0y = *reinterpret_cast<const T* const>(&(input_data[idx_01]));

        const T p1x = *reinterpret_cast<const T* const>(&(input_data[idx_10]));
        const T p1y = *reinterpret_cast<const T* const>(&(input_data[idx_11]));

        const T vx = p1x - p0x;
        const T vy = p1y - p0y;

        pts[num_elements - 1].p0.x = p0x;
        pts[num_elements - 1].p0.y = p0y;

        pts[num_elements - 1].p1.x = p1x;
        pts[num_elements - 1].p1.y = p1y;

        pts[num_elements - 1].p2.x = p1x + vx;
        pts[num_elements - 1].p2.y = p1y + vy;
    }

    // Segments inbetween
    for (size_t k = 1; k < (num_elements - 1); k++)
    {
        const size_t idx_00 = (k - 1) * num_bytes_per_element;
        const size_t idx_01 = num_bytes_for_one_vec + (k - 1) * num_bytes_per_element;

        const size_t idx_10 = k * num_bytes_per_element;
        const size_t idx_11 = num_bytes_for_one_vec + k * num_bytes_per_element;

        const size_t idx_20 = (k + 1) * num_bytes_per_element;
        const size_t idx_21 = num_bytes_for_one_vec + (k + 1) * num_bytes_per_element;

        const T p0x = *reinterpret_cast<const T* const>(&(input_data[idx_00]));
        const T p0y = *reinterpret_cast<const T* const>(&(input_data[idx_01]));

        const T p1x = *reinterpret_cast<const T* const>(&(input_data[idx_10]));
        const T p1y = *reinterpret_cast<const T* const>(&(input_data[idx_11]));

        const T p2x = *reinterpret_cast<const T* const>(&(input_data[idx_20]));
        const T p2y = *reinterpret_cast<const T* const>(&(input_data[idx_21]));

        pts[k].p0.x = p0x;
        pts[k].p0.y = p0y;

        pts[k].p1.x = p1x;
        pts[k].p1.y = p1y;

        pts[k].p2.x = p2x;
        pts[k].p2.y = p2y;
    }

    size_t idx = 0;
    size_t idx_idx = 0;

    for (size_t k = 1; k < num_elements; k++)
    {
        const Points& pt_1 = pts[k - 1];
        const Points& pt = pts[k];

        // p0
        // 1st triangle
        // 0
        output_data.p0[idx] = pt_1.p0.x;
        output_data.p0[idx + 1] = pt_1.p0.y;

        // 1
        output_data.p0[idx + 2] = pt.p0.x;
        output_data.p0[idx + 3] = pt.p0.y;

        // 2
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

        // p1
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

        // p2
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

        // Idx
        output_data.idx_data_[idx_idx] = 0;
        output_data.idx_data_[idx_idx + 1] = 1;
        output_data.idx_data_[idx_idx + 2] = 2;
        output_data.idx_data_[idx_idx + 3] = 3;
        output_data.idx_data_[idx_idx + 4] = 4;
        output_data.idx_data_[idx_idx + 5] = 5;
        output_data.idx_data_[idx_idx + 6] = 6;
        output_data.idx_data_[idx_idx + 7] = 7;
        output_data.idx_data_[idx_idx + 8] = 8;
        output_data.idx_data_[idx_idx + 9] = 9;
        output_data.idx_data_[idx_idx + 10] = 10;
        output_data.idx_data_[idx_idx + 11] = 11;

        idx_idx += 12;

        idx += 24;
    }

    return output_data;
}

Plot2D::InputData convertData2DOuter(const uint8_t* const input_data,
                                   const DataType data_type,
                                   const size_t num_elements,
                                   const size_t num_bytes_per_element,
                                   const size_t num_bytes_for_one_vec)
{
    Plot2D::InputData output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertData2D<float>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertData2D<double>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertData2D<int8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertData2D<int16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertData2D<int32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertData2D<int64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertData2D<uint8_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertData2D<uint16_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertData2D<uint32_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertData2D<uint64_t>(input_data, num_elements, num_bytes_per_element, num_bytes_for_one_vec);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
