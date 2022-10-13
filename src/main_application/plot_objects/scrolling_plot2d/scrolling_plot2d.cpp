#include "main_application/plot_objects/scrolling_plot2d/scrolling_plot2d.h"

struct OutputData
{
    float dt;
    float x;
};

OutputData convertDataScrollingPlotOuter(const uint8_t* const input_data, const DataType data_type);

ScrollingPlot2D::ScrollingPlot2D(std::unique_ptr<const ReceivedData> received_data,
                                 const CommunicationHeader& hdr,
                                 const ShaderCollection shader_collection)
    : PlotObjectBase(std::move(received_data), hdr, shader_collection)
{
    if (type_ != Function::REAL_TIME_PLOT)
    {
        throw std::runtime_error("Invalid function type for ScrollingPlot2D!");
    }

    num_elements_to_draw_ = 1U;

    points_ptr_ = new float[buffer_size_ * 2U];
    dt_vec_ = new float[buffer_size_];

    const size_t num_bytes = buffer_size_ * 2U * sizeof(float);
    std::memset(points_ptr_, 0, num_bytes);
    std::memset(dt_vec_, 0, num_bytes / 2U);

    const OutputData output_data = convertDataScrollingPlotOuter(data_ptr_, data_type_);

    points_ptr_[1U] = output_data.x;

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, points_ptr_, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

void ScrollingPlot2D::findMinMax()
{
    min_vec.x = -1.0;
    min_vec.y = -1.0;
    min_vec.z = -1.0;

    max_vec.x = 1.0;
    max_vec.y = 1.0;
    max_vec.z = 1.0;
}

void ScrollingPlot2D::render()
{
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINE_STRIP, 0, num_elements_to_draw_);
    glBindVertexArray(0);
}

ScrollingPlot2D::~ScrollingPlot2D()
{
    delete[] points_ptr_;
    delete[] dt_vec_;
}

LegendProperties ScrollingPlot2D::getLegendProperties() const
{
    LegendProperties lp{PlotObjectBase::getLegendProperties()};
    lp.type = LegendType::LINE;
    lp.color = color_;

    return lp;
}

void ScrollingPlot2D::updateWithNewData(std::unique_ptr<const ReceivedData> received_data,
                                        const CommunicationHeader& hdr)
{
    data_ptr_ = received_data->data();

    const Properties props(hdr.getObjects());

    if (props.hasProperty(PropertyType::BUFFER_SIZE))
    {
        const BufferSize b = props.getProperty<BufferSize>();
        if (buffer_size_ != b.data)
        {
            std::cout << "New buffer size: " << b.data << std::endl;
            float* points_ptr_tmp = new float[b.data * 2U];
            float* dt_vec_tmp = new float[b.data];

            std::memcpy(points_ptr_tmp, points_ptr_, sizeof(float) * buffer_size_ * 2U);
            std::memcpy(dt_vec_tmp, dt_vec_, sizeof(float) * buffer_size_);

            delete[] points_ptr_;
            delete[] dt_vec_;

            points_ptr_ = points_ptr_tmp;
            dt_vec_ = dt_vec_tmp;

            const size_t num_bytes = b.data * 2U * sizeof(float);

            glGenVertexArrays(1, &vertex_buffer_array_);
            glBindVertexArray(vertex_buffer_array_);

            glGenBuffers(1, &vertex_buffer_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
            glBufferData(GL_ARRAY_BUFFER, num_bytes, points_ptr_, GL_DYNAMIC_DRAW);

            glEnableVertexAttribArray(0);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

            buffer_size_ = b.data;

            // TODO: Delete old GL buffer
        }
    }

    const OutputData output_data = convertDataScrollingPlotOuter(data_ptr_, data_type_);

    num_elements_to_draw_ = (num_elements_to_draw_ + 1U) > buffer_size_ ? buffer_size_ : (num_elements_to_draw_ + 1U);
    int idx = 0;
    const int num_samples_2 = buffer_size_ * 2;
    const int num_samples = buffer_size_;
    for (size_t k = 0; k < (buffer_size_ - 1U); k++)
    {
        points_ptr_[num_samples_2 - idx - 1] = points_ptr_[num_samples_2 - idx - 3];
        idx += 2;
    }

    idx = 0;
    for (size_t k = 0; k < (buffer_size_ - 1U); k++)
    {
        dt_vec_[num_samples - 1 - idx] = dt_vec_[num_samples - 2 - idx];
        idx += 1;
    }

    dt_vec_[0U] = output_data.dt;

    idx = 0;
    for (size_t k = 0; k < buffer_size_; k++)
    {
        points_ptr_[idx + 2] = points_ptr_[idx] + dt_vec_[k];
        idx += 2;
    }

    points_ptr_[1U] = output_data.x;

    const size_t num_bytes_to_replace = buffer_size_ * 2U * sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_to_replace, points_ptr_);
}

template <typename T> OutputData convertDataScrollingPlot(const uint8_t* const input_data)
{
    T dt, x;
    uint8_t* dt_ptr = reinterpret_cast<uint8_t*>(&dt);
    uint8_t* x_ptr = reinterpret_cast<uint8_t*>(&x);

    std::memcpy(dt_ptr, input_data, sizeof(T));
    std::memcpy(x_ptr, input_data + sizeof(T), sizeof(T));

    OutputData output_data;
    output_data.dt = dt;
    output_data.x = x;

    return output_data;
}

OutputData convertDataScrollingPlotOuter(const uint8_t* const input_data, const DataType data_type)
{
    OutputData output_data;
    if (data_type == DataType::FLOAT)
    {
        output_data = convertDataScrollingPlot<float>(input_data);
    }
    else if (data_type == DataType::DOUBLE)
    {
        output_data = convertDataScrollingPlot<double>(input_data);
    }
    else if (data_type == DataType::INT8)
    {
        output_data = convertDataScrollingPlot<int8_t>(input_data);
    }
    else if (data_type == DataType::INT16)
    {
        output_data = convertDataScrollingPlot<int16_t>(input_data);
    }
    else if (data_type == DataType::INT32)
    {
        output_data = convertDataScrollingPlot<int32_t>(input_data);
    }
    else if (data_type == DataType::INT64)
    {
        output_data = convertDataScrollingPlot<int64_t>(input_data);
    }
    else if (data_type == DataType::UINT8)
    {
        output_data = convertDataScrollingPlot<uint8_t>(input_data);
    }
    else if (data_type == DataType::UINT16)
    {
        output_data = convertDataScrollingPlot<uint16_t>(input_data);
    }
    else if (data_type == DataType::UINT32)
    {
        output_data = convertDataScrollingPlot<uint32_t>(input_data);
    }
    else if (data_type == DataType::UINT64)
    {
        output_data = convertDataScrollingPlot<uint64_t>(input_data);
    }
    else
    {
        throw std::runtime_error("Invalid data type!");
    }

    return output_data;
}
