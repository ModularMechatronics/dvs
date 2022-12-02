#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SURF_SURF_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SURF_SURF_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/misc/color_map.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Surf : public PlotObjectBase
{
private:
    VertexBuffer vertex_buffer2_;
    Dimension2D dims_;

    float* points_ptr_;
    float* mean_height_ptr_;
    float* normals_ptr_;

    size_t num_elements_to_render_;

    void findMinMax() override;

public:
    Surf();
    Surf(std::unique_ptr<const ReceivedData> received_data,
         const CommunicationHeader& hdr,
         const ShaderCollection shader_collection);
    ~Surf();

    LegendProperties getLegendProperties() const override;
    bool affectsColormapMinMax() const override;

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_SURF_SURF_H_
