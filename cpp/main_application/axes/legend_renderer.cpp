#include "axes/legend_renderer.h"

#include <iostream>

#include "opengl_low_level/data_structures.h"

constexpr int kMaxNumPoints{100};

GLfloat legend_color_edge[] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,
};

GLfloat legend_color_inner[] = {
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,

        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f,
        0.866666666f, 0.827450f, 0.7843137f
};

void LegendRenderer::setBoxValues(const float new_x_min, const float new_x_max, const float new_z_min, const float new_z_max)
{
    legend_inner_vertices_(0) = new_x_min; legend_inner_vertices_(2) = new_z_min;
    legend_inner_vertices_(3) = new_x_max; legend_inner_vertices_(5) = new_z_min;
    legend_inner_vertices_(6) = new_x_max; legend_inner_vertices_(8) = new_z_max;

    legend_inner_vertices_(9) = new_x_min; legend_inner_vertices_(11) = new_z_min;
    legend_inner_vertices_(12) = new_x_min; legend_inner_vertices_(14) = new_z_max;
    legend_inner_vertices_(15) = new_x_max; legend_inner_vertices_(17) = new_z_max;

    legend_edge_vertices_(0) = new_x_min; legend_edge_vertices_(2) = new_z_min;
    legend_edge_vertices_(3) = new_x_max; legend_edge_vertices_(5) = new_z_min;
    legend_edge_vertices_(6) = new_x_max; legend_edge_vertices_(8) = new_z_max;
    legend_edge_vertices_(9) = new_x_min; legend_edge_vertices_(11) = new_z_max;
    legend_edge_vertices_(12) = new_x_min; legend_edge_vertices_(14) = new_z_min;
}

void LegendRenderer::setVertexAtIdx(const float x, const float y, const float z, const size_t idx)
{
    points_.data()[idx] = x;
    points_.data()[idx + 1] = y;
    points_.data()[idx + 2] = z;
}

void LegendRenderer::setColorAtIdx(const float r, const float g, const float b, const size_t idx)
{
    colors_.data()[idx] = r;
    colors_.data()[idx + 1] = g;
    colors_.data()[idx + 2] = b;
}

void LegendRenderer::renderColorMapLegend(const size_t num_segments, const RGBColorMap<float>* const color_map, const float xc, const float yc, const float r, const float axes_width, const float axes_height)
{
    const float delta_phi = static_cast<float>(M_PI) * 2.0f / static_cast<float>(num_segments);
    float angle = 0.0f;
    const float mul = 400.0f;     // Empirically found
    int idx = 0;

    for(size_t k = 0; k < num_segments; k++)
    {
        const RGBTripletf color{color_map->getColor(angle / (2.0f * M_PI))};
        setVertexAtIdx(xc, 0.0f, yc, idx);
        setColorAtIdx(color.red, color.green, color.blue, idx);
        idx += 3;

        setVertexAtIdx(r * cos(angle) * mul / axes_width + xc, 0.0f, r * sin(angle) * mul / axes_height + yc, idx);
        setColorAtIdx(color.red, color.green, color.blue, idx);
        idx += 3;
        angle += delta_phi;

        setVertexAtIdx(r * cos(angle) * mul / axes_width + xc, 0.0f, r * sin(angle) * mul / axes_height + yc, idx);
        setColorAtIdx(color.red, color.green, color.blue, idx);
        idx += 3;
    }
}

void LegendRenderer::setLegendScaleFactor(const float new_scale_factor)
{
    scale_factor_ = new_scale_factor > 0.1f ? new_scale_factor : 1.0f;
}

