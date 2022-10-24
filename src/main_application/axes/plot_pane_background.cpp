#include "plot_pane_background.h"

#include <cmath>
#include <iostream>

void PlotPaneBackground::render(const float pane_width, const float pane_height)
{
    const float radius = 20.0f;

    const float rx = radius / pane_width;
    const float ry = radius / pane_height;

    constexpr float kVal = 3.0f;
    // First triangle
    data_array_[0] = -kVal + rx;
    data_array_[1] = kVal;

    data_array_[2] = -kVal + rx;
    data_array_[3] = -kVal;

    data_array_[4] = kVal - rx;
    data_array_[5] = -kVal;

    // Second triangle
    data_array_[6] = -kVal + rx;
    data_array_[7] = kVal;

    data_array_[8] = kVal - rx;
    data_array_[9] = kVal;

    data_array_[10] = kVal - rx;
    data_array_[11] = -kVal;

    // Third triangle
    data_array_[12] = -kVal;
    data_array_[13] = kVal - ry;

    data_array_[14] = -kVal + rx;
    data_array_[15] = kVal - ry;

    data_array_[16] = -kVal + rx;
    data_array_[17] = -kVal + ry;

    // Fourth triangle
    data_array_[18] = -kVal + rx;
    data_array_[19] = -kVal + ry;

    data_array_[20] = -kVal;
    data_array_[21] = -kVal + ry;

    data_array_[22] = -kVal;
    data_array_[23] = kVal - ry;

    // Fifth triangle
    data_array_[24] = kVal - rx;
    data_array_[25] = kVal - ry;

    data_array_[26] = kVal;
    data_array_[27] = kVal - ry;

    data_array_[28] = kVal;
    data_array_[29] = -kVal + ry;

    // Sixth triangle
    data_array_[30] = kVal - rx;
    data_array_[31] = kVal - ry;

    data_array_[32] = kVal - rx;
    data_array_[33] = -kVal + ry;

    data_array_[34] = kVal;
    data_array_[35] = -kVal + ry;

    const float dtheta = (M_PI / 2.0f) / static_cast<float>(num_corner_segments_);
    float theta = 0.0f;
    size_t idx = 0;

    // Upper left corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = -kVal + rx;
        const float cy = kVal - ry;

        data_array_[36 + idx] = cx;
        data_array_[36 + idx + 1] = cy;

        data_array_[36 + idx + 2] = cx - rx * std::cos(theta);
        data_array_[36 + idx + 3] = cy + ry * std::sin(theta);

        data_array_[36 + idx + 4] = cx - rx * std::cos(theta + dtheta);
        data_array_[36 + idx + 5] = cy + ry * std::sin(theta + dtheta);

        theta += dtheta;

        idx += 6U;
    }

    theta = 0.0f;
    // Upper right corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = kVal - rx;
        const float cy = kVal - ry;

        data_array_[36 + idx] = cx;
        data_array_[36 + idx + 1] = cy;

        data_array_[36 + idx + 2] = cx + rx * std::cos(theta);
        data_array_[36 + idx + 3] = cy + ry * std::sin(theta);

        data_array_[36 + idx + 4] = cx + rx * std::cos(theta + dtheta);
        data_array_[36 + idx + 5] = cy + ry * std::sin(theta + dtheta);

        theta += dtheta;

        idx += 6U;
    }

    theta = 0.0f;
    // Lower right corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = kVal - rx;
        const float cy = -kVal + ry;

        data_array_[36 + idx] = cx;
        data_array_[36 + idx + 1] = cy;

        data_array_[36 + idx + 2] = cx + rx * std::cos(theta);
        data_array_[36 + idx + 3] = cy - ry * std::sin(theta);

        data_array_[36 + idx + 4] = cx + rx * std::cos(theta + dtheta);
        data_array_[36 + idx + 5] = cy - ry * std::sin(theta + dtheta);

        theta += dtheta;

        idx += 6U;
    }

    theta = 0.0f;
    // Lower left corner
    for (size_t k = 0; k < num_corner_segments_; k++)
    {
        const float cx = -kVal + rx;
        const float cy = -kVal + ry;

        data_array_[36 + idx] = cx;
        data_array_[36 + idx + 1] = cy;

        data_array_[36 + idx + 2] = cx - rx * std::cos(theta);
        data_array_[36 + idx + 3] = cy - ry * std::sin(theta);

        data_array_[36 + idx + 4] = cx - rx * std::cos(theta + dtheta);
        data_array_[36 + idx + 5] = cy - ry * std::sin(theta + dtheta);

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
    num_vertices_ = 18U + num_corner_vertices_ * 4U;
    num_bytes_ = sizeof(float) * num_vertices_ * 2U;
    data_array_ = new float[num_vertices_ * 2];

    std::memset(data_array_, 0, num_bytes_);

    // std::memcpy(data_array_, walls_vertices, num_bytes_);

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
