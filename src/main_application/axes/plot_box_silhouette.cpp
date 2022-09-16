#include "plot_box_silhouette.h"

#include <iostream>

// The value '0.0f' corresponds to the dimension that will be changed
static float walls_vertices[] = {
    // XY Plane
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,

    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,

    1.0f, 1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,

    // YZ Plane
    0.0f, -1.0f, -1.0f,
    0.0f, 1.0f, -1.0f,

    0.0f, -1.0f, 1.0f,
    0.0f, 1.0f, 1.0f,

    0.0f, -1.0f, -1.0f,
    0.0f, -1.0f, 1.0f,

    0.0f, 1.0f, -1.0f,
    0.0f, 1.0f, 1.0f,

    // XZ Plane
    -1.0f, 0.0f, -1.0f,
    1.0f, 0.0f, -1.0f,

    -1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f,

    -1.0f, 0.0f, -1.0f,
    -1.0f, 0.0f, 1.0f,

    1.0f, 0.0f, -1.0f,
    1.0f, 0.0f, 1.0f,
};

void PlotBoxSilhouette::setIndices(const size_t first_vertex_idx, const size_t last_vertex_idx, const size_t dimension_idx, const float val)
{
    for(size_t k = first_vertex_idx; k < last_vertex_idx; k++)
    {
        data_array_[k * 3 + dimension_idx] = val;
    }
}

void PlotBoxSilhouette::render(const float azimuth, const float elevation)
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

    const float xy_val = (elevation > 0.0f) ? -1.0f : 1.0f;
    setIndices(kXYFirstIdx, kXYLastIdx, kXYChangeDimension, xy_val);

    const float yz_val = (azimuth >= 0.0f) ? 1.0f : -1.0f;
    setIndices(kYZFirstIdx, kYZLastIdx, kYZChangeDimension, yz_val);

    const float xz_val = (((-M_PI / 2.0f) <= azimuth) && (azimuth <= (M_PI / 2.0f))) ? 1.0f : -1.0f;
    setIndices(kXZFirstIdx, kXZLastIdx, kXZChangeDimension, xz_val);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 12 * 2 * 3, data_array_);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, 2 * 12);
    glBindVertexArray(0);
}

PlotBoxSilhouette::PlotBoxSilhouette()
{
    num_vertices_ = 3U * 8U;
    const size_t num_elements = num_vertices_ * 3U;
    const size_t num_bytes = sizeof(float) * num_elements;
    data_array_ = new float[num_elements];

    std::memcpy(data_array_, walls_vertices, num_bytes);

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, data_array_, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

PlotBoxSilhouette::~PlotBoxSilhouette()
{
    delete[] data_array_;
}
