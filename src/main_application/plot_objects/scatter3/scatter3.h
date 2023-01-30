#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SCATTER3_SCATTER3_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SCATTER3_SCATTER3_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Scatter3D : public PlotObjectBase
{
public:
    Scatter3D();
    Scatter3D(std::unique_ptr<const ReceivedData> received_data,
              const CommunicationHeader& hdr,
              const Properties& props,
              const ShaderCollection shader_collection,
              ColorPicker& color_picker,
              const bool initalize_gl);
    ~Scatter3D();

    void render() override;
    void modifyShader() override;
    LegendProperties getLegendProperties() const override;
    void initializeGL() override;

    struct OutputData
    {
        float* points_ptr;
        float* color_ptr;
    };

private:
    VertexBuffer vertex_buffer_;
    void findMinMax() override;

    OutputData output_data_;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_SCATTER3_SCATTER3_H_
