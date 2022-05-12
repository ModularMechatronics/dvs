#ifndef DRAW_POLYGON_4_POINTS_H_
#define DRAW_POLYGON_4_POINTS_H_

#include <string>
#include <vector>

#include "dvs/math/math.h"
#include "opengl_low_level/data_structures.h"
#include "opengl_low_level/opengl_low_level.h"
#include "plot_objects/plot_object_base/plot_object_base.h"

class DrawPolygon4Points : public PlotObjectBase
{
private:
    RGBTripletf edge_color_;
    RGBTripletf face_color_;
    float line_width_;

    Point3Dd p0;
    Point3Dd p1;
    Point3Dd p2;
    Point3Dd p3;

    void findMinMax() override;

public:
    DrawPolygon4Points();
    DrawPolygon4Points(std::unique_ptr<const ReceivedData> received_data, const FunctionHeader& hdr, const ShaderCollection shader_collection);

    void render() override;
};

#endif
