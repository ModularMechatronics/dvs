#include "plot_box_grid.h"

#include <iostream>

void PlotBoxGrid::renderXYGrid(const GridVectors& gv)
{
    const float z_val = elevation_ > 0.0f ? (-axes_scale_.z) : axes_scale_.z;
    for(size_t k = 0; k < gv.x.num_valid_values; k++)
    {
        grid_points_[idx_] = gv.x.data[k];
        grid_points_[idx_ + 1] = -axes_scale_.y;
        grid_points_[idx_ + 2] = z_val;

        idx_ += 3;

        grid_points_[idx_] = gv.x.data[k];
        grid_points_[idx_ + 1] = axes_scale_.y;
        grid_points_[idx_ + 2] = z_val;

        idx_ += 3;
    }

    for(size_t k = 0; k < gv.y.num_valid_values; k++)
    {
        grid_points_[idx_] = axes_scale_.x;
        grid_points_[idx_ + 1] = gv.y.data[k];
        grid_points_[idx_ + 2] = z_val;

        idx_ += 3;

        grid_points_[idx_] = -axes_scale_.x;
        grid_points_[idx_ + 1] = gv.y.data[k];
        grid_points_[idx_ + 2] = z_val;

        idx_ += 3;
    }
}


void PlotBoxGrid::renderXZGrid(const GridVectors& gv)
{
    const bool cond = ((-M_PI / 2.0f) <= azimuth_) && (azimuth_ <= (M_PI / 2.0f));
    const float y_val = cond ? axes_scale_.y : (-axes_scale_.y);

    for(size_t k = 0; k < gv.x.num_valid_values; k++)
    {
        grid_points_[idx_] = gv.x.data[k];
        grid_points_[idx_ + 1] = y_val;
        grid_points_[idx_ + 2] = -axes_scale_.z;

        idx_ += 3;

        grid_points_[idx_] = gv.x.data[k];
        grid_points_[idx_ + 1] = y_val;
        grid_points_[idx_ + 2] = axes_scale_.z;

        idx_ += 3;
    }

    for(size_t k = 0; k < gv.z.num_valid_values; k++)
    {
        grid_points_[idx_] = axes_scale_.x;
        grid_points_[idx_ + 1] = y_val;
        grid_points_[idx_ + 2] = gv.z.data[k];

        idx_ += 3;

        grid_points_[idx_] = -axes_scale_.x;
        grid_points_[idx_ + 1] = y_val;
        grid_points_[idx_ + 2] = gv.z.data[k];

        idx_ += 3;
    }
}

void PlotBoxGrid::renderYZGrid(const GridVectors& gv)
{
    const float x_val = (azimuth_ >= 0.0f) ? axes_scale_.x : (-axes_scale_.x);

    for(size_t k = 0; k < gv.y.num_valid_values; k++)
    {
        grid_points_[idx_] = x_val;
        grid_points_[idx_ + 1] = gv.y.data[k];
        grid_points_[idx_ + 2] = -axes_scale_.z;

        idx_ += 3;

        grid_points_[idx_] = x_val;
        grid_points_[idx_ + 1] = gv.y.data[k];
        grid_points_[idx_ + 2] = axes_scale_.z;

        idx_ += 3;
    }

    for(size_t k = 0; k < gv.z.num_valid_values; k++)
    {
        grid_points_[idx_] = x_val;
        grid_points_[idx_ + 1] = axes_scale_.y;
        grid_points_[idx_ + 2] = gv.z.data[k];

        idx_ += 3;

        grid_points_[idx_] = x_val;
        grid_points_[idx_ + 1] = -axes_scale_.y;
        grid_points_[idx_ + 2] = gv.z.data[k];

        idx_ += 3;
    }
}

void PlotBoxGrid::render(const GridVectors& gv,
                         const AxesLimits& axes_limits,
                         const ViewAngles& view_angles)
{
    azimuth_ = view_angles.getSnappedAzimuth();
    elevation_ = view_angles.getSnappedElevation();

    idx_ = 0;

    axes_scale_ = axes_limits.getAxesScale();

    renderXYGrid(gv);
    renderXZGrid(gv);
    renderYZGrid(gv);

    const size_t num_vertices_to_render = idx_ / 3;

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
    glBufferSubData(GL_ARRAY_BUFFER, 0, num_vertices_to_render * 3 * sizeof(float), grid_points_);
    glBindVertexArray(vertex_buffer_array_);
    glDrawArrays(GL_LINES, 0, num_vertices_to_render);
    glBindVertexArray(0);
}

PlotBoxGrid::PlotBoxGrid(const float size)
{
    const size_t max_num_grid_lines_per_plane = 10;
    const size_t num_vertices = max_num_grid_lines_per_plane * 3 * 2 * 2;
    const size_t num_array_elements = num_vertices * 3;
    const size_t num_bytes = num_array_elements * sizeof(float);

    grid_points_ = new float[num_array_elements];
    color_ = new GLfloat[num_array_elements];

    constexpr GLfloat kLineColor = 0.1;

    for(size_t k = 0; k < num_array_elements; k += 3)
    {
        color_[k] = 0.0f;
        color_[k + 1] = 0.0f;
        color_[k + 2] = 0.0f;
    }

    azimuth_ = 0.0f;
    elevation_ = 0.0f;

    idx_ = 0;

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