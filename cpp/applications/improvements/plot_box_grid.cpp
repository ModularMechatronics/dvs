#include "plot_box_grid.h"

#include <iostream>



void PlotBoxGrid::render(const GridVectors& gv,
                         const AxesSettings& axes_settings,
                         const AxesLimits& axes_limits,
                         const ViewAngles& view_angles)
{
    const size_t num_vertices_to_render = gv.x.size() * 2 + gv.y.size() * 2 + gv.z.size() * 2;
    size_t idx = 0;

    const Vec3Dd s = axes_limits.getAxesScale();

    const double box_x_2 = s.x / 2.0f;
    const double box_y_2 = s.y / 2.0f;
    const double box_z_2 = s.z / 2.0f;

    for(size_t k = 0; k < gv.x.size(); k++)
    {
        grid_points_[idx] = gv.x(k);
        grid_points_[idx + 1] = -1.0f;
        grid_points_[idx + 2] = box_z_2;

        idx += 3;

        grid_points_[idx] = gv.x(k);
        grid_points_[idx + 1] = 1.0f;
        grid_points_[idx + 2] = box_z_2;
    }
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices_to_render * 3 * sizeof(float), grid_points_);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, num_vertices_to_render);
    glBindVertexArray(0);
}

PlotBoxGrid::PlotBoxGrid(const float size)
{
    const size_t max_num_grid_lines_per_plane = 10;
    const size_t num_vertices = max_num_grid_lines_per_plane * 3 * 2;
    const size_t num_array_elements = num_vertices * 3;
    const size_t num_bytes = num_array_elements * sizeof(float);

    grid_points_ = new float[num_array_elements];
    color_ = new GLfloat[num_array_elements];

    constexpr GLfloat kLineColor = 0.1;

    for(size_t k = 0; k < num_array_elements; k += 3)
    {
        color_[k] = 1.0f;
        color_[k + 1] = 0.0f;
        color_[k + 2] = 1.0;
    }

    glGenVertexArrays(1, &vertex_buffer_array_);
    glBindVertexArray(vertex_buffer_array_);

    glGenBuffers(1, &vertex_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, grid_points_, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &color_buffer_);
    glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
    glBufferData(GL_ARRAY_BUFFER, num_bytes, color_, GL_STATIC_DRAW);

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

PlotBoxGrid::~PlotBoxGrid()
{
    delete[] grid_points_;
    delete[] color_;
}