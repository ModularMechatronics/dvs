#ifndef MAIN_APPLICATION_PLOT_OBJECTS_FAST_PLOT2D_FAST_PLOT2D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_FAST_PLOT2D_FAST_PLOT2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Plot2D : public PlotObjectBase
{
private:
    float* points_ptr_;

    void findMinMax() override;

public:
    Plot2D();
    Plot2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection);
    ~Plot2D();

    LegendProperties getLegendProperties() const override;

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_FAST_PLOT2D_FAST_PLOT2D_H_
