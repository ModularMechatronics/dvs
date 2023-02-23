#ifndef MAIN_APPLICATION_PLOT_OBJECTS_DRAW_MESH_DRAW_MESH_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_DRAW_MESH_DRAW_MESH_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_header.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawMesh : public PlotObjectBase
{
public:
    DrawMesh();
    DrawMesh(const CommunicationHeader& hdr,
             ReceivedData& received_data,
             std::unique_ptr<const ConvertedDataBase>& converted_data,
             const PlotObjectAttributes& plot_object_attributes,
             const PropertiesData& properties_data,
             const ShaderCollection shader_collection,
             ColorPicker& color_picker);
    ~DrawMesh();

    LegendProperties getLegendProperties() const override;
    void render() override;

    static std::unique_ptr<const ConvertedDataBase> convertRawData(const PlotObjectAttributes& attributes,
                                                                   const PropertiesData& properties_data,
                                                                   const uint8_t* const data_ptr);

private:
    VertexBuffer vertex_buffer_;
    std::unique_ptr<const ConvertedDataBase> converted_data_;
    float* points_ptr_;

    size_t num_elements_to_render_;

    uint32_t num_indices_;

    void findMinMax() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_DRAW_MESH_DRAW_MESH_H_
