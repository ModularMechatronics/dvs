#ifndef MAIN_APPLICATION_PLOT_OBJECTS_PLOT3D_PLOT3D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_PLOT3D_PLOT3D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class Plot3D : public PlotObjectBase
{
public:
    struct InputData
    {
        float* p0;
        float* p1;
        float* p2;
        float* idx_data_;
    };
private:
    InputData input_data_;

    GLuint idx_buffer_;
    GLuint p0_vertex_buffer_, p1_vertex_buffer_, p2_vertex_buffer_;

    void findMinMax() override;

public:
    Plot3D();
    Plot3D(std::unique_ptr<const ReceivedData> received_data, const TransmissionHeader& hdr, const ShaderCollection shader_collection);
    ~Plot3D();

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_PLOT3D_PLOT3D_H_
