#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SURF_SURF_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SURF_SURF_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/misc/color_map.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Surf : public PlotObjectBase
{
private:
    Dimension2D dims_;
    // RGBColorMap<float> color_map_;
    GLuint buffer_idx_;

    float* points_ptr_;
    float* colors_ptr_;

    bool face_color_set_;

    void findMinMax() override;

public:
    Surf();
    Surf(std::unique_ptr<const ReceivedData> received_data, const TransmissionHeader& hdr, const ShaderCollection shader_collection);
    ~Surf();

    LegendProperties getLegendProperties() const override;

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_SURF_SURF_H_
