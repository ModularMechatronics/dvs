#ifndef DVS_LEGEND_RENDERER_H_
#define DVS_LEGEND_RENDERER_H_

#include "opengl_low_level/opengl_header.h"

#include <stddef.h>
#include <vector>
#include <string>

#include "axes/vao_object.h"

class LegendRenderer
{
private:
    VAOObject edge_vao_;
    VAOObject inner_vao_;
    size_t num_vertices_edge_;
    size_t num_vertices_inner_;

public:
    LegendRenderer();

    void render(const std::vector<std::string>& legend_names);

};

#endif
