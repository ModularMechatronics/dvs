#include "axes/legend_renderer.h"

#include <iostream>

const float x_min = -0.1;
const float x_max = 0.5;
const float z_min = 0.1;
const float z_max = 0.5;

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

GLfloat legend_color_inner[] = {
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,

        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0
};

void LegendRenderer::render()
{
    edge_vao_.render();
    inner_vao_.render();
}

LegendRenderer::LegendRenderer()
{
    num_vertices_ = 5;
    const size_t num_vertices_inner_ = 6;

    const size_t num_bytes = sizeof(float) * 3 * num_vertices_;

    edge_vao_ = VAOObject(num_vertices_, legend_edge_vertices, legend_edge_color, GL_LINE_STRIP);
    inner_vao_ = VAOObject(num_vertices_inner_, legend_inner_vertices, legend_color_inner, GL_TRIANGLES);
}
