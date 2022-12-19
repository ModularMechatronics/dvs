#ifndef MAIN_APPLICATION_AXES_LEGEND_RENDERER_H_
#define MAIN_APPLICATION_AXES_LEGEND_RENDERER_H_

#include <stddef.h>

#include <string>
#include <vector>

#include "axes/legend_properties.h"
#include "axes/text_rendering.h"
#include "dvs/math/math.h"
#include "opengl_low_level/opengl_header.h"
#include "opengl_low_level/vertex_buffer.h"
#include "shader.h"

class VAOObject2
{
private:
    size_t num_vertices_;
    size_t num_bytes_;
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;

public:
    VAOObject2() = default;
    VAOObject2(const size_t num_vertices, const float* const vertices, const float* const colors)
        : num_vertices_{num_vertices * 3}, num_bytes_{sizeof(float) * 3 * num_vertices_}
    {
        glGenVertexArrays(1, &vertex_buffer_array_);
        glBindVertexArray(vertex_buffer_array_);

        glGenBuffers(1, &vertex_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);
        glBufferData(GL_ARRAY_BUFFER, num_bytes_, vertices, GL_DYNAMIC_DRAW);

        glEnableVertexAttribArray(0);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &color_buffer_);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glBufferData(GL_ARRAY_BUFFER, num_bytes_, colors, GL_STATIC_DRAW);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    }

    void renderAndUpdateData(const float* const new_vertices,
                             const float* const new_colors,
                             const size_t num_vertices_to_render,
                             const size_t num_bytes_to_update,
                             const GLuint render_type)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_);

        glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_to_update, new_vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindBuffer(GL_ARRAY_BUFFER, color_buffer_);

        glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_to_update, new_colors);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(vertex_buffer_array_);
        glDrawArrays(render_type, 0, num_vertices_to_render);
        glBindVertexArray(0);
    }
};

class LegendRenderer
{
private:
    VertexBuffer edge_vao_;
    VertexBuffer inner_vao_;
    size_t num_vertices_edge_;
    size_t num_vertices_inner_;
    float scale_factor_;
    VAOObject2 legend_shape_;

    TextRenderer text_renderer_;
    ShaderCollection shader_collection_;

    dvs::Vector<float> points_;
    dvs::Vector<float> colors_;

    dvs::Vector<float> legend_inner_vertices_;
    dvs::Vector<float> legend_edge_vertices_;
    void renderColorMapLegend(const size_t num_segments,
                              const RGBColorMap<float>* const color_map,
                              const float xc,
                              const float yc,
                              const float r,
                              const float axes_width,
                              const float axes_height);
    void setVertexAtIdx(const float x, const float y, const float z, const size_t idx);
    void setColorAtIdx(const float r, const float g, const float b, const size_t idx);
    void setBoxValues(const float new_x_min, const float new_x_max, const float new_z_min, const float new_z_max);

public:
    LegendRenderer(const TextRenderer& text_renderer_, const ShaderCollection& shader_collection_);

    void render(const std::vector<LegendProperties>& legend_properties,
                const float axes_width,
                const float axes_height);
    void setLegendScaleFactor(const float new_scale_factor);
};

#endif  // MAIN_APPLICATION_AXES_LEGEND_RENDERER_H_