void LegendRenderer::render(const std::vector<LegendProperties>& legend_properties, const float axes_width, const float axes_height)
{
    float max_width = 0.0f;
    for(size_t k = 0; k < legend_properties.size(); k++)
    {
        const dvs::Vec2Df sz = calculateStringSize(legend_properties[k].name, 0.0005f, axes_width, axes_height);
        const float current_width = sz.x;
        max_width = std::max(max_width, current_width);
    }

    const float text_x_offset = 150.0f / axes_width;  // 150.0f is "empirically" found
    const float text_x_margin = 150.0f / axes_width;
    const float z_offset = 100.0f / axes_height;     // 100.0f is "empirically" found
    const float x_offset = 100.0f / axes_width;
    const float legend_element_z_delta = 140.0f / axes_height;
    // Multiply by 3.0f because the text coordinates has a scale of 1/3 relative to the box coords.
    const float box_width = max_width * 3.0f + text_x_offset + text_x_margin;

    const float x_max = 3.0f - x_offset;// * scale_factor_;
    const float x_min = x_max - box_width * scale_factor_;

    const float z_max = 3.0f - z_offset; //  * scale_factor_;
    // std::max in case legend_properties.size() == 0
    const float z_min = z_max - (2.0f * z_offset + legend_element_z_delta * (std::max(legend_properties.size(), 1UL) - 1)) * scale_factor_;

    setBoxValues(x_min, x_max, z_min, z_max);

    edge_vao_.renderAndUpdateData(legend_edge_vertices_.data(), sizeof(float) * 3 * num_vertices_edge_);
    inner_vao_.renderAndUpdateData(legend_inner_vertices_.data(), sizeof(float) * 3 * num_vertices_inner_);

    float* const legend_shape_vertices = points_.data();
    float* const legend_shape_colors = colors_.data();

    const float x_center = x_min + scale_factor_ * text_x_offset / 2.0f;
    const float legend_symbol_width = 70.0f / axes_width;
    const float legend_symbol_height = 70.0f / axes_height;

    for(size_t k = 0; k < legend_properties.size(); k++)
    {
        const float kf = k;
        const float z_center = z_max - scale_factor_ * (z_offset + legend_element_z_delta * kf);
        if(legend_properties[k].type == LegendType::LINE)
        {
            const RGBTripletf col = legend_properties[k].color;
            legend_shape_vertices[0] = x_center - scale_factor_ * legend_symbol_width / 2.0f;
            legend_shape_vertices[1] = 0.0f;
            legend_shape_vertices[2] = z_center;
            

            legend_shape_vertices[3] = x_center + scale_factor_ * legend_symbol_width / 2.0;
            legend_shape_vertices[4] = 0.0f;
            legend_shape_vertices[5] = z_center;

            legend_shape_colors[0] = col.red;
            legend_shape_colors[1] = col.green;
            legend_shape_colors[2] = col.blue;

            legend_shape_colors[3] = col.red;
            legend_shape_colors[4] = col.green;
            legend_shape_colors[5] = col.blue;
            
            legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 2, 2 * 3 * sizeof(float), GL_LINE_STRIP);
        }
        else if(legend_properties[k].type == LegendType::POLYGON)
        {
            if(legend_properties[k].color_map_set)
            {
                const size_t num_segments = 6;
                const float radius = scale_factor_ * 0.1;
                renderColorMapLegend(num_segments, legend_properties[k].color_map, x_center, z_center, radius, axes_width, axes_height);
                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, num_segments * 3, num_segments * 3 * 3 * sizeof(float), GL_TRIANGLES);
            }
            else
            {
                const RGBTripletf edge_color = legend_properties[k].edge_color;
                const RGBTripletf face_color = legend_properties[k].face_color;
                const float dxc = scale_factor_ * legend_symbol_width / 2.0;
                const float dzc = scale_factor_ * legend_symbol_height / 2.0;

                // Face 0
                legend_shape_vertices[0] = x_center - dxc;
                legend_shape_vertices[1] = 0.0f;
                legend_shape_vertices[2] = z_center - dzc;

                legend_shape_vertices[3] = x_center + dxc;
                legend_shape_vertices[4] = 0.0f;
                legend_shape_vertices[5] = z_center - dzc;

                legend_shape_vertices[6] = x_center + dxc;
                legend_shape_vertices[7] = 0.0f;
                legend_shape_vertices[8] = z_center + dzc;

                legend_shape_vertices[9] = x_center - dxc;
                legend_shape_vertices[10] = 0.0f;
                legend_shape_vertices[11] = z_center - dzc;

                legend_shape_colors[0] = face_color.red;
                legend_shape_colors[1] = face_color.green;
                legend_shape_colors[2] = face_color.blue;

                legend_shape_colors[3] = face_color.red;
                legend_shape_colors[4] = face_color.green;
                legend_shape_colors[5] = face_color.blue;

                legend_shape_colors[6] = face_color.red;
                legend_shape_colors[7] = face_color.green;
                legend_shape_colors[8] = face_color.blue;
                
                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 3, 3 * 3 * sizeof(float), GL_TRIANGLES);

                // Face 1
                legend_shape_vertices[3] = x_center - dxc;
                legend_shape_vertices[4] = 0.0f;
                legend_shape_vertices[5] = z_center + dzc;

                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 3, 3 * 3 * sizeof(float), GL_TRIANGLES);

                // Edge 0
                legend_shape_colors[0] = edge_color.red;
                legend_shape_colors[1] = edge_color.green;
                legend_shape_colors[2] = edge_color.blue;

                legend_shape_colors[3] = edge_color.red;
                legend_shape_colors[4] = edge_color.green;
                legend_shape_colors[5] = edge_color.blue;

                legend_shape_colors[6] = edge_color.red;
                legend_shape_colors[7] = edge_color.green;
                legend_shape_colors[8] = edge_color.blue;

                legend_shape_colors[9] = edge_color.red;
                legend_shape_colors[10] = edge_color.green;
                legend_shape_colors[11] = edge_color.blue;

                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 4, 4 * 3 * sizeof(float), GL_LINE_STRIP);

                // Edge 1
                legend_shape_vertices[3] = x_center + dxc;
                legend_shape_vertices[4] = 0.0f;
                legend_shape_vertices[5] = z_center - dzc;

                legend_shape_.renderAndUpdateData(legend_shape_vertices, legend_shape_colors, 4, 4 * 3 * sizeof(float), GL_LINE_STRIP);

            }
        }
        
    }

    glUseProgram(0);

    glUseProgram(shader_collection_.text_shader.programId());
    const GLint text_color_uniform = glGetUniformLocation(shader_collection_.text_shader.programId(), "textColor");
    glUniform3f(text_color_uniform, 0.0f, 0.0f, 0.0f);

    for(size_t k = 0; k < legend_properties.size(); k++)
    {
        const float kf = k;

        // Divided by three because the legend pane is in the coordinate system that spans [-3.0, 3.0]
        const float xp = (x_min + scale_factor_ * text_x_offset) / 3.0f;
        const float zp = (z_max - scale_factor_ * (z_offset + legend_element_z_delta * kf)) / 3.0f;
        text_renderer_.renderTextFromLeftCenter(legend_properties[k].name, xp, zp, scale_factor_ * 0.0005f, axes_width, axes_height);
    }

    glUseProgram(0);
}

LegendRenderer::LegendRenderer(const TextRenderer& text_renderer, const ShaderCollection& shader_collection) :
    text_renderer_{text_renderer},
    shader_collection_{shader_collection},
    points_(kMaxNumPoints * 3),
    colors_(kMaxNumPoints * 3),
    legend_inner_vertices_(18),
    legend_edge_vertices_(15)
{
    scale_factor_ = 1.0f;
    num_vertices_edge_ = 5;
    num_vertices_inner_ = 6;

    legend_inner_vertices_.fill(0.0f);
    legend_edge_vertices_.fill(0.0f);

    edge_vao_ = VAOObject(num_vertices_edge_, legend_edge_vertices_.data(), legend_color_edge, GL_LINE_STRIP);
    inner_vao_ = VAOObject(num_vertices_inner_, legend_inner_vertices_.data(), legend_color_inner, GL_TRIANGLES);
    legend_shape_ = VAOObject2(kMaxNumPoints, points_.data(), colors_.data());
}
