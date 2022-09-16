#ifndef MAIN_APPLICATION_AXES_PLOT_BOX_SILHOUETTE_H_
#define MAIN_APPLICATION_AXES_PLOT_BOX_SILHOUETTE_H_

#include "opengl_low_level/opengl_header.h"
#include <stddef.h>

class PlotBoxSilhouette
{
private:
    GLuint vertex_buffer_, vertex_buffer_array_;
    size_t num_vertices_;
    void setIndices(const size_t first_vertex_idx, const size_t last_vertex_idx, const size_t dimension_idx, const float val);

    static constexpr size_t kXYFirstIdx = 0;
    static constexpr size_t kXYLastIdx = 6;
    static constexpr size_t kXYChangeDimension = 2;

    static constexpr size_t kYZFirstIdx = 6;
    static constexpr size_t kYZLastIdx = 12;
    static constexpr size_t kYZChangeDimension = 0;

    static constexpr size_t kXZFirstIdx = 12;
    static constexpr size_t kXZLastIdx = 18;
    static constexpr size_t kXZChangeDimension = 1;

    float* data_array_;

public:
    PlotBoxSilhouette();
    ~PlotBoxSilhouette();

    void render(const float azimuth, const float elevation);
};

#endif // MAIN_APPLICATION_AXES_PLOT_BOX_SILHOUETTE_H_
