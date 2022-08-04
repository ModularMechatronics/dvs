#include "plot_box_walls.h"

#include <iostream>

float walls_vertices[] = {
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,

        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
    };


constexpr GLfloat xy_r = 1.0f;
constexpr GLfloat xy_g = 1.0f;
constexpr GLfloat xy_b = 1.0f;

constexpr GLfloat yz_r = 1.0f;
constexpr GLfloat yz_g = 1.0f;
constexpr GLfloat yz_b = 1.0f;

constexpr GLfloat xz_r = 1.0f;
constexpr GLfloat xz_g = 1.0f;
constexpr GLfloat xz_b = 1.0f;


GLfloat walls_color[] = {
        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,

        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,

        yz_r, yz_g, yz_b,
        yz_r, yz_g, yz_b,
        yz_r, yz_g, yz_b,

        yz_r, yz_g, yz_b,
        yz_r, yz_g, yz_b,
        yz_r, yz_g, yz_b,

        xz_r, xz_g, xz_b,
        xz_r, xz_g, xz_b,
        xz_r, xz_g, xz_b,

        xz_r, xz_g, xz_b,
        xz_r, xz_g, xz_b,
        xz_r, xz_g, xz_b
};

void PlotBoxWalls::setIndices(const size_t first_vertex_idx, const size_t last_vertex_idx, const size_t dimension_idx, const float val)
{
    for(size_t k = first_vertex_idx; k < last_vertex_idx; k++)
    {
        walls_vertices[k * 3 + dimension_idx] = val;
    }
}

void PlotBoxWalls::render(const float azimuth, const float elevation)
{
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

    const float xy_val = (elevation < 0.0f) ? 1.0f : -1.0f;
    setIndices(kXYFirstIdx, kXYLastIdx, kXYChangeDimension, xy_val);

    const float yz_val = (azimuth >= 0.0f) ? 1.0f : -1.0f;
    setIndices(kYZFirstIdx, kYZLastIdx, kYZChangeDimension, yz_val);

    const float xz_val = (((-M_PI / 2.0f) <= azimuth) && (azimuth <= (M_PI / 2.0f))) ? 1.0f : -1.0f;
    setIndices(kXZFirstIdx, kXZLastIdx, kXZChangeDimension, xz_val);

    glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * 12, walls_vertices);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_TRIANGLES, 0, num_vertices_);
    glBindVertexArray(0);
}

PlotBoxWalls::PlotBoxWalls(const float size)
{
    num_vertices_ = 18;
    const size_t num_bytes = sizeof(float) * 3 * num_vertices_;

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, walls_vertices, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, walls_color, GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glVertexAttribPointer(
        1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
        3,                                // size
        GL_FLOAT,                         // type
        GL_FALSE,                         // normalized?
        0,                                // stride
        (void*)0                          // array buffer offset
    );
}
