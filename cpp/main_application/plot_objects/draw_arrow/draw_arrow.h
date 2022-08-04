#ifndef MAIN_APPLICATION_PLOT_OBJECTS_DRAW_ARROW_DRAW_ARROW_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_DRAW_ARROW_DRAW_ARROW_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "main_application/plot_objects/utils.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawArrow : public PlotObjectBase
{
private:
    void findMinMax() override;

    float* points_ptr_;

    Point2Dd p0_;
    Point2Dd p1_;
    Point2Dd p1_left_;
    Point2Dd p1_right_;

public:
    DrawArrow();
    DrawArrow(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection);

    void render() override;
};

#endif // MAIN_APPLICATION_PLOT_OBJECTS_DRAW_ARROW_DRAW_ARROW_H_
