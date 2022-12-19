#ifndef MAIN_APPLICATION_PLOT_OBJECTS_STAIRS_STAIRS_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_STAIRS_STAIRS_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Stairs : public PlotObjectBase
{
private:
    VertexBuffer vertex_buffer_;

    void findMinMax() override;

public:
    Stairs();
    Stairs(std::unique_ptr<const ReceivedData> received_data,
           const CommunicationHeader& hdr,
           const Properties& props,
           const ShaderCollection shader_collection);
    ~Stairs();

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_STAIRS_STAIRS_H_
