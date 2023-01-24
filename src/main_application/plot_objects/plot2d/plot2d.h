#ifndef MAIN_APPLICATION_PLOT_OBJECTS_PLOT2D_PLOT2D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_PLOT2D_PLOT2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Plot2D : public PlotObjectBase
{
private:
    VertexBuffer vertex_buffer_;
    float gap_size_, dash_size_;

    size_t num_points_;

    void findMinMax() override;

public:
    Plot2D();
    Plot2D(std::unique_ptr<const ReceivedData> received_data,
           const CommunicationHeader& hdr,
           const Properties& props,
           const ShaderCollection shader_collection, ColorPicker& color_picker);
    ~Plot2D();

    LegendProperties getLegendProperties() const override;
    void updateWithNewData(std::unique_ptr<const ReceivedData> received_data,
                           const CommunicationHeader& hdr,
                           const Properties& props) override;

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_PLOT2D_PLOT2D_H_
