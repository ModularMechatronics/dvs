#ifndef IM_SHOW_H_
#define IM_SHOW_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/misc/color_map.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class ImShow : public PlotObjectBase
{
private:
    RGBColorMap<float> color_map_;
    GLuint buffer_idx_;
    GLuint color_buffer_;

    float* points_ptr_;
    float* colors_ptr_;

    uint8_t num_channels_;

    ImageC1<float> img_c1_;
    ImageC3<float> img_c3_;

    internal::Dimension2D dims_;
    unsigned char* pixel_data_;
    GLuint program_id_;
    GLuint texture_id_;
    int width;
    int height;
    void findMinMax() override;

public:
    ImShow();
    ImShow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~ImShow();

    void render() override;
};

#endif
