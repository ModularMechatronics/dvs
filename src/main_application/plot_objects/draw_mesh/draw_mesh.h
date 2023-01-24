#ifndef MAIN_APPLICATION_PLOT_OBJECTS_DRAW_MESH_DRAW_MESH_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_DRAW_MESH_DRAW_MESH_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_low_level.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawMesh : public PlotObjectBase
{
private:
    VertexBuffer vertex_buffer_;
    float* points_ptr_;

    size_t num_elements_to_render_;

    uint32_t num_vertices_;
    uint32_t num_indices_;

    void findMinMax() override;

public:
    DrawMesh();
    DrawMesh(std::unique_ptr<const ReceivedData> received_data,
             const CommunicationHeader& hdr,
             const Properties& props,
             const ShaderCollection shader_collection,
             ColorPicker& color_picker);
    ~DrawMesh();

    LegendProperties getLegendProperties() const override;
    bool affectsColormapMinMax() const override;
    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_DRAW_MESH_DRAW_MESH_H_
