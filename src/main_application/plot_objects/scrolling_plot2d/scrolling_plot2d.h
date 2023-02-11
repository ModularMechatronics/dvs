#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SCROLLING_PLOT2D_SCROLLING_PLOT2D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SCROLLING_PLOT2D_SCROLLING_PLOT2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "misc/color_map.h"
#include "opengl_low_level/opengl_header.h"
#include "opengl_low_level/vertex_buffer.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class ScrollingPlot2D : public PlotObjectBase
{
public:
    ScrollingPlot2D();
    ScrollingPlot2D(const CommunicationHeader& hdr,
                    ReceivedData& received_data,
                    const std::unique_ptr<const ConvertedDataBase>& converted_data,
                    const Properties& props,
                    const ShaderCollection shader_collection,
                    ColorPicker& color_picker);
    ~ScrollingPlot2D();

    LegendProperties getLegendProperties() const override;
    void updateWithNewData(ReceivedData& received_data,
                           const CommunicationHeader& hdr,
                           const std::unique_ptr<const ConvertedDataBase>& converted_data,
                           const Properties& props) override;

    void render() override;

    static std::unique_ptr<const ConvertedDataBase> convertRawData(const PlotObjectAttributes& attributes,
                                                                   const uint8_t* const data_ptr);

private:
    // VertexBuffer vertex_buffer_;
    float* points_ptr_;
    float* dt_vec_;
    size_t previous_buffer_size_;
    GLuint sp_vertex_buffer_, sp_vertex_buffer_array_;

    void findMinMax() override;

    size_t num_elements_to_draw_;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_SCROLLING_PLOT2D_SCROLLING_PLOT2D_H_
