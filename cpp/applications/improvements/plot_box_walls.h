#ifndef DVS_DRAW_PLOT_BOX_H_
#define DVS_DRAW_PLOT_BOX_H_

#include "opengl_low_level/opengl_header.h"
#include <stddef.h>


class PlotBoxWalls
{
private:
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;
    size_t num_vertices_;

public:
    PlotBoxWalls() = delete;
    PlotBoxWalls(const float size);
    void render(const float azimuth, const float elevation);
};


#endif
