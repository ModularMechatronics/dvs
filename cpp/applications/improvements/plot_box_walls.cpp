#include "plot_box_walls.h"


float walls_vertices[] = {
        0.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        1.0f, 1.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,

        1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 0.0f,
    };

constexpr GLfloat kWallColor = 0.5;

GLfloat walls_color[] = {
        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,

        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,

        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,

        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,

        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,

        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor,
        kWallColor,  kWallColor,  kWallColor
};


void PlotBoxWalls::render() const
{
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
    glBufferData(GL_ARRAY_BUFFER, num_bytes, walls_vertices, GL_STATIC_DRAW);

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
