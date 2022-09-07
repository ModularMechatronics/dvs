#ifndef MAIN_APPLICATION_PLOT_OBJECTS_DRAW_LINE3D_DRAW_LINE3D_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_DRAW_LINE3D_DRAW_LINE3D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawLine3D : public PlotObjectBase
{
private:
    void findMinMax() override;

    float* points_ptr_;
    GLuint buffer_idx_;

    Point3d p0_;
    Point3d p1_;

public:
    DrawLine3D();
    DrawLine3D(std::unique_ptr<const ReceivedData> received_data, const TransmissionHeader& hdr, const ShaderCollection shader_collection);

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_DRAW_LINE3D_DRAW_LINE3D_H_
