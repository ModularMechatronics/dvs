#ifndef MAIN_APPLICATION_AXES_PLOT_BOX_SILHOUETTE_H_
#define MAIN_APPLICATION_AXES_PLOT_BOX_SILHOUETTE_H_

#include "opengl_low_level/opengl_header.h"
#include <stddef.h>

class PlotBoxSilhouette
{
private:
    GLuint vertex_buffer_, vertex_buffer_array_, color_buffer_;
    size_t num_vertices_;
public:
    PlotBoxSilhouette() = delete;
    PlotBoxSilhouette(const float size);

    void render() const;
};

#endif // MAIN_APPLICATION_AXES_PLOT_BOX_SILHOUETTE_H_
