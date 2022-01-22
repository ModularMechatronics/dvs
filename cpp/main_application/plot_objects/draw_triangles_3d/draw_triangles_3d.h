#ifndef DRAW_TRIANGLES_3D_H_
#define DRAW_TRIANGLES_3D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawTriangles3D : public PlotObjectBase
{
private:
    float* points_ptr_;

    GLuint buffer_handle_;

    void findMinMax() override;

public:
    DrawTriangles3D();
    DrawTriangles3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawTriangles3D();

    void render() override;
};

#endif
