#ifndef MAIN_APPLICATION_PLOT_OBJECTS_DRAW_PLANE_XZ_DRAW_PLANE_XZ_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_DRAW_PLANE_XZ_DRAW_PLANE_XZ_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawPlaneXZ : public PlotObjectBase
{
private:
    Point3d p00;
    Point3d p11;
    Point3d p01;
    Point3d p10;

    GLuint buffer_idx_;
    float* points_ptr_;

    void findMinMax() override;

public:
    DrawPlaneXZ();
    DrawPlaneXZ(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection);
    ~DrawPlaneXZ();

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_DRAW_PLANE_XZ_DRAW_PLANE_XZ_H_
