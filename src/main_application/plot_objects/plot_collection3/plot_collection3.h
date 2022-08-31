#ifndef MAIN_APPLICATION_PLOT_OBJECTS_PLOT_COLLECTION3_PLOT_COLLECTION3_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_PLOT_COLLECTION3_PLOT_COLLECTION3_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class PlotCollection3D : public PlotObjectBase
{
private:
    float* points_ptr_;

    uint32_t num_points_;
    uint32_t num_objects_;

    void findMinMax() override;

public:
    PlotCollection3D();
    PlotCollection3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection);
    ~PlotCollection3D();

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_PLOT_COLLECTION3_PLOT_COLLECTION3_H_
