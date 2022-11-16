#include "plot_pane_background.h"

#include <cmath>
#include <iostream>

void PlotPaneBackground::render(const float pane_width, const float pane_height)
{
    const float radius = 100.0f;

    const float rx = radius / pane_width;
    const float ry = radius / pane_height;

    constexpr float kVal = 3.0f;

    const float dtheta = (M_PI / 2.0f) / static_cast<float>(num_corner_segments_);
    float theta = 0.0f;
    size_t idx = 0;

    // Upper left corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = -kVal + rx;
        const float cy = kVal - ry;

        data_array_[idx] = -kVal;
        data_array_[idx + 1] = kVal;

        data_array_[idx + 2] = cx - rx * std::cos(theta);
        data_array_[idx + 3] = cy + ry * std::sin(theta);

        data_array_[idx + 4] = cx - rx * std::cos(theta + dtheta);
        data_array_[idx + 5] = cy + ry * std::sin(theta + dtheta);

        theta += dtheta;

        idx += 6U;
    }

    theta = 0.0f;
    // Upper right corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = kVal - rx;
        const float cy = kVal - ry;

        data_array_[idx] = kVal;
        data_array_[idx + 1] = kVal;

        data_array_[idx + 2] = cx + rx * std::cos(theta);
        data_array_[idx + 3] = cy + ry * std::sin(theta);

        data_array_[idx + 4] = cx + rx * std::cos(theta + dtheta);
        data_array_[idx + 5] = cy + ry * std::sin(theta + dtheta);

        theta += dtheta;

        idx += 6U;
    }

    theta = 0.0f;
    // Lower right corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = kVal - rx;
        const float cy = -kVal + ry;

        data_array_[idx] = kVal;
        data_array_[idx + 1] = -kVal;

        data_array_[idx + 2] = cx + rx * std::cos(theta);
        data_array_[idx + 3] = cy - ry * std::sin(theta);

        data_array_[idx + 4] = cx + rx * std::cos(theta + dtheta);
        data_array_[idx + 5] = cy - ry * std::sin(theta + dtheta);

        theta += dtheta;

        idx += 6U;
    }

    theta = 0.0f;
    // Lower left corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = -kVal + rx;
        const float cy = -kVal + ry;

        data_array_[idx] = -kVal;
        data_array_[idx + 1] = -kVal;

        data_array_[idx + 2] = cx - rx * std::cos(theta);
        data_array_[idx + 3] = cy - ry * std::sin(theta);

        data_array_[idx + 4] = cx - rx * std::cos(theta + dtheta);
        data_array_[idx + 5] = cy - ry * std::sin(theta + dtheta);

        theta += dtheta;

        idx += 6U;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_, data_array_);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices_);
    glBindVertexArray(0);
}

PlotPaneBackground::PlotPaneBackground()
{
    num_corner_segments_ = 10U;
    num_corner_vertices_ = num_corner_segments_ * 3U;
    num_vertices_ = num_corner_vertices_ * 4U;
    num_bytes_ = sizeof(float) * num_vertices_ * 2U;
    data_array_ = new float[num_vertices_ * 2];

    std::memset(data_array_, 0, num_bytes_);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes_, data_array_, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
}

PlotPaneBackground::~PlotPaneBackground()
{
    delete[] data_array_;
}
