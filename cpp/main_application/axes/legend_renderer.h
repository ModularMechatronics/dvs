#ifndef DVS_LEGEND_RENDERER_H_
#define DVS_LEGEND_RENDERER_H_

#include "opengl_low_level/opengl_header.h"

#include <stddef.h>

#include "axes/vao_object.h"

class LegendRenderer
{
private:
    GLuint vertex_buffer_edge_, vertex_buffer_array_edge_, color_buffer_edge_;
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;
    VAOObject edge_vao_;
    VAOObject inner_vao_;
    size_t num_vertices_;

public:
    LegendRenderer();

    void render();

};

#endif
