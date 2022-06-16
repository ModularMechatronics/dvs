#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SCATTER_SCATTER_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SCATTER_SCATTER_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Scatter2D : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;

    void findMinMax() override;

public:
    Scatter2D();
    Scatter2D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection);
    ~Scatter2D();

    void render() override;
    void modifyShader() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_SCATTER_SCATTER_H_
