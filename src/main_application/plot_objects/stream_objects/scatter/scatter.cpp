#include "plot_objects/stream_objects/scatter/scatter.h"

ScatterStream::ScatterStream() {}

constexpr size_t kStreamBufferSize{500U};

ScatterStream::ScatterStream(const SubscribedStreamSettings& subscribed_stream_settings,
                             const ShaderCollection& shader_collection)
    : StreamObjectBase(subscribed_stream_settings, shader_collection)
{
    num_elements_to_draw_ = 0U;

    points_ptr_ = new float[kStreamBufferSize * 2U];

    const size_t num_bytes = kStreamBufferSize * 2U * sizeof(float);
    std::memset(points_ptr_, 0, num_bytes);

    glGenVertexArrays(1, &sp_vertex_buffer_array_);
    glBindVertexArray(sp_vertex_buffer_array_);

    glGenBuffers(1, &sp_vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, sp_vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, points_ptr_, GL_DYNAMIC_DRAW);

    point_size_ = subscribed_stream_settings.point_size;

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

    if (subscribed_stream_settings.color.has_value())
    {
        color_ = RGBTripletf(static_cast<float>(subscribed_stream_settings.color.value().red) / 255.0f,
                             static_cast<float>(subscribed_stream_settings.color.value().green) / 255.0f,
                             static_cast<float>(subscribed_stream_settings.color.value().blue) / 255.0f);
    }
    else
    {
        color_ = RGBTripletf(0.0f, 0.0f, 0.0f);
    }
}

void ScatterStream::appendNewData(const std::vector<std::shared_ptr<objects::BaseObject>>& obj)
{
    for (size_t k = 0; k < obj.size(); k++)
    {
        appendNewData(obj[k]);
    }
}

void ScatterStream::appendNewData(const std::shared_ptr<objects::BaseObject>& obj)
{
    num_elements_to_draw_ =
        (num_elements_to_draw_ + 1U) > kStreamBufferSize ? kStreamBufferSize : (num_elements_to_draw_ + 1U);

    const int64_t num_samples_2 = kStreamBufferSize * 2;
    const uint64_t timestamp = obj->timestamp();

    float dt;

    if (timestamp >= previous_timestamp_)
    {
        dt = 0.001f * static_cast<float>(timestamp - previous_timestamp_);
    }
    else
    {
        // Overflow has occured
        constexpr uint64_t kMaxTimestamp = 0xFFFFFFFFU;  // The timestamp received from the microcontroller is 32-bit
        dt = 0.001f * static_cast<float>((kMaxTimestamp - previous_timestamp_) + timestamp);
    }

    int64_t idx = static_cast<uint64_t>(num_samples_2) - 1;

    for (size_t k = 0; k < (kStreamBufferSize - 1); k++)
    {
        points_ptr_[idx] = points_ptr_[idx - 2];
        points_ptr_[idx - 1] = points_ptr_[idx - 3] + dt;
        idx -= 2;
    }

    points_ptr_[0U] = 0.0f;
    points_ptr_[1U] = static_cast<objects::Float*>(obj.get())->value();

    constexpr size_t num_bytes_to_replace = kStreamBufferSize * 2U * sizeof(float);

    glBindBuffer(GL_ARRAY_BUFFER, sp_vertex_buffer_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_to_replace, points_ptr_);

    previous_timestamp_ = timestamp;
}

void ScatterStream::render()
{
    shader_collection_.scatter_shader.use();
    shader_collection_.scatter_shader.base_uniform_handles.point_size.setFloat(point_size_);
    shader_collection_.scatter_shader.base_uniform_handles.scatter_mode.setInt(static_cast<int>(2));
    shader_collection_.scatter_shader.base_uniform_handles.vertex_color.setColor(color_);

    glBindVertexArray(sp_vertex_buffer_array_);
    glDrawArrays(GL_POINTS, 0, num_elements_to_draw_);
    glBindVertexArray(0);
}

void ScatterStream::clear()
{
    num_elements_to_draw_ = 0U;
}
