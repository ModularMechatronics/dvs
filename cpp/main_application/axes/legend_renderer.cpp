#include "axes/legend_renderer.h"

#include <iostream>

const float dx = 2.4;
const float dz = 0.2;
const float x_min = 0.0;
const float x_max = x_min + dx;
const float z_min = 0.0;
const float z_max = z_min + dz;

const float dz_text = 0.5;

float legend_inner_vertices[] = {
        x_min, 0.0f, z_min,
        x_max, 0.0f, z_min,
        x_max, 0.0f, z_max,

        x_min, 0.0f, z_min,
        x_min, 0.0f, z_max,
        x_max, 0.0f, z_max
    };

float legend_edge_vertices[] = {
        x_min, 0.0f, z_min,
        x_max, 0.0f, z_min,
        x_max, 0.0f, z_max,
        x_min, 0.0f, z_max,
        x_min, 0.0f, z_min,
    };


constexpr GLfloat xy_r = 0.0f;
constexpr GLfloat xy_g = 0.0f;
constexpr GLfloat xy_b = 0.0f;

constexpr GLfloat yz_r = 0.0f;
constexpr GLfloat yz_g = 0.0f;
constexpr GLfloat yz_b = 0.0f;

constexpr GLfloat xz_r = 0.0f;
constexpr GLfloat xz_g = 0.0f;
constexpr GLfloat xz_b = 0.0f;


GLfloat legend_edge_color[] = {
        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,
        xy_r, xy_g, xy_b,
};

GLfloat legend_color_inner[] = {
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,

        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f
};

void setValues(const float new_x_min, const float new_x_max, const float new_z_min, const float new_z_max)
{
    legend_inner_vertices[0] = new_x_min; legend_inner_vertices[2] = new_z_min;
    legend_inner_vertices[3] = new_x_max; legend_inner_vertices[5] = new_z_min;
    legend_inner_vertices[6] = new_x_max; legend_inner_vertices[8] = new_z_max;

    legend_inner_vertices[9] = new_x_min; legend_inner_vertices[11] = new_z_min;
    legend_inner_vertices[12] = new_x_min; legend_inner_vertices[14] = new_z_max;
    legend_inner_vertices[15] = new_x_max; legend_inner_vertices[17] = new_z_max;

    legend_edge_vertices[0] = new_x_min; legend_edge_vertices[2] = new_z_min;
    legend_edge_vertices[3] = new_x_max; legend_edge_vertices[5] = new_z_min;
    legend_edge_vertices[6] = new_x_max; legend_edge_vertices[8] = new_z_max;
    legend_edge_vertices[9] = new_x_min; legend_edge_vertices[11] = new_z_max;
    legend_edge_vertices[12] = new_x_min; legend_edge_vertices[14] = new_z_min;
}

void LegendRenderer::render(const std::vector<std::string>& legend_names)
{
    setValues(x_min, x_min + dx, z_min, z_max + legend_names.size() * dz_text );

    edge_vao_.renderAndUpdateData(legend_edge_vertices, sizeof(float) * 3 * num_vertices_edge_);
    inner_vao_.renderAndUpdateData(legend_inner_vertices, sizeof(float) * 3 * num_vertices_inner_);
}

LegendRenderer::LegendRenderer()
{
    num_vertices_edge_ = 5;
    num_vertices_inner_ = 6;

    const size_t num_bytes = sizeof(float) * 3 * num_vertices_edge_;

    edge_vao_ = VAOObject(num_vertices_edge_, legend_edge_vertices, legend_edge_color, GL_LINE_STRIP);
    inner_vao_ = VAOObject(num_vertices_inner_, legend_inner_vertices, legend_color_inner, GL_TRIANGLES);
}
