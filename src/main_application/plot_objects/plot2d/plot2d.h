#ifndef MAIN_APPLICATION_PLOT_OBJECTS_PLOT2D_PLOT2D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_PLOT2D_PLOT2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Plot2D : public PlotObjectBase
{
private:
    float gap_size_, dash_size_;

    size_t num_points_;
    GLuint idx_buffer_;
    GLuint p0_vertex_buffer_, p1_vertex_buffer_, p2_vertex_buffer_, length_along_vertex_buffer_;

    void findMinMax() override;

public:
    Plot2D();
    Plot2D(std::unique_ptr<const ReceivedData> received_data,
           const CommunicationHeader& hdr,
           const ShaderCollection shader_collection);
    ~Plot2D();

    LegendProperties getLegendProperties() const override;

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_PLOT2D_PLOT2D_H_
