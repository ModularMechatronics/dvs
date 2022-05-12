#ifndef DRAW_PLANE_YZ_H_
#define DRAW_PLANE_YZ_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawPlaneYZ : public PlotObjectBase
{
private:
    Point3Dd p00;
    Point3Dd p11;
    Point3Dd p01;
    Point3Dd p10;

    GLuint buffer_idx_;
    float* points_ptr_;

    void findMinMax() override;

public:
    DrawPlaneYZ();
    DrawPlaneYZ(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection);
    ~DrawPlaneYZ();

    void render() override;
};

#endif
