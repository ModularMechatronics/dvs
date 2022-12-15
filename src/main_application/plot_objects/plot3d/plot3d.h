#ifndef MAIN_APPLICATION_PLOT_OBJECTS_PLOT3D_PLOT3D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_PLOT3D_PLOT3D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Plot3D : public PlotObjectBase
{
public:
private:
    VertexBuffer vertex_buffer2_;

    size_t num_points_;

    void findMinMax() override;

public:
    Plot3D();
    Plot3D(std::unique_ptr<const ReceivedData> received_data,
           const CommunicationHeader& hdr,
           const Properties& props,
           const ShaderCollection shader_collection);
    ~Plot3D();

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_PLOT3D_PLOT3D_H_
