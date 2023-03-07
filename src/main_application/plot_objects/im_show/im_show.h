#ifndef MAIN_APPLICATION_PLOT_OBJECTS_IM_SHOW_IM_SHOW_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_IM_SHOW_IM_SHOW_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/misc/color_map.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_header.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class ImShow : public PlotObjectBase
{
public:
    ImShow();
    ImShow(const CommunicationHeader& hdr,
           ReceivedData& received_data,
           const std::unique_ptr<const ConvertedDataBase>& converted_data,

           const PlotObjectAttributes& plot_object_attributes,
           const PropertiesData& properties_data,
           const ShaderCollection& shader_collection,
           ColorPicker& color_picker);
    ~ImShow();

    void render() override;
    static std::unique_ptr<const ConvertedDataBase> convertRawData(const CommunicationHeader& hdr,
                                                                   const PlotObjectAttributes& attributes,
                                                                   const PropertiesData& properties_data,
                                                                   const uint8_t* const data_ptr);

private:
    uint8_t num_channels_;
    unsigned int texture_handle_;
    unsigned int vertex_buffer_object_, vertex_array_object_, extended_buffer_object_;
    Vector<float> vertices_;
    Vector<unsigned int> indices_;

    internal::Dimension2D dims_;
    int width_;
    int height_;
    void findMinMax() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_IM_SHOW_IM_SHOW_H_
