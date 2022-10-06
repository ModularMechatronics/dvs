#ifndef MAIN_APPLICATION_PLOT_OBJECTS_DRAW_POLYGON_4_POINTS_DRAW_POLYGON_4_POINTS_H_
#define MAIN_APPLICATION_PLOT_OBJECTS_DRAW_POLYGON_4_POINTS_DRAW_POLYGON_4_POINTS_H_

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

    Point3d p0;
    Point3d p1;
    Point3d p2;
    Point3d p3;

    void findMinMax() override;

public:
    DrawPolygon4Points();
    DrawPolygon4Points(std::unique_ptr<const ReceivedData> received_data,
                       const CommunicationHeader& hdr,
                       const ShaderCollection shader_collection);

    void render() override;
};

#endif  // MAIN_APPLICATION_PLOT_OBJECTS_DRAW_POLYGON_4_POINTS_DRAW_POLYGON_4_POINTS_H_
