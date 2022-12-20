#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SCATTER_SCATTER_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SCATTER_SCATTER_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Scatter2D : public PlotObjectBase
{
private:
    VertexBuffer vertex_buffer_;
    void findMinMax() override;

public:
    Scatter2D();
    Scatter2D(std::unique_ptr<const ReceivedData> received_data,
              const CommunicationHeader& hdr,
              const Properties& props,
              const ShaderCollection shader_collection);
    ~Scatter2D();

    void render() override;
    void modifyShader() override;
    LegendProperties getLegendProperties() const override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_SCATTER_SCATTER_H_
