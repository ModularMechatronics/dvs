#ifndef MAIN_APPLICATION_PLOT_OBJECTS_SCATTER3_SCATTER3_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_SCATTER3_SCATTER3_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Scatter3D : public PlotObjectBase
{
private:
    uint8_t* points_ptr_;

    void findMinMax() override;

public:
    Scatter3D();
    Scatter3D(std::unique_ptr<const ReceivedData> received_data,
              const CommunicationHeader& hdr,
              const ShaderCollection shader_collection);
    ~Scatter3D();

    void render() override;
    void modifyShader() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_SCATTER3_SCATTER3_H_
