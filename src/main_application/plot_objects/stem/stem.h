#ifndef MAIN_APPLICATION_PLOT_OBJECTS_STEM_STEM_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_STEM_STEM_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_header.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Stem : public PlotObjectBase
{
public:
    Stem();
    Stem(const CommunicationHeader& hdr,
         ReceivedData& received_data,
         const std::unique_ptr<const ConvertedDataBase>& converted_data,

         const PlotObjectAttributes& plot_object_attributes,
         const PropertiesData& properties_data,
         const ShaderCollection& shader_collection,
         ColorPicker& color_picker);
    ~Stem();

    LegendProperties getLegendProperties() const override;

    void render() override;
    void modifyShader() override;

    static std::unique_ptr<const ConvertedDataBase> convertRawData(const CommunicationHeader& hdr,
                                                                   const PlotObjectAttributes& attributes,
                                                                   const PropertiesData& properties_data,
                                                                   const uint8_t* const data_ptr);

private:
    VertexBuffer vertex_buffer_lines_, vertex_buffer_points_;
    GLuint lines_vertex_buffer_, lines_vertex_buffer_array_, points_vertex_buffer_, points_vertex_buffer_array_;

    void findMinMax() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_STEM_STEM_H_
