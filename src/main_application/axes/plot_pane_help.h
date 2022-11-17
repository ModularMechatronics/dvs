#ifndef MAIN_APPLICATION_AXES_PLOT_PANE_HELP_H_
#define MAIN_APPLICATION_AXES_PLOT_PANE_HELP_H_

#include <stddef.h>

#include "opengl_low_level/opengl_header.h"

class PlotPaneHelp
{
private:
    size_t num_vertices_;
    size_t num_corner_segments_;
    size_t num_corner_vertices_;
    size_t num_bytes_;
    float* data_array_;

    GLuint vertex_buffer_, vertex_buffer_array_;

public:
    PlotPaneHelp();
    ~PlotPaneHelp();

    void render(const float pane_width, const float pane_height);
};

#endif  // MAIN_APPLICATION_AXES_PLOT_PANE_HELP_H_
