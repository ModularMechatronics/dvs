#ifndef DRAW_TRIANGLE_2D_H_
#define DRAW_TRIANGLE_2D_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawTriangle3D : public PlotObjectBase
{
private:
    Triangle3D<double> triangle_;

    uint8_t* points_ptr_;
    GLuint buffer_idx_;

    void findMinMax() override;

public:
    DrawTriangle3D();
    DrawTriangle3D(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr);
    ~DrawTriangle3D();

    void render() override;
};

#endif
