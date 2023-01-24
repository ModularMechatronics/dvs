#ifndef MAIN_APPLICATION_PLOT_OBJECTS_LINE_COLLECTION2_LINE_COLLECTION2_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_LINE_COLLECTION2_LINE_COLLECTION2_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class LineCollection2D : public PlotObjectBase
{
private:
    VertexBuffer vertex_buffer_;

    void findMinMax() override;

public:
    LineCollection2D();
    LineCollection2D(std::unique_ptr<const ReceivedData> received_data,
                     const CommunicationHeader& hdr,
                     const Properties& props,
                     const ShaderCollection shader_collection, ColorPicker& color_picker);
    ~LineCollection2D();

    LegendProperties getLegendProperties() const override;

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_LINE_COLLECTION2_LINE_COLLECTION2_H_
